
#pragma once
#ifndef SMART_GARDEN_DISCONNECT_HANDLER_H
#define SMART_GARDEN_DISCONNECT_HANDLER_H

#include "../base/utils.hpp"


void handleDisconnectEvent(const char * payload, size_t length) {
  wsConnected = false;
  prl("Disconnected from Server!");
}

#endif
