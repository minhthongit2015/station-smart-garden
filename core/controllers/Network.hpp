
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
    bool initialized = false;

    void initialize() {
      initialized = true;
      begin(cfg.gardenHost.c_str(), cfg.gardenPort);
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
  prf("> [Websocket] Connecting to -~=> %s : %d\r\n",
    cfg.gardenHost.c_str(), cfg.gardenPort);
  if (wifiMgr.isConnected()) {
    initialize();
  }
  // webSocket.setAuthorization("username", "password");
}

void Network::loop() {
  static unsigned long last = 0;
  if (!wifiMgr.isConnected()) {
    return;
  }
  
  if (!deepSleep) {
    if (connected) {

    } else {
      if (disconnectedAt == 0) { // First time disconnect
        disconnectedAt = millis();
      }
      if (millis() - disconnectedAt > waitBeforeSleep) {
        deepSleep = true;
        log("Websocket", "Deeply sleep... zzZ...");
        return;
      }
    }
  } else {
    if (millis() - last < checkOnSleepInterval) {
      return;
    } else {
      network.emit(POST VerifyStationEndpoint, DEVICE_INFO);
      log("Websocket", "Wake up at the mid night.");
    }
  }
  last = millis();
  SocketIoClient::loop();
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
  network.emit(POST RecordsEndpoint, state.toJSON());
}

void handleCommandEvent(const char *payload, size_t length) {
  log("Websocket", "Garden Command!");
  deserializeJson(state.doc, payload);
  serializeJsonPretty(state.doc, Serial);
  relays.syncState();
}

#endif