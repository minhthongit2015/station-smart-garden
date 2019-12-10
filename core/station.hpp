

#pragma once
#ifndef SMART_GARDEN_WIFI_STA_H
#define SMART_GARDEN_WIFI_STA_H

#include "./base/utils.hpp"
#include "./controllers/WifiController.hpp"
#include "./variables/global.hpp"
#include "./controllers/RelayController.hpp"
#include "./controllers/SensorsController.hpp"
#include "./controllers/DisplayController.hpp"
#include "./controllers/WebsocketController.hpp"
#include "./controllers/ConfigController.hpp"


void onKeyDown(KeyEvent event) {
  uint8_t key = event.data.Key.key;
  Global::lcd.lcd->printf(" %2d", key);
  switch (key)
  {
  case 1:
    Global::fsz.listingFiles();
    break;
  case 2:
    Global::cfg.loadConfigurations();
    break;
  case 3:
    Global::cfg.saveConfigurations();
    break;
  case 4: 
    Global::fsz.format();
    break;
  case 5:
    Global::fsz.showInfo();
    break;
  default:
    break;
  }
}

void onConfigChange();

class SmartGardenStation {
  public:
    void setup();
    void loop();
} station;


/*                   Setup                  */
void SmartGardenStation::setup() {
  logStart("Station");

  togglePerformanceChannel(0, false);
  toggleLogChannel(1, false); // Hide sensor value

  i2cScanner();

  Global::setup();
  configCtl.setup();
  Global::cfg.onChange(onConfigChange);

  Global::touchPad.onKeyDown(onKeyDown);
  displayCtl.setup();
  relayCtl.setup();
  sensorsCtl.setup();

  wifiCtl.setup();
  websocketCtl.setup();
}


/*                   Loop                    */
void SmartGardenStation::loop() {
  // static unsigned long timer = millis();
  //  digitalWrite(equips[0], 1);
  // if (millis() - timer > 1800000) reset(); // Khởi động lại ESP mỗi 30p để tránh treo

  #ifndef ENV_PROD
  performance("sensorsCtl");
  #endif
  sensorsCtl.loop();
  
  #ifndef ENV_PROD
  performance("relayCtl");
  #endif
  relayCtl.loop();
  
  #ifndef ENV_PROD
  performance("touchPad");
  #endif
  Global::touchPad.loop();

  #ifndef ENV_PROD
  performance("displayCtl");
  #endif
  displayCtl.loop();

  #ifndef ENV_PROD
  performance("configCtl");
  #endif
  configCtl.loop();

  #ifndef ENV_PROD
  performance("websocketLoop");
  #endif
  websocketCtl.loop();
  
  #ifndef ENV_PROD
  performance("wifiCtl");
  #endif
  wifiCtl.loop();

  #ifndef ENV_PROD
  performance("end loop ------\r\n");
  #endif
  delay(LOOP_DELAY_TIME);
}

void onConfigChange() {
  logz("Station", "Configurations change -> resetup wifi");
  wifiCtl.setup();
  websocketCtl.setup();
}

#endif
