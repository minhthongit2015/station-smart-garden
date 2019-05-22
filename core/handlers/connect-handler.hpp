
#pragma once
#ifndef SMART_GARDEN_CONNECT_HANDLER_H
#define SMART_GARDEN_CONNECT_HANDLER_H

#include "../base/utils.hpp"


void handleConnectEvent(const char * payload, size_t length) {
  wsConnected = true;
  prl("Connected to Server");
  webSocket.emit("stationConnect", DEVICE_INFO);
}


#endif
