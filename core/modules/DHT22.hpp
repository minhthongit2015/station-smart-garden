

#pragma once
#ifndef SMART_GARDEN_DHT22_H
#define SMART_GARDEN_DHT22_H

#include "../base/utils.hpp"
#include "../variables/config.hpp"
#include "./_BaseModule.hpp"
#include <DHT.h>

class HuTempDHT22 : public BaseModule {
  private:
  public:
    DHT dht;
    Data prevData = { { 80, 25 } };
    Data data = { { 80, 25 } };

    HuTempDHT22()
      :dht(Config::DhtPin, Config::DhtType)
    {
      CHECK_INTERVAL = 2000;
    }

    void setup();
    void loop();
    bool fetch();
};

void HuTempDHT22::setup() {
  logStart("HuTemp Sensor (DHT22)");
  dht.begin();
}

void HuTempDHT22::loop() {
  if (!check()) {
    // prl("read nothing...");
    return;
  }
  if (data.HuTemp != prevData) {
    dispatch(data, VALUE_CHANGE);
    prevData = data;
  }
}

bool HuTempDHT22::fetch() {
  static Data newData = { { 0, 0 } };
  newData.HuTemp.temperature = dht.readTemperature();
  newData.HuTemp.humidity = dht.readHumidity();
  if (!newData.HuTemp) {
    return false;
  }
  data = newData;
  return true;
}

#endif
