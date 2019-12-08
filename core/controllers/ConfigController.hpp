
#pragma once
#ifndef SMART_GARDEN_CONFIG_CONTROLLER_H
#define SMART_GARDEN_CONFIG_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"

class ConfigController {
  public:
    void setup();
    void loop();
} configCtl;

void ConfigController::setup() {
  logStart("Config Controller");
}

void ConfigController::loop() {
  if (Serial.available()) {
    Global::cfg.putConfigurations(Serial);
  }
}

#endif
