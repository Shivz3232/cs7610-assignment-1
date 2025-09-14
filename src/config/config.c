#include "config.h"

#include <stdio.h>
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
  cEnv = getenv("C_ENV");

  if (!cEnv) {
    fprintf(stderr, "Failed to initialize C_ENV.\n");
    exit(1);
  }
  
  return NULL;
}

void* initializePort() {
  port = getenv("PORT");
  if (port) {
    debug("PORT: %s\n", port);
  } else {
    info("Failed to initialize PORT.\n");
    exit(1);
  }

  return NULL;
}

void* initializeHostsFilePath() {
  hostsFilePath = getenv("HOSTSFILE_PATH");
  if (hostsFilePath) {
    debug("HOSTSFILE_PATH: %s\n", hostsFilePath);
  } else {
    info("Failed to initialize HOSTSFILE_PATH.\n");
    exit(1);
  }

  return NULL;
}

void* initializeMaxPeerNameSize() {
  char const* value = getenv("MAX_PEER_NAME_SIZE");
  if (value) {
    debug("MAX_PEER_NAME_SIZE: %s\n", value);
  } else {
    info("Failed to initialize MAX_PEER_NAME_SIZE.\n");
    exit(1);
  }

  maxPeerNameSize = atoi(value);
  
  return NULL;
}

void* initializeMaxPeers() {
  char const* value = getenv("MAX_PEERS");
  if (value) {
    debug("MAX_PEERS: %s\n", value);
  } else {
    info("Failed to initialize MAX_PEERS.\n");
    exit(1);
  }

  maxPeers = atoi(value);
  
  return NULL;
}

void* initializeMaxMessageSize() {
  char const* value = getenv("MAX_MESSAGE_SIZE");
  if (value) {
    debug("MAX_MESSAGE_SIZE: %s\n", value);
  } else {
    info("Failed to initialize MAX_MESSAGE_SIZE.\n");
    exit(1);
  }

  maxMessageSize = atoi(value);
  
  return NULL;
}
