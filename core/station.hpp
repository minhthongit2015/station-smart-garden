

#pragma once
#ifndef BEYOND_GARDEN_WIFI_STA_H
#define BEYOND_GARDEN_WIFI_STA_H

#include "./base/ArduinoDefine.hpp"
#include "./base/utils.hpp"
#include "./base/fs.hpp"
#include "./controllers/ConfigManager.hpp"
#include "./controllers/Display.hpp"
#include "./controllers/Keyboard.hpp"
#include "./controllers/SensorsController.hpp"
#include "./controllers/RelayController.hpp"
#include "./controllers/WifiManager.hpp"
#include "./controllers/Network.hpp"


void onConfigChange(pEvent event);
void onHuTempChange(pEvent event);
void onLightChange(pEvent event);
void onMovingChange(pEvent event);
void onKeyDown(pEvent event);

class SmartGardenStation {
  public:
    void setup();
    void setupListeners();
    void loop();
} station;


/*                   Setup                  */
void SmartGardenStation::setup() {
  logStart("Station");

  togglePerformanceChannel(0, false);
  toggleLogChannel(1, false); // false: Hide sensor value

  i2cScanner();

  fsz.setup();
  cfg.setup();

  display.setup();
  keyboard.setup();
  relays.setup();
  sensors.setup();

  wifiMgr.setup();
  network.setup();

  setupListeners();
}

void SmartGardenStation::setupListeners() {
  cfg.onChange(onConfigChange);
  keyboard.onKeyDown(onKeyDown);
  sensors.onHuTempChange(onHuTempChange);
  sensors.onLightChange(onLightChange);
  sensors.onMovingChange(onMovingChange);
}

/*                   Loop                    */
void SmartGardenStation::loop() {
  // static unsigned long timer = millis();
  //  digitalWrite(equips[0], 1);
  // if (millis() - timer > 1800000) reset(); // Khởi động lại ESP mỗi 30p để tránh treo

  #ifdef ENV_DEV
  performance("sensorsCtl");
  #endif
  sensors.loop();
  
  #ifdef ENV_DEV
  performance("relayCtl");
  #endif
  relays.loop();
  
  #ifdef ENV_DEV
  performance("keyboard");
  #endif
  // keyboard.loop();

  #ifdef ENV_DEV
  performance("displayCtl");
  #endif
  display.loop();

  #ifdef ENV_DEV
  performance("configCtl");
  #endif
  cfg.loop();

  #ifdef ENV_DEV
  performance("websocketLoop");
  #endif
  network.loop();
  
  #ifdef ENV_DEV
  performance("wifiMgr");
  #endif
  wifiMgr.loop();

  #ifdef ENV_DEV
  performance("end loop ------\r\n");
  #endif
  delay(MAIN_LOOP_DELAY_TIME);
}

void onConfigChange(pEvent event) {
  log("Station", "Configurations change -> resetup wifi");
  wifiMgr.setup();
  network.setup();
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
      cfg.saveConfigurations();
      break;
    case 4: 
      fsz.format();
      break;
    case 5:
      fsz.showInfo();
      break;
    case 16:
      display.busy = false;
      display.lcd.clear();
      display.lcd.printCenterLine("Resetting...", 1);
      display.lcd.printCenterLine("(._.)zZ", 2);
      delay(1000);
      reset();
      break;
    default:
      break;
  }
}

void onHuTempChange(pEvent event) {
  state.temperature = event->data.HuTemp.temperature;
  state.humidity = event->data.HuTemp.humidity;
  display.printStationState();
  if (network.connected) {
    network.emit(POST RecordsEndpoint, state.toJSON());
  }
}
void onLightChange(pEvent event) {
  state.light = event->data.Light.light;
  display.printStationState();
  if (network.connected) {
    network.emit(POST RecordsEndpoint, state.toJSON());
  }
}
void onMovingChange(pEvent event) {
  state.moving = event->data.Moving.moving;
  display.printStationState();
  if (network.connected) {
    network.emit(POST RecordsEndpoint, state.toJSON());
  }
}

#endif
