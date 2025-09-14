#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>

#include "../config/config.h"
#include "../peers/peers.h"
#include "../logger/logger.h"

int bindToBest(struct addrinfo* addr_info) {
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
    debug("Successfully bound to %s Service: %s\n", host, service);

    return socket_fd;
  }

  return -1;
}

void* parseHostsfile(struct Peer* peers[]) {
  FILE *hostsfile;

  hostsfile = fopen(hostsFilePath, "r");
  if (hostsfile == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char line[maxPeerNameSize];
  while(fgets(line, maxPeerNameSize, hostsfile) != NULL) {
    if (numPeers > maxPeers) {
      info("Exeeded maximum number of supported peers.\n");
      exit(1);
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    
    struct Peer* peer = malloc(sizeof(struct Peer));
    peer->name = malloc(sizeof(maxPeerNameSize));

    peer->id = numPeers + 1;
    strcpy(peer->name, line);
    peer->connected = 0;

    peers[numPeers] = peer;
    populatePeerInfo(peer);
    
    numPeers += 1;
  }

  if (numPeers > 0) {
    debug("Number of peers: %d\n", numPeers);
    for (int i = 0; i < numPeers; i++) {
      debug("Peer %d: %s\n", peers[i]->id, peers[i]->name);
    }
  } else {
    info("Hostsfile empty");
    exit(1);
  }
  
  fclose(hostsfile);
  
  return NULL;
}