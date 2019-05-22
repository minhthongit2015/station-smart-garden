
#pragma once
#ifndef SMART_GARDEN_EMULATOR_H
#define SMART_GARDEN_EMULATOR_H

#include "./base/utils.hpp"
#include "./stationstate.hpp"

class StationEmulator {

public:
  void emulateState() {
    state.temperature = random(2500, 3500) / 100.0f;
    state.humidity = random(600, 900) / 10.0f;
    state.light = random(300, 1500);
  }

  void setup() {
    randomSeed(analogRead(0));
  }

  void loop() {
    static unsigned long last = millis();
    static unsigned long dif = 0;
    if (millis() - last > dif) {
      last = millis();
      dif = random(10000, 15000);
      this->emulateState();
      webSocket.emit("environment", state.toJSON());
    }
  }
} emulator;

#endif