

#pragma once
#ifndef SMART_GARDEN_BH1750_H
#define SMART_GARDEN_BH1750_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <set>
#include <BH1750FVI.h>


class LightBH1750 : public BaseModule {
  private:
  public:
    BH1750FVI bh1750;
    Data prevLight = { { 0 } };
    Data light = { { 0 } };

    LightBH1750()
      :bh1750(BH1750FVI::k_DevModeContHighRes)
    {
      CHECK_INTERVAL = 2000;
    }

    void setup();
    void loop();
    bool fetch();
};

void LightBH1750::setup() {
  logStart("Light Sensor (BH1750)");
  bh1750.begin();
}

void LightBH1750::loop() {
  if (!check()) return;
  if (light.Light != prevLight) {
    dispatch(light, VALUE_CHANGE);
    prevLight = light;
  }
}

bool LightBH1750::fetch() {
  static Data newData = { { 0 } };
  light.Light.light = bh1750.GetLightIntensity();
  return true;
}

#endif
