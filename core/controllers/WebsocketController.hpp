
#pragma once
#ifndef SMART_GARDEN_WEBSOCKET_CONTROLLER_H
#define SMART_GARDEN_WEBSOCKET_CONTROLLER_H

#include <SocketIoClient.h>
#include "../base/utils.hpp"
#include "../variables/global.hpp"


DynamicJsonDocument doc(1024);
void handleConnectEvent(const char * payload, size_t length);
void handleDisconnectEvent(const char * payload, size_t length);
void handleAcceptedEvent(const char * payload, size_t length);
void handleCommandEvent(const char * payload, size_t length);


class WebsocketController : public SocketIoClient {
  public:
    bool connected = false;

    void setup() {
      on("connect", handleConnectEvent);
      on("disconnect", handleDisconnectEvent);
      on("accepted", handleAcceptedEvent);
      on("command", handleCommandEvent);
      // webSocket.begin(Config::gardenHost.c_str(), Config::gardenPort);
      // webSocket.setAuthorization("username", "password");
    }

    void loop() {
      SocketIoClient::loop();
      // static unsigned long last = millis();
      // if (millis() - last > 5000) {
      //   last = millis();
      // }
    }
} websocketCtl;

void handleConnectEvent(const char * payload, size_t length) {
  websocketCtl.connected = true;
  prl("Connected to Server");
  websocketCtl.emit("stationConnect", DEVICE_INFO);
}

void handleDisconnectEvent(const char * payload, size_t length) {
  websocketCtl.connected = false;
  prl("Disconnected from Server!");
}

void handleAcceptedEvent(const char * payload, size_t length) {
  prl("Garden accepted");
  websocketCtl.emit("stationState", Global::state.toJSON());
}

void handleCommandEvent(const char * payload, size_t length) {
  // prf("got command: %s\n", payload);
  deserializeJson(doc, payload);
  Global::state.pump = doc["pump"];
  Global::state.led = doc["led"];
  Global::state.fan = doc["fan"];
  Global::state.misting = doc["fan"];
  Global::state.nutri = doc["nutri"];
  prl(Global::state.toJSON());
  relayCtl.executeCommand();
}

#endif