
#pragma once
#ifndef BEYOND_GARDEN_GARDENER_H
#define BEYOND_GARDEN_GARDENER_H

#include "../utils/superws.hpp"
#include "../controllers/WifiManager.hpp"
#include "../controllers/SensorsController.hpp"
#include "../controllers/RelayController.hpp"
#include "../modules/Display.hpp"
#include "../models/StationState.hpp"
#include "../services/AIService.hpp"
#include "../utils/Constants.hpp"

#define GARDENER "Gardener"

declareWSListener(handleSetState);
declareListener(handleHuTempChange);
declareListener(handleLightChange);
declareListener(handleMovingChange);


struct Gardener {
  void setup() {
    sensors.setup();
    relays.setup();
    setupListeners();
    displayStationStatus();
  }

  void setupListeners() {
    ws.on(WSEvent.SET_STATE, handleSetState);
    sensors.onHuTempChange(handleHuTempChange);
    sensors.onLightChange(handleLightChange);
    sensors.onMovingChange(handleMovingChange);
  }

  void loop() {
    sensors.loop();
    relays.loop();
  }

  void handleStateChange() {
    // log(GARDENER, "Station state change");
    displayStationStatus();
    if (ws.isConnected()) {
      ws.emit(POST RecordsEndpoint, state.toJSON());
    }
  }

  void displayStationStatus();
  void printMovingDetection();
} gardener;

extern Gardener gardener;

declareWSListener(handleSetState) {
  log(GARDENER, "Garden set station state");
  deserializeJson(state.doc, payload);
  state.fromDoc(state.doc);
  relays.syncState();
  serializeJsonPretty(state.doc, Serial); prl();
}

declareListener(handleHuTempChange) {
  state.temperature = event->data.HuTemp.temperature;
  state.humidity = event->data.HuTemp.humidity;
  gardener.handleStateChange();
}

declareListener(handleLightChange) {
  state.light = event->data.Light.light;
  gardener.handleStateChange();
}

declareListener(handleMovingChange) {
  state.moving = event->data.Moving.moving;
  gardener.handleStateChange();
}

void Gardener::displayStationStatus() {
  display.lcd.setLine(0);
  display.lcd.printf("%5.2f ", state.temperature); display.lcd.print('\xdf'); display.lcd.print('C');
  display.lcd.setLine(1);
  display.lcd.printf("%5.2f %%", state.humidity);
  display.lcd.setLine(2);
  display.lcd.printf("%5d lx", state.light);
  display.lcd.setLine(3);
  if (state.moving) {
    printMovingDetection();
  } else {
    display.lcd.print("  ----  ");
  }
  display.lcd.printRight(WiFi.SSID().c_str(), 0);
}

void Gardener::printMovingDetection() {
  static unsigned long last = 0;
  static uint8_t lastPos = 0;
  if (millis() - last > 200) {
    display.lcd.print("  ");
    for (int i=0; i < 4; ++i) {
      display.lcd.printf("%c", i == lastPos ? '>' : ' ');
    }
    display.lcd.print("  ");
    last = millis();
    lastPos = ++lastPos % 4;
  }
}

#endif
