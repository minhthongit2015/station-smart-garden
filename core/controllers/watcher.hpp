
#pragma once
#ifndef SMART_GARDEN_EMULATOR_H
#define SMART_GARDEN_EMULATOR_H

#include "../base/utils.hpp"
#include "../variables/state.hpp"
#include "../variables/global.hpp"
#include "../controllers/ScreenController.hpp"


void onHuTempChange(float temperature, float humidity);
void onLightChange(uint16_t light);
void onMovingChange(bool moving);


class StationEmulator {
  private:
  public:
    bool dirty = false;

    void emulateState() {
      state.temperature = random(2500, 3500) / 100.0f;
      state.humidity = random(600, 900) / 10.0f;
      state.light = random(300, 1500);
    }

    void setup() {
      randomSeed(analogRead(0));
      dht.setup();
      dht.onChange(onHuTempChange);
      bh1750.setup();
      bh1750.onChange(onLightChange);
      hcsr501.setup();
      hcsr501.onChange(onMovingChange);
    }

    void loop() {
      static unsigned long last = millis();
      static unsigned long dif = 0;

      dirty = false;

      dht.loop();
      bh1750.loop();
      hcsr501.loop();
      // if (millis() - last > dif) {
      //   last = millis();
      //   dif = random(10000, 15000);
      //   this->emulateState();
      //   webSocket.emit("environment", state.toJSON());
      // }
      
      if (dirty && wsConnected) {
        webSocket.emit("environment", state.toJSON());
      }
    }
} watcher;

void onHuTempChange(float temperature, float humidity) {
  prf("Temp: %.2f | Humi: %.2f\r\n", temperature, humidity);
  state.temperature = temperature;
  state.humidity = humidity;
  watcher.dirty = true;
}

void onLightChange(uint16_t light) {
  prf("Light: %d\r\n", light);
  state.light = light;
  watcher.dirty = true;
}

void onMovingChange(bool moving) {
  prf("Moving: %s\r\n", moving ? "true" : "false");
  state.moving = moving;
  watcher.dirty = true;
}

#endif