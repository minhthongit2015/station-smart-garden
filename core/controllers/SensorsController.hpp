
#pragma once
#ifndef SMART_GARDEN_SENSORS_CONTROLLER_H
#define SMART_GARDEN_SENSORS_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"

#include "./DisplayController.hpp"
#include "./WebsocketController.hpp"


void onHuTempChange(float temperature, float humidity);
void onLightChange(uint16_t light);
void onMovingChange(bool moving);

class SensorsController {
  private:
  public:
    bool dirty = false;

    void emulateState() {
      Global::state.temperature = random(2500, 3500) / 100.0f;
      Global::state.humidity = random(600, 900) / 10.0f;
      Global::state.light = random(300, 1500);
    }

    void setup() {
      logStart("Sensors Controller");
      randomSeed(analogRead(0));
      Global::dht.setup();
      Global::dht.onChange(onHuTempChange);
      Global::bh1750.setup();
      Global::bh1750.onChange(onLightChange);
      Global::hcsr501.setup();
      Global::hcsr501.onChange(onMovingChange);
    }

    void loop() {
      static unsigned long last = millis();
      static unsigned long dif = 0;

      Global::dht.loop();
      Global::bh1750.loop();
      Global::hcsr501.loop();
      // if (millis() - last > dif) {
      //   last = millis();
      //   dif = random(10000, 15000);
      //   this->emulateState();
      //   webSocket.emit("environment", state.toJSON());
      // }
      
      if (dirty && websocketCtl.connected) {
        websocketCtl.emit("environment", Global::state.toJSON());
        dirty = false;
      }
    }
} sensorsCtl;

void onHuTempChange(float temperature, float humidity) {
  if (logChannels[1]) prf("Temp: %.2f | Humi: %.2f\r\n", temperature, humidity);
  Global::state.temperature = temperature;
  Global::state.humidity = humidity;
  sensorsCtl.dirty = true;
}

void onLightChange(uint16_t light) {
  if (logChannels[1]) prf("Light: %d\r\n", light);
  Global::state.light = light;
  sensorsCtl.dirty = true;
}

void onMovingChange(bool moving) {
  if (logChannels[1]) prf("Moving: %s\r\n", moving ? "true" : "false");
  Global::state.moving = moving;
  sensorsCtl.dirty = true;
}

#endif