
#pragma once
#ifndef SMART_GARDEN_COMMAND_HANDLER_H
#define SMART_GARDEN_COMMAND_HANDLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"
#include "../variables/state.hpp"

DynamicJsonDocument doc(1024);

void handleCommandEvent(const char * payload, size_t length) {
  // prf("got command: %s\n", payload);
  deserializeJson(doc, payload);
  state.pump = doc["pump"];
  state.led = doc["led"];
  state.fan = doc["fan"];
  state.misting = doc["fan"];
  state.nutri = doc["nutri"];
  prl(state.toJSON());
  relayCtl.executeCommand();
}

#endif
