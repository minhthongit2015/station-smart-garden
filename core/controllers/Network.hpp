
#pragma once
#ifndef BEYOND_GARDEN_WEBSOCKET_CONTROLLER_H
#define BEYOND_GARDEN_WEBSOCKET_CONTROLLER_H

// #define NODEBUG_SOCKETIOCLIENT

// #include <SocketIoClient.h>
#include "../base/socket-io-client.hpp"
#include "../base/utils.hpp"
#include "../variables/Configuration.hpp"
#include "../variables/State.hpp"
#include "./RelayController.hpp"
#include "../base/WifiManager.hpp"

WiFiClient client;

void handleConnectEvent(const char *payload, size_t length);
void handleDisconnectEvent(const char *payload, size_t length);
void handleAcceptEvent(const char *payload, size_t length);
void handleCommandEvent(const char *payload, size_t length);

class Network : public SocketIoClient {
  public:
    bool connected = false;
    unsigned long disconnectedAt = 0;
    bool deepSleep = false;
    unsigned long waitBeforeSleep = 30000;
    unsigned long checkOnSleepInterval = 30000;
    int maxRetryTimes = 10;
    bool isInitialized = false;

    bool initialize() {
      log("Websocket", "Initializing...");
      if (testServer()) {
        isInitialized = true;
        deepSleep = false;
        disconnectedAt = false;
        begin(cfg.gardenHost.c_str(), cfg.gardenPort, getConnectionUrlPath());
      }
      return isInitialized;
    }

    bool testServer() {
      bool isServerUp = client.connect(cfg.gardenHost.c_str(), cfg.gardenPort);
      client.stop();
      return isServerUp;
    }

    const char *getConnectionUrlPath() {
      static char urlPathBuffer[100];
      #define DEFAULT_URL_PATH "/socket.io/?EIO=3&transport=websocket"
      #define WITH_SESSION_URL_PATH DEFAULT_URL_PATH "&token=%s"
      
      if (cfg.sessionId.length() > 0) {
        sprintf(urlPathBuffer, WITH_SESSION_URL_PATH, cfg.sessionId.c_str());
        return urlPathBuffer;
      } else {
        return DEFAULT_URL_PATH;
      }
    }

    void setup();
    void loop();
} network;

extern Network network;

void Network::setup() {
  logStart("Websocket");
  on("connect", handleConnectEvent);
  on("disconnect", handleDisconnectEvent);
  on("accept", handleAcceptEvent);
  on("command", handleCommandEvent);
  prf("> [Websocket] Connecting to -~=> %s : %d\r\n", cfg.gardenHost.c_str(), cfg.gardenPort);
  if (wifiMgr.isConnected()) {
    initialize();
  }
  // webSocket.setAuthorization("username", "password");
}

void Network::loop() {
  static unsigned long last = 0;
  static int retryTimes = 0;
  if (!wifiMgr.isConnected()) {
    return;
  }
  
  if (!deepSleep) {
    if (!connected) {
      if (disconnectedAt == 0) { // First time disconnect
        disconnectedAt = millis();
      }
      if (millis() - disconnectedAt > waitBeforeSleep) {
        log("Websocket", "Deeply sleep... zzZ...");
        retryTimes = 0;
        deepSleep = true;
        last = millis();
        return;
      }
    }
  } else {
    if (millis() - last < checkOnSleepInterval) {
      return;
    } else {
      log("Websocket", "Wake up at the mid night.");
      ++retryTimes;
      if (retryTimes > maxRetryTimes) {
        return reset();
      }
      if (!isInitialized) {
        initialize();
      } else {
        network.emit(POST VerifyStationEndpoint, DEVICE_INFO);
      }
    }
  }
  last = millis();
  if (isInitialized) {
    SocketIoClient::loop();
  }
}

void handleConnectEvent(const char *payload, size_t length) {
  network.deepSleep = false;
  network.disconnectedAt = 0;
  log("Websocket", "Connected to Garden");
  network.emit(POST VerifyStationEndpoint, DEVICE_INFO);
}

void handleDisconnectEvent(const char *payload, size_t length) {
  if (network.connected) {
    log("Websocket", "Disconnected from Garden!");
    network.connected = false;
  }
}

void handleAcceptEvent(const char *payload, size_t length) {
  log("Websocket", "Garden accepted!");
  network.connected = true;
  cfg.sessionId = payload;
  cfg.saveConfigurations();
  network.emit(POST RecordsEndpoint, state.toJSON());
}

void handleCommandEvent(const char *payload, size_t length) {
  log("Websocket", "Garden Command!");
  deserializeJson(state.doc, payload);
  serializeJsonPretty(state.doc, Serial);
  state.fromDoc(state.doc);
  relays.syncState();
}

#endif