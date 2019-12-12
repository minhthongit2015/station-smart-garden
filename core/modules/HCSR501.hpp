

#pragma once
#ifndef BEYOND_GARDEN_HCSR501_H
#define BEYOND_GARDEN_HCSR501_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include "../variables/Configuration.hpp"
#include <set>

#define HCSR501_DELAY 6000

class MotionDetectorHCSR501 : public BaseModule {
  private:
    ListenerSet afterMovingListeners;
  public:
    Data moving = { { false } };
    unsigned long delayTime = 0;

    MotionDetectorHCSR501() {
      CHECK_INTERVAL = 200;
      listenersMap.insert(ListenerPair(MOVING_CHANGE, &afterMovingListeners));
    }

    bool fetch();
    void onAfterMoving(EventListener callback, unsigned long timeout = 0) {
      this->afterMovingListeners.insert(callback);
    }

    void setup();
    void loop();
};

void MotionDetectorHCSR501::setup() {
  logStart("Motion Detector (HC-SR501)");
  if (cfg.HcSr501Pin != NOT_A_PIN) {
    pinMode(cfg.HcSr501Pin, INPUT);
  }
}

void MotionDetectorHCSR501::loop() {
  if (cfg.HcSr501Pin == NOT_A_PIN) return;
  if (!check()) return;
  if (moving.Moving != prevData) {
    dispatch(moving, MOVING_CHANGE);
  }
}

bool MotionDetectorHCSR501::fetch() {
  moving.Moving.moving = digitalRead(cfg.HcSr501Pin);
  // prf("isMove: %s\r\n", newMoving ? "true" : "false");
  return true;
}

#endif
