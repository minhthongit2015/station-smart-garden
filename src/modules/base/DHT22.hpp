#pragma once
#ifndef BEYOND_GARDEN_DHT22_H
#define BEYOND_GARDEN_DHT22_H

#include "../../utils/Utils.hpp"
#include "./_BaseModule.hpp"
#include <DHT.h>

#define HUTEMP_SENSOR "HuTemp Sensor"

struct HuTempDHT22 : BaseModule {
  DHT *pDHT = NULL;

  void setup(uint8_t pin, uint8_t type = DHT22) {
    logStart(HUTEMP_SENSOR);
    if (isAPin(pin)) {
      logf(HUTEMP_SENSOR, "Setup on pin %d - Type: %d\r\n", pin, type);
      pDHT = new DHT(pin, type);
      pDHT->begin();
    }
  }
  bool isReady() override {
    return pDHT;
  }
  void fetch(EventData &newData) override {
    newData.HuTemp.temperature = pDHT->readTemperature();
    newData.HuTemp.humidity = pDHT->readHumidity();
    // prf("> temperature: %.2f / humidity: %.2f (prevTemp: %.2f, prevHumi: %.2f)\r\n",
    //   newData.HuTemp.temperature, newData.HuTemp.humidity,
    //   prevData.HuTemp.temperature, prevData.HuTemp.humidity);
  };
  bool validate(EventData &newData) override {
    return !!newData.HuTemp;
  }
  bool hasChange() override {
    // if (data.HuTemp != prevData) {
    //   prf("> HuTemp change humi: %d / temp: %d\r\n",
    //     isEqual(data.HuTemp.humidity, prevData.HuTemp.humidity),
    //     isEqual(data.HuTemp.temperature, prevData.HuTemp.temperature));
    // }
    return data.HuTemp != prevData;
  }
};

#endif
