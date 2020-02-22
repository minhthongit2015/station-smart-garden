#pragma once
#ifndef BEYOND_GARDEN_DHT22_H
#define BEYOND_GARDEN_DHT22_H

#include <DHT.h>
#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include "../base/types.hpp"

class HuTempDHT22 : public BaseModule {
  private:
  public:
    DHT *pDHT = NULL;
    void setup(uint8_t pin, uint8_t type) {
      logStart("HuTemp Sensor (DHT22)");
      pDHT = new DHT(pin, type);
      pDHT->begin();
    }
    void fetch(EventData &newData) override {
      newData.HuTemp.temperature = pDHT->readTemperature();
      newData.HuTemp.humidity = pDHT->readHumidity();
    };
    bool validate(EventData &newData) override {
      return !!newData.HuTemp;
    }
    bool hasChange() override {
      return newData.HuTemp != prevData;
    }
};

#endif
