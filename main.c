#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <pthread.h>

#define BACKLOG 10
#define MAX_PEERS 100
#define MAX_PEER_NAME_SIZE 100
#define MAX_MESSAGE_SIZE 100

struct Peer {
  int id;
  char* name[MAX_PEER_NAME_SIZE];
  int socket_fd;
  struct addrinfo* addr_info;
  struct addrinfo* chosen_addr_info;
  int connected;
};

char* port;
int numPeers = 0;
int connectedPeers = 0;
struct Peer* peers[MAX_PEERS]; // @TODO Free memory allocated to peers

int bind_to_best(struct addrinfo*);
void* hear(void*);
void broadcast(void*);
void* populate_peer_info(struct Peer*);
void* mark_peer_as_connected(struct sockaddr_in*, socklen_t*);

void* initializePort();
void* parseHostsfile();

int main(int argc, char const* argv[]) {
  fprintf(stderr, "Process started\n");

  initializePort();
  fprintf(stderr, "============================================\n\n");

  parseHostsfile();
  fprintf(stderr, "============================================\n\n");

  struct addrinfo hints, *addr_info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, port, &hints, &addr_info) < 0) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "Creating socket.\n");
  int socket_fd;

  if ((socket_fd = bind_to_best(addr_info)) < 0) {
    perror("bind_to_best");
    exit(2);
  }
  fprintf(stderr, "Socket created successfully\n");

  fprintf(stderr, "============================================\n\n");

  pthread_t listener_thread;
  pthread_create(&listener_thread, NULL, hear, &socket_fd);
  pthread_detach(listener_thread);

  char message[5] = "PING";
  while (connectedPeers < numPeers) {
    for (int i = 0; i < numPeers; i++) {
      if (peers[i]->connected) {
        continue;
      }
      
      int num_bytes;
      if ((num_bytes = sendto(socket_fd, &message, strlen(message), 0, peers[i]->chosen_addr_info->ai_addr, peers[i]->chosen_addr_info->ai_addrlen)) < 0) {
          perror("sendto");
          exit(1);
      }
    }

    sleep(10);
  }

  fprintf(stderr, "READY\n");

  fprintf(stderr, "============================================\n\n");

  close(socket_fd);
  freeaddrinfo(addr_info);

  fprintf(stderr, "Socket closed\n");
  fprintf(stderr, "Processs finished\n");

  fprintf(stderr, "============================================\n\n");

  return 0;
}

int bind_to_best(struct addrinfo* addr_info) {
  int yes = 1;

  int socket_fd;
  
  for (struct addrinfo* p = addr_info; p != NULL; p = p->ai_next) {
    if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      perror("socket");
      continue;
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }

    if (bind(socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
      close(socket_fd);
      perror("bind");
      continue;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    getnameinfo(addr_info->ai_addr, addr_info->ai_addrlen, host, sizeof(host), service, sizeof(service), 0);
    fprintf(stderr, "Successfully bound to %s Service: %s\n", host, service);

    return socket_fd;
  }

  return -1;
}

void* hear(void* input) {
  fprintf(stderr, "Listener started\n");
  
  int peer_fd = *(int*)input;
  
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);

  int numbytes;
  char message[MAX_MESSAGE_SIZE];
  while (1) {
    memset(&message, 0, (size_t) MAX_MESSAGE_SIZE);
    
    if ((numbytes = recvfrom(peer_fd, message, MAX_MESSAGE_SIZE-1, 0, (struct sockaddr*)&peer_addr, &peer_addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }

    mark_peer_as_connected(&peer_addr, &peer_addr_len);
  }

  fprintf(stderr, "Listener finished.\n");

  return NULL;
}

void* populate_peer_info(struct Peer* peer) {
  struct addrinfo hints, *addr_info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(peer->name, port, &hints, &addr_info) < 0) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  peer->addr_info = addr_info;

  int socket_fd;
  for (struct addrinfo* p = addr_info; p != NULL; p = p->ai_next) {
    if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      perror("socket");
      continue;
    }


    peer->chosen_addr_info = p;
    peer->socket_fd = socket_fd;
    
    break;
  }
  
  return NULL;
}

void* mark_peer_as_connected(struct sockaddr_in* peer_addr, socklen_t* peer_addr_len) {
  char peer_name[MAX_PEER_NAME_SIZE];
  
  if (getnameinfo(peer_addr, *peer_addr_len, peer_name, MAX_PEER_NAME_SIZE, NULL, 0, NI_NAMEREQD) < 0) {
    perror("getnameinfo");
    exit(EXIT_FAILURE);
  }

  // REDUCE FQDN by trimming at the first dot
  char *dot = strchr(peer_name, '.');
  if (dot) {
    *dot = '\0';
  }
  
  for (int i = 0; i < numPeers; i++) {
    if (strcmp(peers[i]->name, peer_name) == 0 && peers[i]->connected == 0) {
      peers[i]->connected = 1;
      connectedPeers += 1;

      fprintf(stderr, "Marked peer %s as connected\n", peer_name);
      
      return NULL;
    }
  }

  fprintf(stderr, "Recieived message from an unknown peer: %s\n", peer_name);
  
  return NULL;
}

void* initializePort() {
  fprintf(stderr, "Initializing port\n");
  
  port = getenv("PORT");
  if (port) {
    fprintf(stderr, "Initialized port to: %s\n", port);
  } else {
    fprintf(stderr, "Failed to initialize port.\n");
    exit(1);
  }

  return NULL;
}

void* parseHostsfile() {
  fprintf(stderr, "Parsing hostsfile.txt\n");
  
  FILE *hostsfile;

  hostsfile = fopen("hostsfile.txt", "r");
  if (hostsfile == NULL) {
    perror("Error opening hostsfile");
    exit(1);
  }

  char line[MAX_PEER_NAME_SIZE];
  while(fgets(line, MAX_PEER_NAME_SIZE, hostsfile) != NULL) {
    if (numPeers > MAX_PEERS) {
      fprintf(stderr, "Exeeded maximum number of supported peers.\n");
      exit(1);
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    
    struct Peer* peer = malloc(sizeof(struct Peer));
    peer->id = numPeers + 1;
    strcpy(peer->name, line);
    peer->connected = 0;

    peers[numPeers] = peer;
    populate_peer_info(peer);
    
    numPeers += 1;
  }

  if (numPeers > 0) {
    fprintf(stderr, "Number of peers: %d\n", numPeers);
    for (int i = 0; i < numPeers; i++) {
      fprintf(stderr, "Peer %d: %s\n", peers[i]->id, peers[i]->name);
    }
  } else {
    fprintf(stderr, "Hostfile empty\n");
    exit(1);
  }
  
  fclose(hostsfile);
  
  return NULL;
}