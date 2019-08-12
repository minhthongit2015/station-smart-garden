
#pragma once
#ifndef SMART_GARDEN_EMULATOR_H
#define SMART_GARDEN_EMULATOR_H

#include "./base/utils.hpp"
#include "./variables/state.hpp"
#include "./variables/global.hpp"


void onHuTempChange(float temperature, float humidity);


class StationEmulator {
public:
  void emulateState() {
    state.temperature = random(2500, 3500) / 100.0f;
    state.humidity = random(600, 900) / 10.0f;
    state.light = random(300, 1500);
  }

  void setup() {
    randomSeed(analogRead(0));
    dht.setup();
    dht.onChange(onHuTempChange);
  }

  void loop() {
    static unsigned long last = millis();
    static unsigned long dif = 0;
    dht.loop();
    // if (millis() - last > dif) {
    //   last = millis();
    //   dif = random(10000, 15000);
    //   this->emulateState();
    //   webSocket.emit("environment", state.toJSON());
    // }
  }
} watcher;

void onHuTempChange(float temperature, float humidity) {
  static char buffer[100] = {0};
  sprintf(buffer, "Temp: %.2f | Humi: %.2f\r\n", temperature, humidity);
  pr(buffer);
  lcd.lcd.clear();
  lcd.lcd.setCursor(0, 0);
  lcd.lcd.print(buffer);
  state.temperature = temperature;
  state.humidity = humidity;
  if (wsConnected) {
    webSocket.emit("environment", state.toJSON());
  }
}

#endif