

#pragma once
#ifndef SMART_GARDEN_BH1750_H
#define SMART_GARDEN_BH1750_H

#include "../base/utils.hpp"
#include <set>
#include <BH1750FVI.h>

typedef void (*onLightChangeListener)(uint16_t light);

class LightBH1750 {
  private:
    std::set<onLightChangeListener> _onChange;
  public:
    BH1750FVI bh1750;
    uint16_t prevLight = 80;
    uint16_t light = 80;
    unsigned long delayTime = 2000;

    LightBH1750()
      :bh1750(BH1750FVI::k_DevModeContHighRes)
    {

    }

    void setup();
    void loop();

    void onChange(onLightChangeListener callback) {
      this->_onChange.insert(callback);
    }

    bool read();
    void dispatchEvent(uint16_t light);
};

void LightBH1750::setup() {
  logStart("Light Sensor (BH1750)");
  bh1750.begin();
}

void LightBH1750::loop() {
  if (!read()) return;
  if ((prevLight != light)
      && _onChange.size() > 0) {
    dispatchEvent(light);
  }
  prevLight = light;
}

bool LightBH1750::read() {
  static unsigned long now = 0;
  static unsigned long last = 0;
  static unsigned long dif = 0;
  now = millis();
  dif = now - last;
  
  if (dif >= delayTime) {
    light = bh1750.GetLightIntensity();
    last = now;
    return true;
  }

  return false;
}

void LightBH1750::dispatchEvent(uint16_t light) {
	for (std::set<onLightChangeListener>::iterator p = _onChange.begin(); p != _onChange.end(); ++p) {
    (*p)(light);
  }
}

#endif
