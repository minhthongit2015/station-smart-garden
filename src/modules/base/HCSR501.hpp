

#pragma once
#ifndef BEYOND_GARDEN_HCSR501_H
#define BEYOND_GARDEN_HCSR501_H

#include "../../utils/Utils.hpp"
#include "./_BaseModule.hpp"
#include "../../utils/Console.hpp"


struct MotionDetectorHCSR501 : BaseModule {
  Event afterMovingEvent;
  timestamp_t delayUntilAfterMoving = 6000;
  uint8_t pin;

  MotionDetectorHCSR501() : BaseModule() {
    checkInterval = 0;
    defineEvent(AFTER_MOVING);
  }

  void onAfterMoving(EventListener callback, unsigned long timeout = 0) {
    onEvent(AFTER_MOVING, callback);
  }
  void dispatchAfterMovingEvent() {
    afterMovingEvent.type = AFTER_MOVING;
    afterMovingEvent.data = data;
    dispatch(&afterMovingEvent);
  }

  void setup(uint8_t pin) {
    logStart("Motion Detector (HC-SR501)");
    this->pin = pin;
    if (!isAPin(pin)) {
      useIn(pin);
    }
  }
  bool isReady() override {
    return isAPin(this->pin);
  }
  void fetch(EventData &newData) override {
    if (notAPin(pin)) return;
    newData.Moving.moving = digitalRead(pin);
  }
  bool validate(EventData &newData) override {
    return !!newData.Moving;
  }
  bool hasChange() override {
    if (Console::timeOver(MODULE_HRSR501_AFTER_MOVING, delayUntilAfterMoving)) {
      Console::timeEnd(MODULE_HRSR501_AFTER_MOVING);
      dispatchAfterMovingEvent();
    }
    return data.Moving != prevData;
  };
  void afterChange() override {
    Console::time(MODULE_HRSR501_AFTER_MOVING, "After Moving");
  }
};

#endif
