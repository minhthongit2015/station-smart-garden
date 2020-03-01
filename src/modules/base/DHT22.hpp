#pragma once
#ifndef BEYOND_GARDEN_DHT22_H
#define BEYOND_GARDEN_DHT22_H

#include "../../utils/Utils.hpp"
#include "./_BaseModule.hpp"
#include <DHT.h>


struct HuTempDHT22 : BaseModule {
  DHT *pDHT = NULL;

  void setup(uint8_t pin, uint8_t type = DHT22) {
    logStart("HuTemp Sensor (DHT22)");
    if (isAPin(pin)) {
      pDHT = new DHT(pin, type);
      pDHT->begin();
    }
  }
  void fetch(EventData &newData) override {
    if (!pDHT) return;
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
