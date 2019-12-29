

#pragma once
#ifndef BEYOND_GARDEN_BH1750_H
#define BEYOND_GARDEN_BH1750_H

#include "../base/utils.hpp"
#include "./_BaseModule.hpp"
#include <set>
#include <BH1750FVI.h>


class LightBH1750 : public BaseModule {
  private:
  public:
    uint16_t NULL_VALUE = 54612;
    EventType getDefaultEventType() override {
      return LIGHT_CHANGE;
    }
    BH1750FVI *pBH1750 = NULL;
    Data light = { { 0 } };

    LightBH1750() : BaseModule(&light) {
      CHECK_INTERVAL = 2000;
      listenersMap.insert(ListenerPair(getDefaultEventType(), &changeListeners));
    }

    void setup() override;
    void loop() override;
    bool fetch() override;
};

void LightBH1750::setup() {
  logStart("Light Sensor (BH1750)");
  pBH1750 = new BH1750FVI(BH1750FVI::k_DevModeContHighRes);
  (*pBH1750).begin();
}

void LightBH1750::loop() {
  if (!check()) return;
  if (light.Light != prevData) {
    dispatch(light);
  }
}

bool LightBH1750::fetch() {
  if (!pBH1750) return false;
  static Data newData = { { 0, 0 } };

  newData.Light.light = (*pBH1750).GetLightIntensity();
  if (newData.Light.light == NULL_VALUE) {
    return false;
  }

  light = newData;
  return true;
}

#endif
