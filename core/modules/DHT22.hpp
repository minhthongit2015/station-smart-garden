

#pragma once
#ifndef SMART_GARDEN_DHT22_H
#define SMART_GARDEN_DHT22_H

#include "../base/utils.hpp"
#include <DHT.h>

#define DHTPIN 12 // D6
#define DHTTYPE DHT22

class HuTempDHT22 {
  private:
  public:
    DHT dht;
    float prevHumidity = 80;
    float prevTemperature = 25;
    float humidity = 80;
    float temperature = 25;
    unsigned long delayTime = 2000;

    HuTempDHT22()
      :dht(DHTPIN, DHTTYPE)
    {

    }

    void setup();
    void loop();

    std::function<void (float temperature, float humidity)> _onChange = NULL;
    void onChange(std::function<void (float temperature, float humidity)> __onChange) {
      this->_onChange = __onChange;
    }

    bool read();
    void dispatchEvent(float temperature, float humidity);
};

void HuTempDHT22::setup() {
  dht.begin();
}

void HuTempDHT22::loop() {
  if (!read()) return;
  if ((prevTemperature != temperature
      || prevHumidity != humidity)
      && _onChange) {
    dispatchEvent(temperature, humidity);
  }
  prevTemperature = temperature;
  prevHumidity = humidity;
}

bool HuTempDHT22::read() {
  static unsigned long now = 0;
  static unsigned long last = 0;
  static unsigned long dif = 0;
  static float newTemperature = 0;
  static float newHumidity = 0;
  now = millis();
  dif = now - last;
  
  if (dif >= delayTime) {
    newTemperature = dht.readTemperature();
    newHumidity = dht.readHumidity();
    if (isnan(newTemperature) || isnan(newHumidity)) {
      return false;
    }
    temperature = newTemperature;
    humidity = newHumidity;
    last = now;
    return true;
  }

  return false;
}

void HuTempDHT22::dispatchEvent(float temperature, float humidity) {
  _onChange(temperature, humidity);
}

#endif
