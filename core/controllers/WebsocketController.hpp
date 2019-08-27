
#pragma once
#ifndef SMART_GARDEN_WEBSOCKET_CONTROLLER_H
#define SMART_GARDEN_WEBSOCKET_CONTROLLER_H

#include <SocketIoClient.h>
#include "../base/utils.hpp"
#include "../variables/global.hpp"


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
  // on("command", handleCommandEvent);
  prf("> [Websocket] Connect to -~=> %s : %d\r\n",
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
  websocketCtl.connected = true;
  websocketCtl.emit("POST/station", DEVICE_INFO);
}

void handleDisconnectEvent(const char * payload, size_t length) {
  if (websocketCtl.connected) {
    logz("Websocket", "Disconnected from Garden!");
    websocketCtl.connected = false;
  }
}

void handleAcceptEvent(const char * payload, size_t length) {
  logz("Websocket", "Garden accepted!");
  websocketCtl.emit("POST/station/state", Global::state.toJSON());
}

void handleCommandEvent(const char * payload, size_t length) {
  // prf("got command: %s\n", payload);
  deserializeJson(Global::state.doc, payload);
  Global::state.pump = Global::state.doc["pump"];
  Global::state.led = Global::state.doc["led"];
  Global::state.fan = Global::state.doc["fan"];
  Global::state.misting = Global::state.doc["fan"];
  Global::state.nutri = Global::state.doc["nutri"];
  prl(Global::state.toJSON());
  relayCtl.executeCommand();
}

#endif