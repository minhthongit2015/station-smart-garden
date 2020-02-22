

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
    BH1750FVI *pBH1750 = NULL;

    void setup() override {
      logStart("Light Sensor (BH1750)");
      pBH1750 = new BH1750FVI(BH1750FVI::k_DevModeContHighRes);
      pBH1750->begin();
    }
    void fetch(EventData &newData) override {
      newData.Light.light = pBH1750->GetLightIntensity();
    }
    bool validate(EventData &newData) override {
      return !!newData.Light;
    }
    bool hasChange() override {
      return data.Light != prevData;
    }
};

#endif
