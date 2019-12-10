

#pragma once
#ifndef SMART_GARDEN_HCSR501_H
#define SMART_GARDEN_HCSR501_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <set>

#define HCSR501_DELAY 6000

class MotionDetectorHCSR501 : public BaseModule {
  private:
    std::set<ChangeListener> afterMovingListeners;
  public:
    Data moving = { { false } };
    Data prevMoving = { { false } };
    uint8_t sensorPin = Config::HcSr501Pin;
    unsigned long delayTime = 0;

    MotionDetectorHCSR501() {
      CHECK_INTERVAL = 200;
    }

    void setup();
    void loop();
    bool fetch();
    void onAfterMoving(ChangeListener callback, unsigned long timeout = 0) {
      this->afterMovingListeners.insert(callback);
    }
};

void MotionDetectorHCSR501::setup() {
  logStart("Motion Detector (HC-SR501)");
  pinMode(sensorPin, INPUT);
}

void MotionDetectorHCSR501::loop() {
  if (sensorPin == NOT_A_PIN) return;
  if (!check()) return;
  if (moving.Moving != prevMoving) {
    dispatch(moving, VALUE_CHANGE);
    prevMoving = moving;
  }
}

bool MotionDetectorHCSR501::fetch() {
  moving.Moving.moving = digitalRead(sensorPin);
  // prf("isMove: %s\r\n", newMoving ? "true" : "false");
  return true;
}

#endif
