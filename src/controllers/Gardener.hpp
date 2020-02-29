
#pragma once
#ifndef BEYOND_GARDEN_GARDENER_H
#define BEYOND_GARDEN_GARDENER_H

#include "../utils/superws.hpp"
#include "../controllers/SensorsController.hpp"
#include "../controllers/RelayController.hpp"
#include "../models/StationState.hpp"
#include "./services/AIService.hpp"

#define GARDENER "Gardener"

defineWSListener(handleCommand);
defineListener(handleHuTempChange);
defineListener(handleLightChange);
defineListener(handleMovingChange);


struct Gardener {
  void setup() {
    relays.setup();
    sensors.setup();
    setupListeners();
  }

  void setupListeners() {
    ws.on("command", handleCommand);
    sensors.onHuTempChange(handleHuTempChange);
    sensors.onLightChange(handleLightChange);
    sensors.onMovingChange(handleMovingChange);
  }

  void loop() {
    sensors.loop();
    relays.loop();
  }
} gardener;

extern Gardener gardener;

defineWSListener(handleCommand) {
  log(GARDENER, "Garden Command!");
  deserializeJson(state.doc, payload);
  state.fromDoc(state.doc);
  relays.syncState();
  serializeJsonPretty(state.doc, Serial);
}

defineListener(handleHuTempChange) {
  state.temperature = event->data.HuTemp.temperature;
  state.humidity = event->data.HuTemp.humidity;
  display.printStationState();
  if (ws.isConnected()) {
    ws.emit(POST RecordsEndpoint, state.toJSON());
  }
}

defineListener(handleLightChange) {
  state.light = event->data.Light.light;
  display.printStationState();
  if (ws.isConnected()) {
    ws.emit(POST RecordsEndpoint, state.toJSON());
  }
}

defineListener(handleMovingChange) {
  state.moving = event->data.Moving.moving;
  display.printStationState();
  if (ws.isConnected()) {
    ws.emit(POST RecordsEndpoint, state.toJSON());
  }
}

#endif
