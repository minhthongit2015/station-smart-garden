
#pragma once
#ifndef SMART_GARDEN_TTP229_H
#define SMART_GARDEN_TTP229_H

#include "../base/utils.hpp"
#include <TTP229.h>
#include <set>

typedef void (*keyListener)(uint8_t key);

// typedef std::set<std::function<void (uint8_t key)>> keyListener;

class TouchPadTTP229 {
  private:
    void getKey();
    void readKey();
    void resolveKey(uint8_t key);
    bool blocking = false;
    unsigned long delay = 1000;
    unsigned long freqTime = 200;

    std::set<keyListener> _onKeyDown;
    std::set<keyListener> _onKeyUp;
    std::set<keyListener> _onKeyPress;
    bool keys[16] = {0};
  public:
    TTP229 *ttp229;

    void onKeyDown(keyListener callback);
    void onKeyUp(keyListener callback);
    void triggerEvent(std::set<keyListener> callbackSet, uint8_t key);

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
  ttp229 = new TTP229(Config::TTP229SclPin, Config::TTP229SdoPin);
}

void TouchPadTTP229::loop()
{
  // performance2("Key loop", 1);
  if (blocking) readKey();
  else getKey();
}

void TouchPadTTP229::getKey() {
  uint8_t key = ttp229->GetKey16(); // Non Blocking
  // prf("-----> getKey( %d );\r\n", key);
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
    prf("key: %d\r\n", key);
    if (oldKey && _onKeyUp.size() > 0) triggerEvent(_onKeyUp, oldKey);
    if (oldKey > 0) keys[oldKey - 1] = false;

    if (key && _onKeyDown.size() > 0) triggerEvent(_onKeyDown, key);
    if (key > 0) keys[key - 1] = true;

    oldKey = key;
  } else {
    // Key press
    last = millis();
  }
}


void TouchPadTTP229::onKeyDown(keyListener callback) {
  _onKeyDown.insert(callback);
}

void TouchPadTTP229::onKeyUp(keyListener callback) {
  _onKeyUp.insert(callback);
}

void TouchPadTTP229::triggerEvent(std::set<keyListener> callbackSet, uint8_t key) {
	for (std::set<keyListener>::iterator p = callbackSet.begin(); p != callbackSet.end(); ++p) {
    (*p)(key);
  }
}

#endif