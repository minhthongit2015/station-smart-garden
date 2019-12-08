
#pragma once
#ifndef SMART_GARDEN_WEBSOCKET_CONTROLLER_H
#define SMART_GARDEN_WEBSOCKET_CONTROLLER_H

#include <SocketIoClient.h>
#include "../base/utils.hpp"
#include "../variables/global.hpp"

#define SOCKETIOCLIENT_DEBUG(...)

void handleConnectEvent(const char * payload, size_t length);
void handleDisconnectEvent(const char * payload, size_t length);
void handleAcceptEvent(const char * payload, size_t length);
void handleCommandEvent(const char * payload, size_t length);


class WebsocketController : public SocketIoClient {
  public:
    bool connected;
    WebsocketController()
      :connected(false)
    {

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
  SocketIoClient::loop();
  // static unsigned long last = millis();
  // if (millis() - last > 5000) {
  //   last = millis();
  // }
}

void handleConnectEvent(const char * payload, size_t length) {
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