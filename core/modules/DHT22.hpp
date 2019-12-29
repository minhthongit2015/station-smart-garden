

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
    int precision = 100;
    EventType getDefaultEventType() override {
      return HUTEMP_CHANGE;
    }
    DHT *pDHT = NULL;
    Data data = { { 80, 25 } };

    HuTempDHT22() : BaseModule(&data) {
      CHECK_INTERVAL = 2000;
      listenersMap.insert(ListenerPair(getDefaultEventType(), &changeListeners));
    }

    void setup() override;
    void loop() override;
    bool fetch() override;
};

void HuTempDHT22::setup() {
  logStart("HuTemp Sensor (DHT22)");
  pDHT = new DHT(cfg.DhtPin, cfg.DhtType);
  (*pDHT).begin();
}

void HuTempDHT22::loop() {
  if (!check()) return;
  if (data.HuTemp != prevData) {
    dispatch(data);
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
