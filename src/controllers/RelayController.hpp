
#pragma once
#ifndef BEYOND_GARDEN_RELAY_CONTROLLER_H
#define BEYOND_GARDEN_RELAY_CONTROLLER_H

#include "../utils/Utils.hpp"
#include "../models/StationState.hpp"
#include "../helpers/ConfigHelper.hpp"

#define RELAY_CONTROLLER "Relay Controller"

#define PIN_PUMP "PIN_PUMP"
#define PIN_OXYGEN "PIN_OXYGEN"
#define PIN_LED "PIN_LED"
#define PIN_FAN "PIN_FAN"
#define PIN_MISTING "PIN_MISTING"
#define PIN_NUTRIENT "PIN_NUTRIENT"


struct RelayController {
  void setup() {
    logStart(RELAY_CONTROLLER);
    logf(RELAY_CONTROLLER, "> constant \"NOT_A_PIN\" is defined as \"%d\"\r\n", NOT_A_PIN);
    cfg.setDefault(PIN_PUMP, D3);
    cfg.setDefault(PIN_OXYGEN, NOT_A_PIN);
    cfg.setDefault(PIN_LED, D4);
    cfg.setDefault(PIN_FAN, NOT_A_PIN);
    cfg.setDefault(PIN_MISTING, NOT_A_PIN);
    cfg.setDefault(PIN_NUTRIENT, NOT_A_PIN);
    useOutOff(cfg.getPin(PIN_PUMP));
    useOutOff(cfg.getPin(PIN_OXYGEN));
    useOutOff(cfg.getPin(PIN_LED));
    useOutOff(cfg.getPin(PIN_FAN));
    useOutOff(cfg.getPin(PIN_MISTING));
    useOutOff(cfg.getPin(PIN_NUTRIENT));
    syncState();
  }
  void loop() {
    // TODO: Check and stop nutri
  }
  
  void syncState() {
    if (state.doc.containsKey("pump")) {
      this->setRelay(cfg.getPin(PIN_PUMP), state.pump);
    }
    if (state.doc.containsKey("oxygen")) {
      this->setRelay(cfg.getPin(PIN_OXYGEN), state.oxygen);
    }
    if (state.doc.containsKey("led")) {
      this->setRelay(cfg.getPin(PIN_LED), state.led);
    }
    if (state.doc.containsKey("fan")) {
      this->setRelay(cfg.getPin(PIN_FAN), state.fan);
    }
    if (state.doc.containsKey("misting")) {
      this->setRelay(cfg.getPin(PIN_MISTING), state.misting);
    }
    if (state.doc.containsKey("nutri")) {
      this->setRelay(cfg.getPin(PIN_NUTRIENT), state.nutri);
    }
  }

  void setRelay(uint8_t pin, bool isEnable) {
    if (notAPin(pin)) return;
    if (isEnable) {
      onRelay(pin);
    } else {
      offRelay(pin);
    }
  }

} relays;

extern RelayController relays;

#endif