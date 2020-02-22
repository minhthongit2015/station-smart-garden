

#pragma once
#ifndef BEYOND_GARDEN_HCSR501_H
#define BEYOND_GARDEN_HCSR501_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <set>

#define HCSR501_DELAY 6000

class MotionDetectorHCSR501 : public BaseModule {
  private:
    ListenerSet afterMovingListeners;
  public:
    unsigned long delayTime = 0;
    uint8_t pin;

    MotionDetectorHCSR501() {
      checkInterval = 200;
      listenersMap.insert(ListenerPair(MOVING_CHANGE, &afterMovingListeners));
    }

    void onAfterMoving(EventListener callback, unsigned long timeout = 0) {
      this->afterMovingListeners.insert(callback);
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
