#pragma once
#ifndef BEYOND_GARDEN_HD38_H
#define BEYOND_GARDEN_HD38_H

#include "../../utils/Utils.hpp"
#include "./_BaseModule.hpp"
#include <DHT.h>

#define HD38_SENSOR "Soil Moisture Sensor"

struct HD38 : BaseModule {
  uint8_t pin;
  
  void setup(uint8_t pin) {
    logStart(HD38_SENSOR);
    if (isAPin(pin)) {
      this->pin = pin;
      logf(HD38_SENSOR, "Setup on pin %d\r\n", pin);
    }
  }

  void fetch(EventData &newData) override {
    int analog = analogRead(pin);
    float percent = (1.0f - (analog / 1023.0f)) * 100.0f;
    newData.SoilMoister = percent;
  };
  bool validate(EventData &newData) override {
    return !!newData.SoilMoister;
  }
  bool hasChange() override {
    return data.SoilMoister != prevData;
  }
};

#endif
