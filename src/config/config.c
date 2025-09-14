#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../logger/logger.h"

char* cEnv;
char* port;
char* hostsFilePath;
int maxPeerNameSize;
int maxPeers;
int maxMessageSize;

void* initializeCEnv();
void* initializePort();
void* initializeHostsFilePath();
void* initializeMaxPeerNameSize();
void* initializeMaxPeers();
void* initializeMaxMessageSize();

void* initializeEnvVariables() {
  initializeCEnv();
  initializePort();
  initializeHostsFilePath();
  initializeMaxPeerNameSize();
  initializeMaxPeers();
  initializeMaxMessageSize();

  return NULL;
}

void* initializeCEnv() {
  char* value = getenv("C_ENV");

  if (!value) {
    cEnv = strdup("3000");
    info("C_ENV not set, using default: %s\n", cEnv);
  } else {
    cEnv = strdup(value);
    debug("C_ENV: %s\n", cEnv);
  }
  
  return NULL;
}

void* initializePort() {
  char* value = getenv("PORT");

  if (!value) {
    port = strdup("3000");
    info("PORT not set, using default: %s\n", port);
  } else {
    port = strdup(value);  // Copy to heap
    debug("PORT: %s\n", port);
  }

  return NULL;
}

void* initializeHostsFilePath() {
  char* value = getenv("HOSTSFILE_PATH");

  if (!value) {
    hostsFilePath = strdup("hostsfile.txt");
    info("HOSTSFILE_PATH not set, using default: %s\n", hostsFilePath);
  } else {
    hostsFilePath = strdup(value);
    debug("HOSTSFILE_PATH: %s\n", hostsFilePath);
  }

  return NULL;
}

void* initializeMaxPeerNameSize() {
  char* value = getenv("MAX_PEER_NAME_SIZE");

  if (!value) {
    maxPeerNameSize = 100;
    info("MAX_PEER_NAME_SIZE not set, using default: %d\n", maxPeerNameSize);
  } else {
    maxPeerNameSize = atoi(value);
    debug("MAX_PEER_NAME_SIZE: %d\n", maxPeerNameSize);
  }
  
  return NULL;
}

void* initializeMaxPeers() {
  char* value = getenv("MAX_PEERS");

  if (!value) {
    maxPeers = 100;
    info("MAX_PEERS not set, using default: %d\n", maxPeers);
  } else {
    maxPeers = atoi(value);
    debug("MAX_PEERS: %d\n", maxPeers);
  }
  
  return NULL;
}

void* initializeMaxMessageSize() {
  char* value = getenv("MAX_MESSAGE_SIZE");

  if (!value) {
    maxMessageSize = 100;
    info("MAX_MESSAGE_SIZE not set, using default: %d\n", maxMessageSize);
  } else {
    maxMessageSize = atoi(value);
    debug("MAX_MESSAGE_SIZE: %d\n", maxMessageSize);
  }
  
  return NULL;
}
