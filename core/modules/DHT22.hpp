

#pragma once
#ifndef SMART_GARDEN_DHT22_H
#define SMART_GARDEN_DHT22_H

#include "../base/utils.hpp"
#include "../variables/config.hpp"
#include <DHT.h>

typedef void (*onHutempChangeListener)(float temperature, float humidity);

class HuTempDHT22 {
  private:
    std::set<onHutempChangeListener> _onChange;
  public:
    DHT dht;
    float prevHumidity = 80;
    float prevTemperature = 25;
    float humidity = 80;
    float temperature = 25;
    unsigned long delayTime = 2000;

    HuTempDHT22()
      :dht(Config::DhtPin, Config::DhtType)
    {

    }

    void setup();
    void loop();

    void onChange(onHutempChangeListener callback) {
      this->_onChange.insert(callback);
    }

    bool read();
    void dispatchEvent(float temperature, float humidity);
};

void HuTempDHT22::setup() {
  logStart("HuTemp Sensor (DHT22)");
  dht.begin();
}

void HuTempDHT22::loop() {
  if (!read()) return;
  if ((prevTemperature != temperature
      || prevHumidity != humidity)
      && _onChange.size() > 0) {
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
	for (std::set<onHutempChangeListener>::iterator p = _onChange.begin(); p != _onChange.end(); ++p) {
    (*p)(temperature, humidity);
  }
}

#endif
