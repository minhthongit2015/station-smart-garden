
#pragma once
#ifndef SMART_GARDEN_TTP229_H
#define SMART_GARDEN_TTP229_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <TTP229.h>
#include <set>

class TouchPadTTP229 : public BaseModule {
  private:
    void getKey();
    void readKey();
    void resolveKey(uint8_t key);
    bool blocking = false;
    unsigned long delay = 1000;
    unsigned long freqTime = 200;

    std::set<KeyListener> keyDownListeners;
    std::set<KeyListener> keyUpListeners;
    std::set<KeyListener> keyPressListeners;
    bool keys[16] = {0};
  public:
    TTP229 *ttp229;

    TouchPadTTP229() : BaseModule() {
      listenersMap.insert(ListenerPair(KEY_DOWN, &keyDownListeners));
      listenersMap.insert(ListenerPair(KEY_DOWN, &keyUpListeners));
      listenersMap.insert(ListenerPair(KEY_DOWN, &keyPressListeners));
    }

    void onKeyDown(KeyListener callback);
    void onKeyUp(KeyListener callback);
    void dispatch(uint8_t key, EventType type);

    void setup();
    void loop();

    void setDelayTime(unsigned long delay); // Delay before change to key press event
    void setKeyPressFreq(unsigned long freq); // Time between two key press event
    void setBlocking(bool isBlock);
    bool isPressed(uint8_t key);
};

void TouchPadTTP229::setup()
{
  logStart("Touch Pad (TTP229)");
  if (Config::TTP229SclPin == NOT_A_PIN || Config::TTP229SdoPin == NOT_A_PIN) return;
  ttp229 = new TTP229(Config::TTP229SclPin, Config::TTP229SdoPin);
}

void TouchPadTTP229::loop()
{
  if (Config::TTP229SclPin == NOT_A_PIN || Config::TTP229SdoPin == NOT_A_PIN) return;
  // performance2("Key loop", 1);
  if (blocking) readKey();
  else getKey();
}

void TouchPadTTP229::getKey() {
  uint8_t key = ttp229->GetKey16(); // Non Blocking
  resolveKey(key);
}

void TouchPadTTP229::readKey() {
  uint8_t key = ttp229->ReadKey16(); // Blocking
  resolveKey(key);
}

bool TouchPadTTP229::isPressed(uint8_t key) {
  return key > 0 ? keys[key - 1] : false;
}

void TouchPadTTP229::resolveKey(uint8_t key) {
  static unsigned long last = 0;
  unsigned long dif = 0;
  dif = millis() - last;

  static uint8_t oldKey = 0;
  if (key != oldKey) {
    prf("Key pressed: %d\r\n", key);
    if (oldKey > 0) keys[oldKey - 1] = false;
    if (key > 0) keys[key - 1] = true;
    if (oldKey) dispatch(oldKey, KEY_UP);
    if (key) dispatch(key, KEY_DOWN);
    oldKey = key;
  } else {
    last = millis();
  }
}


void TouchPadTTP229::onKeyDown(KeyListener callback) {
  keyDownListeners.insert(callback);
}

void TouchPadTTP229::onKeyUp(KeyListener callback) {
  keyUpListeners.insert(callback);
}

void TouchPadTTP229::dispatch(uint8_t key, EventType type) {
  static Data newData = { { key } };
  newData.Key.key = key;
  BaseModule::dispatch(newData, type);
}

#endif