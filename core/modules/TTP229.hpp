
#pragma once
#ifndef BEYOND_GARDEN_TTP229_H
#define BEYOND_GARDEN_TTP229_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include "../variables/Configuration.hpp"
#include <TTP229.h>
#include <set>

class TouchPadTTP229 : public BaseModule {
  private:
    void getKey();
    void readKey();
    void resolveKey(Data &key);
    bool blocking = false;
    bool keys[16] = {0};
    ListenerSet keyDownListeners;
    ListenerSet keyUpListeners;
    ListenerSet keyPressListeners;
  public:
    TTP229 *ttp229;
    Data key = { { 0 } };

    TouchPadTTP229() : BaseModule() {
      listenersMap.insert(ListenerPair(KEY_DOWN, &keyDownListeners));
      listenersMap.insert(ListenerPair(KEY_UP, &keyUpListeners));
      listenersMap.insert(ListenerPair(KEY_PRESS, &keyPressListeners));
      CHECK_INTERVAL = 0;
      prevData = key;
    }

    void onKeyDown(EventListener callback);
    void onKeyUp(EventListener callback);

    void setBlocking(bool isBlock);
    bool isPressed(uint8_t key);
    bool isValidKey(uint8_t key);

    bool fetch();
    void setup();
    void loop();
};

void TouchPadTTP229::setup() {
  logStart("Touch Pad (TTP229)");
  if (cfg.TTP229SclPin == NOT_A_PIN || cfg.TTP229SdoPin == NOT_A_PIN) return;
  ttp229 = new TTP229(cfg.TTP229SclPin, cfg.TTP229SdoPin);
}

void TouchPadTTP229::loop() {
  if (cfg.TTP229SclPin == NOT_A_PIN || cfg.TTP229SdoPin == NOT_A_PIN) return;
  // performance2("Key loop", 1);
  if (!check()) return;
  resolveKey(key);
}

bool TouchPadTTP229::fetch() {
  if (blocking) readKey();
  else getKey();
  return !(!key.Key);
}

void TouchPadTTP229::readKey() {
  uint8_t key = ttp229->ReadKey16(); // Blocking
}

void TouchPadTTP229::getKey() {
  uint8_t key = ttp229->GetKey16(); // Non Blocking
}

bool TouchPadTTP229::isPressed(uint8_t key) {
  return key > 0 ? keys[key - 1] : false;
}
bool TouchPadTTP229::isValidKey(uint8_t key) {
  return key > 0 && key < 16;
}

void TouchPadTTP229::resolveKey(Data &key) {
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


void TouchPadTTP229::onKeyDown(EventListener callback) {
  keyDownListeners.insert(callback);
}

void TouchPadTTP229::onKeyUp(EventListener callback) {
  keyUpListeners.insert(callback);
}

#endif