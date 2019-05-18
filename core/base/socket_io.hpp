
#pragma once
#ifndef SMART_GARDEN_SOCKET_IO_H
#define SMART_GARDEN_SOCKET_IO_H

#include <SocketIoClient.h>
#include "../handlers/connect-handler.hpp"
#include "../handlers/command-handler.hpp"

SocketIoClient webSocket;

void websocketSetup()
{
  // webSocket.on("connect", handleConnectEvent);
  webSocket.on("command", handleCommandEvent);
  webSocket.begin("192.168.1.25", 4000, "/socket.io/?transport=websocket");
  // webSocket.setAuthorization("username", "password");
}

void websocketLoop()
{
  webSocket.loop();
  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    webSocket.emit("message", "{\"hello\": \"world\"}");
  }
}

#endif