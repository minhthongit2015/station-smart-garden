
#pragma once
#ifndef BEYOND_GARDEN_TTP229_H
#define BEYOND_GARDEN_TTP229_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <TTP229.h>
#include <set>

class TouchPadTTP229 : public BaseModule {
  private:
    uint8_t getKey();
    uint8_t readKey();
    void resolveKey(EventData &key);
    bool blocking = false;
    bool keys[16] = {0};
    ListenerSet keyDownListeners;
    ListenerSet keyUpListeners;
    ListenerSet keyPressListeners;
  public:
    TTP229 *ttp229;

    TouchPadTTP229() : BaseModule() {
      listenersMap.insert(ListenerPair(KEY_DOWN, &keyDownListeners));
      listenersMap.insert(ListenerPair(KEY_UP, &keyUpListeners));
      listenersMap.insert(ListenerPair(KEY_PRESS, &keyPressListeners));
      checkInterval = 0;
    }

    void onKeyDown(EventListener callback) {
      keyDownListeners.insert(callback);
    }
    void onKeyUp(EventListener callback) {
      keyUpListeners.insert(callback);
    }

    void setBlocking(bool isBlock);
    bool isPressed(uint8_t key);
    bool isValidKey(uint8_t key);

    void setup(uint8_t sclPin, uint8_t sdoPin) {
      logStart("Touch Pad (TTP229)");
      if (isAPin(sclPin) && isAPin(sdoPin)) {
        ttp229 = new TTP229(sclPin, sdoPin);
      }
    }
    void fetch(EventData &newData) override {
      if (blocking) newData.Key.key = readKey();
      else newData.Key.key = getKey();
    }
    bool validate(EventData &newData) override {
      return !!newData.Key;
    }
    bool hasChange() override {
      resolveKey(data);
      return false;
    }
};

uint8_t TouchPadTTP229::readKey() {
  return ttp229->ReadKey16(); // Blocking
}

uint8_t TouchPadTTP229::getKey() {
  return ttp229->GetKey16(); // Non Blocking
}

bool TouchPadTTP229::isPressed(uint8_t key) {
  return key > 0 ? keys[key - 1] : false;
}

void TouchPadTTP229::resolveKey(EventData &key) {
  static unsigned long last = 0;
  unsigned long dif = 0;
  static uint8_t keyz = 0, prevKeyz = 0;
  dif = millis() - last;

  if (key.Key != prevData) {
    keyz = key.Key.key;
    prevKeyz = prevData.Key.key;
    prf("Key pressed: %d\r\n", keyz);
    if (!!prevData.Key) keys[prevKeyz - 1] = false;
    if (!!key.Key) keys[keyz - 1] = true;
    if (!!prevData.Key) dispatch(prevData, KEY_UP);
    if (!!key.Key) dispatch(key, KEY_DOWN);
    prevData = key;
  } else {
    last = millis();
  }
}

#endif