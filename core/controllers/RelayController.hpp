
#pragma once
#ifndef BEYOND_GARDEN_RELAY_CONTROLLER_H
#define BEYOND_GARDEN_RELAY_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/State.hpp"
#include "../variables/Configuration.hpp"

class RelayController {
  private:
  public:
    void setupPin(uint8_t pin);
    void setRelay(uint8_t pin, bool isEnable);
    void syncState();

    void setup();
    void loop();
} relays;

extern RelayController relays;

void RelayController::setup() {
  logStart("Relay Controller");
  setupPin(cfg.pumpPin);
  setupPin(cfg.oxygenPin);
  setupPin(cfg.ledPin);
  setupPin(cfg.fanPin);
  setupPin(cfg.mistingPin);
  setupPin(cfg.nutriPin);
  syncState();
}

void RelayController::loop() {
  // TODO: Check and stop nutri
}

void RelayController::setupPin(uint8_t pin) {
  if (pin != NOT_A_PIN) {
    useOutOff(pin);
  }
}

void RelayController::syncState() {
  if (state.doc.containsKey("pump")) {
    this->setRelay(cfg.pumpPin, state.pump);
  }
  if (state.doc.containsKey("oxygen")) {
    this->setRelay(cfg.oxygenPin, state.oxygen);
  }
  if (state.doc.containsKey("led")) {
    this->setRelay(cfg.ledPin, state.led);
  }
  if (state.doc.containsKey("fan")) {
    this->setRelay(cfg.fanPin, state.fan);
  }
  if (state.doc.containsKey("misting")) {
    this->setRelay(cfg.mistingPin, state.misting);
  }
  if (state.doc.containsKey("nutri")) {
    this->setRelay(cfg.nutriPin, state.nutri);
  }
}

void RelayController::setRelay(uint8_t pin, bool isEnable) {
  if (pin == NOT_A_PIN) return;
  if (isEnable) {
    onRelay(pin);
  } else {
    offRelay(pin);
  }
}

#endif