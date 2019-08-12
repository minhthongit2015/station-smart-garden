
#pragma once
#ifndef SMART_GARDEN_GARDEN_CONNECTED_HANDLER_H
#define SMART_GARDEN_GARDEN_CONNECTED_HANDLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"
#include "../variables/state.hpp"


void handleGardenConnectedEvent(const char * payload, size_t length) {
  prl("Garden accepted");
  webSocket.emit("stationState", state.toJSON());
}


#endif
