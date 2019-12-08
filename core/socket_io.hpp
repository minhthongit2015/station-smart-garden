
#pragma once
#ifndef SMART_GARDEN_SOCKET_IO_H
#define SMART_GARDEN_SOCKET_IO_H

#include "./base/utils.hpp"
#include "./handlers/connect-handler.hpp"
#include "./handlers/disconnect-handler.hpp"
#include "./handlers/command-handler.hpp"
#include "./handlers/garden-connected-handler.hpp"

void websocketSetup()
{
  webSocket.on("connect", handleConnectEvent);
  webSocket.on("disconnect", handleDisconnectEvent);
  webSocket.on("command", handleCommandEvent);
  webSocket.on("stationAccepted", handleGardenConnectedEvent);
  webSocket.begin(WEBSOCKET_SERVER, WEBSOCKET_SERVER_PORT, "/socket.io/?transport=websocket");
  // webSocket.setAuthorization("username", "password");
}

void websocketLoop()
{
  webSocket.loop();
  // static unsigned long last = millis();
  // if (millis() - last > 5000) {
  //   last = millis();
  // }
}

#endif