
#pragma once
#ifndef SMART_GARDEN_COMMAND_HANDLER_H
#define SMART_GARDEN_COMMAND_HANDLER_H

#include "../base/utils.hpp"
#include <ArduinoJson.h>

void handleCommandEvent(const char * payload, size_t length) {
  prf("got command: %s\n", payload);
}

#endif
