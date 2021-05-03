
#pragma once
#ifndef BEYOND_GARDEN_RELAY_CONTROLLER_H
#define BEYOND_GARDEN_RELAY_CONTROLLER_H

#include "../utils/Utils.hpp"
#include "../models/StationState.hpp"
#include "../helpers/ConfigHelper.hpp"

#define RELAY_CONTROLLER "Relay Controller"

const struct {
  const char* PUMP;
  const char* OXYGEN;
  const char* LED;
  const char* FAN;
  const char* MISTING;
  const char* NUTRIENT;
} RelayPin = {
  "PIN_PUMP",
  "PIN_OXYGEN",
  "PIN_LED",
  "PIN_FAN",
  "PIN_MISTING",
  "PIN_NUTRIENT"
};


struct RelayController {
  void setup() {
    logStart(RELAY_CONTROLLER);
    logf(RELAY_CONTROLLER, "> constant \"NOT_A_PIN\" is defined as \"%d\"\r\n", NOT_A_PIN);
    configRelay(RelayPin.PUMP, D4);
    configRelay(RelayPin.OXYGEN, NOT_A_PIN);
    configRelay(RelayPin.LED, D3);
    configRelay(RelayPin.FAN, NOT_A_PIN);
    configRelay(RelayPin.MISTING, NOT_A_PIN);
    configRelay(RelayPin.NUTRIENT, NOT_A_PIN);
    syncState();
  }

  void configRelay(const char *pinKey, int pinNumber) {
    cfg.setDefault(pinKey, pinNumber);
    useOutOff(cfg.getPin(pinKey));
  }

  void loop() {
    // TODO: Check and stop nutri
  }
  
  void syncState() {
    if (state.doc.containsKey("pump")) {
      setRelay(cfg.getPin(RelayPin.PUMP), state.pump);
    }
    if (state.doc.containsKey("oxygen")) {
      setRelay(cfg.getPin(RelayPin.OXYGEN), state.oxygen);
    }
    if (state.doc.containsKey("led")) {
      setRelay(cfg.getPin(RelayPin.LED), state.led);
    }
    if (state.doc.containsKey("fan")) {
      setRelay(cfg.getPin(RelayPin.FAN), state.fan);
    }
    if (state.doc.containsKey("misting")) {
      setRelay(cfg.getPin(RelayPin.MISTING), state.misting);
    }
    if (state.doc.containsKey("nutri")) {
      setRelay(cfg.getPin(RelayPin.NUTRIENT), state.nutri);
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