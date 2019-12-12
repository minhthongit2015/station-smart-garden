

#pragma once
#ifndef BEYOND_GARDEN_DHT22_H
#define BEYOND_GARDEN_DHT22_H

#include "../base/utils.hpp"
#include "../variables/Configuration.hpp"
#include "./_BaseModule.hpp"
#include <DHT.h>

class HuTempDHT22 : public BaseModule {
  private:
  public:
    DHT *pDHT = NULL;
    Data data = { { 80, 25 } };

    HuTempDHT22() : BaseModule() {
      CHECK_INTERVAL = 2000;
      prevData = data;
    }

    void setup();
    void loop();
    bool fetch();
};

void HuTempDHT22::setup() {
  logStart("HuTemp Sensor (DHT22)");
  pDHT = new DHT(cfg.DhtPin, cfg.DhtType);
  (*pDHT).begin();
}

void HuTempDHT22::loop() {
  if (!check()) return;
  if (data.HuTemp != prevData) {
    dispatch(data, HUTEMP_CHANGE);
  }
}

bool HuTempDHT22::fetch() {
  static Data newData = { { 0, 0 } };
  newData.HuTemp.temperature = (*pDHT).readTemperature();
  newData.HuTemp.humidity = (*pDHT).readHumidity();
  if (!newData.HuTemp) {
    return false;
  }
  data = newData;
  return true;
}

#endif
