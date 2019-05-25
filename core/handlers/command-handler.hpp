
#pragma once
#ifndef SMART_GARDEN_COMMAND_HANDLER_H
#define SMART_GARDEN_COMMAND_HANDLER_H

#include "../base/utils.hpp"
#include "../stationstate.hpp"
#include "../modules/Controller.hpp"

DynamicJsonDocument doc(1024);

void handleCommandEvent(const char * payload, size_t length) {
  // prf("got command: %s\n", payload);
  deserializeJson(doc, payload);
  state.pump = doc["pump"];
  state.led = doc["led"];
  state.fan = doc["fan"];
  state.misting = doc["fan"];
  state.nutri = doc["nutri"];
  controller.executeCommand();
  prl(payload);
}

#endif
