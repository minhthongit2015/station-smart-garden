
#pragma once
#ifndef SMART_GARDEN_WEBSOCKET_CONTROLLER_H
#define SMART_GARDEN_WEBSOCKET_CONTROLLER_H

#define NODEBUG_SOCKETIOCLIENT

#include <SocketIoClient.h>
#include "../base/utils.hpp"
#include "../variables/global.hpp"

void handleConnectEvent(const char * payload, size_t length);
void handleDisconnectEvent(const char * payload, size_t length);
void handleAcceptEvent(const char * payload, size_t length);
void handleCommandEvent(const char * payload, size_t length);

class WebsocketController : public SocketIoClient {
  public:
    bool connected = false;
    unsigned long disconnectedAt = 0;
    bool deepSleep = false;
    unsigned long waitBeforeSleep = 15000;
    unsigned long checkOnSleepInterval = 30000;
    WebsocketController() {
    }

    void setup();
    void loop();
} websocketCtl;

void WebsocketController::setup() {
  logStart("Websocket");
  on("connect", handleConnectEvent);
  on("disconnect", handleDisconnectEvent);
  on("accept", handleAcceptEvent);
  on("command", handleCommandEvent);
  prf("> [Websocket] Connecting to -~=> %s : %d\r\n",
    Global::cfg.gardenHost.c_str(), Global::cfg.gardenPort);
  begin(Global::cfg.gardenHost.c_str(), Global::cfg.gardenPort);
  // webSocket.setAuthorization("username", "password");
}

void WebsocketController::loop() {
  static unsigned long last = 0;
  
  if (!deepSleep) {
    if (!connected && disconnectedAt == 0) { // First time disconnect
      disconnectedAt = millis();
    }
    if (millis() - disconnectedAt > waitBeforeSleep) {
      deepSleep = true;
      logz("Websocket", "Deeply sleep... zzZ...");
    }
  } else {
    if (millis() - last < checkOnSleepInterval) {
      return;
    } else {
      logz("Websocket", "Wake up at the mid night.");
    }
  }
  last = millis();
  SocketIoClient::loop();
}

void handleConnectEvent(const char * payload, size_t length) {
  websocketCtl.deepSleep = false;
  websocketCtl.disconnectedAt = 0;
  logz("Websocket", "Connected to Garden");
  websocketCtl.emit(POST stationConnectEndpoint, DEVICE_INFO);
}

void handleDisconnectEvent(const char * payload, size_t length) {
  if (websocketCtl.connected) {
    logz("Websocket", "Disconnected from Garden!");
    websocketCtl.connected = false;
  }
}

void handleAcceptEvent(const char * payload, size_t length) {
  logz("Websocket", "Garden accepted!");
  websocketCtl.connected = true;
  websocketCtl.emit(POST stationStateEndpoint, Global::state.toJSON());
}

void handleCommandEvent(const char * payload, size_t length) {
  logz("Websocket", "Garden Command!");
  deserializeJson(Global::state.doc, payload);
  serializeJsonPretty(Global::state.doc, Serial);
  relayCtl.syncState();
}

#endif