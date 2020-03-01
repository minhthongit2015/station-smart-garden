

#pragma once
#ifndef BEYOND_GARDEN_HCSR501_H
#define BEYOND_GARDEN_HCSR501_H

#include "../../utils/Utils.hpp"
#include "./_BaseModule.hpp"


#define HCSR501_DELAY 6000

struct MotionDetectorHCSR501 : BaseModule {
  unsigned long delayTime = 0;
  uint8_t pin;

  MotionDetectorHCSR501() {
    checkInterval = 200;
  }

  void onAfterMoving(EventListener callback, unsigned long timeout = 0) {
    onEvent(callback);
  }

  void setup(uint8_t pin) {
    logStart("Motion Detector (HC-SR501)");
    this->pin = pin;
    if (!isAPin(pin)) {
      useIn(pin);
    }
  }
  void fetch(EventData &newData) override {
    if (notAPin(pin)) return;
    newData.Moving.moving = digitalRead(pin);
  }
  bool validate(EventData &newData) override {
    return !!newData.Moving;
  }
  bool hasChange() override {
    return data.Moving != prevData;
  };
};

#endif
