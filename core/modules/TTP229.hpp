
#pragma once
#ifndef SMART_GARDEN_TTP229_H
#define SMART_GARDEN_TTP229_H

#include "../base/utils.hpp"
#include <TTP229.h>

#ifndef TTP229_SCL_PIN
  #define TTP229_SCL_PIN D3
#endif
#ifndef TTP229_SDO_PIN
  #define TTP229_SDO_PIN D4
#endif

typedef void (*keyListener)(uint8_t key);

class TouchPadTTP229 {
  private:
    void getKey();
    void readKey();
    void resolveKey(uint8_t key);
    bool blocking = false;
    unsigned long delay = 1000;
    unsigned long freqTime = 200;
  public:
    TTP229 _ttp229(D3, D4);
    bool keys[16] = {0};
    keyListener onKeyDown = NULL;
    keyListener onKeyUp = NULL;
    keyListener onKeyPress = NULL;

    void setup();
    void loop();

    void setDelayTime(unsigned long delay); // Delay before change to key press event
    void setKeyPressFreq(unsigned long freq); // Time between two key press event
    void setBlocking(bool isBlock);
    bool isPressed(uint8_t key);
};

void TouchPadTTP229::setup()
{
  logStart("Touch Pad TTP229");
}

void TouchPadTTP229::loop()
{
  if (blocking) readKey();
  else getKey();
}

void TouchPadTTP229::getKey() {
  uint8_t key = _ttp229.GetKey16(); // Non Blocking
  prf("-----> getKey( %d );\r\n", key);
  resolveKey(key);
}

void TouchPadTTP229::readKey() {
  uint8_t key = _ttp229.ReadKey16(); // Blocking
  resolveKey(key);
}

bool TouchPadTTP229::isPressed(uint8_t key) {
  return keys[key - 1];
}

void TouchPadTTP229::resolveKey(uint8_t key) {
  static unsigned long last = 0;
  unsigned long dif = 0;
  dif = millis() - last;

  static uint8_t oldKey = 0;
  if (key != oldKey) {
    prf("key: %d\r\n", key);
    if (onKeyUp) onKeyUp(oldKey);
    if (oldKey) keys[oldKey - 1] = false;

    if (onKeyDown) onKeyDown(key);
    if (key) keys[key - 1] = false;
  } else {
    // Key press
    last = millis();
  }

  oldKey = key;
}

#endif