

#pragma once
#ifndef SMART_GARDEN_WIFI_STA_H
#define SMART_GARDEN_WIFI_STA_H

#include "./base/utils.hpp"
#include "./base/wifi.hpp"
#include "./variables/global.hpp"
#include "./controllers/RelayController.hpp"
#include "./controllers/SensorsController.hpp"
#include "./controllers/DisplayController.hpp"
#include "./controllers/WebsocketController.hpp"


void onKeyDown(uint8_t key) {
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


class SmartGardenStation {
  private:
  GardenWifi wifi;  // Giữ kết nối đến mạng wifi

  public:
  void setup();
  void loop();
};


/*                   Setup                  */
void SmartGardenStation::setup() {
  logStart("Station");
  i2cScanner();

  Global::setup();
  Global::touchPad.onKeyDown(onKeyDown);

  WiFi.mode(WIFI_STA);
  this->wifi.setup();

  relayCtl.setup();
  screenCtl.setup();
  sensorsCtl.setup();
  websocketCtl.setup();

  performanceDisable(0);
  toggleLogChannel(1, false);
}


/*                   Loop                    */
void SmartGardenStation::loop() {
  // static unsigned long timer = millis();
  //  digitalWrite(equips[0], 1);
  // if (millis() - timer > 1800000) reset(); // Khởi động lại ESP mỗi 30p để tránh treo
  
  performance("sensorsCtl");
  sensorsCtl.loop();
  
  performance("relayCtl");
  relayCtl.loop();
  
  performance("touchPad");
  Global::touchPad.loop();

  performance("screenCtl");
  screenCtl.loop();

  performance("websocketLoop");
  websocketCtl.loop();

  performance("end loop ------\r\n");
  delay(LOOP_DELAY_TIME);
}

#endif
