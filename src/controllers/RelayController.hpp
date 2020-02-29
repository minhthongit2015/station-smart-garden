
#pragma once
#ifndef BEYOND_GARDEN_RELAY_CONTROLLER_H
#define BEYOND_GARDEN_RELAY_CONTROLLER_H

#include "../utils/Utils.hpp"
#include "../models/StationState.hpp"
#include "../helpers/ConfigHelper.hpp"

#define PIN_PUMP "PIN_PUMP"
#define PIN_OXYGEN "PIN_OXYGEN"
#define PIN_LED "PIN_LED"
#define PIN_FAN "PIN_FAN"
#define PIN_MISTING "PIN_MISTING"
#define PIN_NUTRIENT "PIN_NUTRIENT"


class RelayController {
  private:
  public:
    void setRelay(uint8_t pin, bool isEnable);
    void syncState();

    void setup();
    void loop();
} relays;

extern RelayController relays;

void RelayController::setup() {
  logStart("Relay Controller");
  cfg.setDefault(PIN_PUMP, D3);
  cfg.setDefault(PIN_OXYGEN, NOT_A_PIN);
  cfg.setDefault(PIN_LED, D4);
  cfg.setDefault(PIN_FAN, NOT_A_PIN);
  cfg.setDefault(PIN_MISTING, NOT_A_PIN);
  cfg.setDefault(PIN_NUTRIENT, NOT_A_PIN);
  useOutOff(cfg.getUInt8(PIN_PUMP));
  useOutOff(cfg.getUInt8(PIN_OXYGEN));
  useOutOff(cfg.getUInt8(PIN_LED));
  useOutOff(cfg.getUInt8(PIN_FAN));
  useOutOff(cfg.getUInt8(PIN_MISTING));
  useOutOff(cfg.getUInt8(PIN_NUTRIENT));
  syncState();
}

void RelayController::loop() {
  // TODO: Check and stop nutri
}

void RelayController::syncState() {
  if (state.doc.containsKey("pump")) {
    this->setRelay(cfg.getUInt8(PIN_PUMP), state.pump);
  }
  if (state.doc.containsKey("oxygen")) {
    this->setRelay(cfg.getUInt8(PIN_OXYGEN), state.oxygen);
  }
  if (state.doc.containsKey("led")) {
    this->setRelay(cfg.getUInt8(PIN_LED), state.led);
  }
  if (state.doc.containsKey("fan")) {
    this->setRelay(cfg.getUInt8(PIN_FAN), state.fan);
  }
  if (state.doc.containsKey("misting")) {
    this->setRelay(cfg.getUInt8(PIN_MISTING), state.misting);
  }
  if (state.doc.containsKey("nutri")) {
    this->setRelay(cfg.getUInt8(PIN_NUTRIENT), state.nutri);
  }
}

void RelayController::setRelay(uint8_t pin, bool isEnable) {
  if (notAPin(pin)) return;
  if (isEnable) {
    onRelay(pin);
  } else {
    offRelay(pin);
  }
}

#endif