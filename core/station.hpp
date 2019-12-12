

#pragma once
#ifndef BEYOND_GARDEN_WIFI_STA_H
#define BEYOND_GARDEN_WIFI_STA_H

#include "./base/utils.hpp"
#include "./variables/Configuration.hpp"
#include "./controllers/Display.hpp"
#include "./controllers/Keyboard.hpp"
#include "./controllers/SensorsController.hpp"
#include "./controllers/RelayController.hpp"
#include "./base/WifiManager.hpp"
#include "./controllers/Network.hpp"


void onConfigChange(Event event);
void onHuTempChange(Event event);
void onLightChange(Event event);
void onMovingChange(Event event);
void onKeyDown(Event event);

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
  toggleLogChannel(1, true); // Hide sensor value

  i2cScanner();

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

  #ifndef ENV_PROD
  performance("sensorsCtl");
  #endif
  sensors.loop();
  
  #ifndef ENV_PROD
  performance("relayCtl");
  #endif
  relays.loop();
  
  #ifndef ENV_PROD
  performance("keyboard");
  #endif
  keyboard.loop();

  #ifndef ENV_PROD
  performance("displayCtl");
  #endif
  display.loop();

  #ifndef ENV_PROD
  performance("configCtl");
  #endif
  cfg.loop();

  #ifndef ENV_PROD
  performance("websocketLoop");
  #endif
  network.loop();
  
  #ifndef ENV_PROD
  performance("wifiMgr");
  #endif
  wifiMgr.loop();

  #ifndef ENV_PROD
  performance("end loop ------\r\n");
  #endif
  delay(MAIN_LOOP_DELAY_TIME);
}

void onConfigChange(Event event) {
  log("Station", "Configurations change -> resetup wifi");
  wifiMgr.setup();
  network.setup();
}

void onKeyDown(Event event) {
  uint8_t key = event.data.Key.key;
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

void onHuTempChange(Event event) {
  state.temperature = event.data.HuTemp.temperature;
  state.humidity = event.data.HuTemp.humidity;
  display.printStationState();
  if (network.connected) {
    network.emit(POST stationStateEndpoint, state.toJSON());
  }
}
void onLightChange(Event event) {
  state.light = event.data.Light.light;
  display.printStationState();
  if (network.connected) {
    network.emit(POST stationStateEndpoint, state.toJSON());
  }
}
void onMovingChange(Event event) {
  state.moving = event.data.Moving.moving;
  display.printStationState();
  if (network.connected) {
    network.emit(POST stationStateEndpoint, state.toJSON());
  }
}

#endif
