
#pragma once
#ifndef SMART_GARDEN_GLOBAL_H
#define SMART_GARDEN_GLOBAL_H

#include "./state.hpp"
#include "../base/fs.hpp"
#include "./config.hpp"
#include "../base/utils.hpp"

#include "../modules/LiquidCrystal20x4.hpp"
#include "../modules/TTP229.hpp"
#include "../modules/DHT22.hpp"
#include "../modules/BH1750.hpp"
#include "../modules/HCSR501.hpp"

class Global {
  public:
    static StationState state;
    static FileSystem fsz;
    static Config cfg;
    static LiquidCrystal20x4 lcd;
    static TouchPadTTP229 touchPad;
    static HuTempDHT22 dht;
    static LightBH1750 bh1750;
    static MotionDetectorHCSR501 hcsr501;

    static void setup() {
      logStart("Global");

      fsz.setup();
      cfg.setup(fsz);
      cfg.loadConfigurations();
      touchPad.setup();
    }
};

StationState Global::state = {0};
FileSystem Global::fsz;
Config Global::cfg;
LiquidCrystal20x4 Global::lcd;
TouchPadTTP229 Global::touchPad;
HuTempDHT22 Global::dht;
LightBH1750 Global::bh1750;
MotionDetectorHCSR501 Global::hcsr501;


#endif