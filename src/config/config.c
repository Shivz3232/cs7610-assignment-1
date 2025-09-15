#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "../logger/logger.h"

// DEFAULTS
// char* cEnv = "development"; // To enable debug logs
char* cEnv = "production";
char* port = "3000";
char* hostsFilePath = "hostsfile.txt";
int maxPeerNameSize = 100;
int maxPeers = 100;
int maxMessageSize = 100;

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
    debug("C_ENV not found, defaulting to: %s", cEnv);
  } else {
    cEnv = strdup(value);
    debug("C_ENV set to %s", cEnv);
  }
  
  return NULL;
}

void* initializePort() {
  char* value = getenv("PORT");

  if (!value) {
    debug("PORT not found, defaulting to: %s", port);
  } else {
    port = strdup(value);
    debug("PORT set to %s", port);
  }

  return NULL;
}

void* initializeHostsFilePath() {
  char* value = getenv("HOSTSFILE_PATH");

  if (!value) {
    debug("HOSTSFILE_PATH not found, defaulting to: %s", hostsFilePath);
  } else {
    hostsFilePath = strdup(value);
    debug("HOSTSFILE_PATH set to %s", hostsFilePath);
  }

  return NULL;
}

void* initializeMaxPeerNameSize() {
  char* value = getenv("MAX_PEER_NAME_SIZE");

  if (!value) {
    debug("MAX_PEER_NAME_SIZE not found, defaulting to: %s", maxPeerNameSize);
  } else {
    maxPeerNameSize = atoi(value);
    debug("MAX_PEER_NAME_SIZE set to %s", maxPeerNameSize);
  }
  
  return NULL;
}

void* initializeMaxPeers() {
  char* value = getenv("MAX_PEERS");

  if (!value) {
    debug("MAX_PEERS not found, defaulting to: %s", maxPeers);
  } else {
    maxPeers = atoi(value);
    debug("MAX_PEERS set to %s", maxPeers);
  }
  
  return NULL;
}

void* initializeMaxMessageSize() {
  char* value = getenv("MAX_MESSAGE_SIZE");

  if (!value) {
    debug("MAX_MESSAGE_SIZE not found, defaulting to: %s", maxMessageSize);
  } else {
    maxMessageSize = atoi(value);
    debug("MAX_MESSAGE_SIZE set to %s", maxMessageSize);
  }
  
  return NULL;
}
