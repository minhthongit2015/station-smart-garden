

#pragma once
#ifndef SMART_GARDEN_HCSR501_H
#define SMART_GARDEN_HCSR501_H

#include "../base/utils.hpp"
#include <set>

typedef void (*onMovementListener)(bool moving);

#define HCSR501_DELAY 6000

class MotionDetectorHCSR501 {
  private:
    std::set<onMovementListener> _onChange;
    std::set<onMovementListener> _onAfterMoving;
  public:
    bool prevMoving = false;
    bool moving = false;
    uint8_t sensorPin = Config::HcSr501Pin;
    unsigned long delayTime = 0;

    void setup();
    void loop();

    void onChange(onMovementListener callback) {
      this->_onChange.insert(callback);
    }
    void onAfterMoving(onMovementListener callback, unsigned long timeout = 0) {
      this->_onAfterMoving.insert(callback);
    }

    bool read();
    void dispatchEvent(bool moving);
};

void MotionDetectorHCSR501::setup() {
  logStart("Motion Detector (HC-SR501)");
}

void MotionDetectorHCSR501::loop() {
  if (!read()) return;
  if ((prevMoving != moving)
      && _onChange.size() > 0) {
    dispatchEvent(moving);
  }
  prevMoving = moving;
}

bool MotionDetectorHCSR501::read() {
  static unsigned long now = 0;
  static unsigned long last = 0;
  static unsigned long dif = 0;
  now = millis();
  dif = now - last;

  static bool newMoving = false;
  newMoving = digitalRead(sensorPin);
  // prf("isMove: %s\r\n", newMoving ? "true" : "false");

  if (dif >= delayTime) {
    moving = newMoving;

    if (newMoving) {
      delayTime = HCSR501_DELAY;
      last = now;
    }
    return true;
  } else {
    if (newMoving) {
      last = now;
    }
  }
  

  return false;
}

void MotionDetectorHCSR501::dispatchEvent(bool moving) {
	for (std::set<onMovementListener>::iterator p = _onChange.begin(); p != _onChange.end(); ++p) {
    (*p)(moving);
  }
}

#endif
