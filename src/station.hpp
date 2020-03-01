

#pragma once
#ifndef BEYOND_GARDEN_WIFI_STA_H
#define BEYOND_GARDEN_WIFI_STA_H

#include "./configs/ArduinoDefine.hpp"
#include "./utils/Utils.hpp"
#include "./utils/fs.hpp"
#include "./helpers/ConfigHelper.hpp"
#include "./modules/Display.hpp"
#include "./modules/Keyboard.hpp"
#include "./controllers/NetworkManager.hpp"
#include "./controllers/Gardener.hpp"


void onConfigChange(pEvent event);
void onKeyDown(pEvent event);

struct SmartGardenStation {
  void setup() {
    logStart("Station");

    i2cScanner();

    fsz.setup();
    cfg.setup();

    display.setup();
    keyboard.setup();

    network.setup();
    gardener.setup();

    setupListeners();
  }

  void setupListeners() {
    cfg.onChange(onConfigChange);
    keyboard.onKeyDown(onKeyDown);
  }

  void loop() {
    cfg.loop();
    display.loop();
    network.loop();
    // gardener.loop();
    delay(MAIN_LOOP_DELAY_TIME);
  }
} station;

extern SmartGardenStation station;

void onConfigChange(pEvent event) {
  log("Station", "Configurations change!");
  reset();
}

void onKeyDown(pEvent event) {
  uint8_t key = event->data.Key.key;
  display.lcd.printf(" %2d", key);
  switch (key) {
    case 1:
      fsz.listingFiles();
      break;
    case 2:
      cfg.loadConfigurations();
      break;
    case 3:
      cfg.save();
      break;
    case 4: 
      fsz.format();
      break;
    case 5:
      fsz.showInfo();
      break;
    case 16:
      display.busy = true;
      display.lcd.clear();
      display.lcd.printCenter("Resetting...", 1);
      display.lcd.printCenter("(=_=)zZ", 2);
      delay(1000);
      reset();
      break;
    default:
      break;
  }
}

#endif
