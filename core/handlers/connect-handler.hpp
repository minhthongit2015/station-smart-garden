
#pragma once
#ifndef SMART_GARDEN_CONNECT_HANDLER_H
#define SMART_GARDEN_CONNECT_HANDLER_H

#include <ArduinoJson.h>

void handleConnectEvent(const char * payload, size_t length) {
  Serial.printf("Connected to Websocket Server: %s\n", payload);
}

#endif
