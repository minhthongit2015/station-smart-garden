
#pragma once
#ifndef BEYOND_GARDEN_RELAY_CONTROLLER_H
#define BEYOND_GARDEN_RELAY_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/State.hpp"
#include "../variables/Configuration.hpp"

class RelayController {
  private:
  public:
    int pump = NOT_A_PIN;
    int oxygen = NOT_A_PIN;
    int led = NOT_A_PIN;
    int fan = NOT_A_PIN;
    int misting = NOT_A_PIN;
    int nutri = NOT_A_PIN;

    void setPump(bool isEnable);
    void setOxygen(bool isEnable);
    void setLed(bool isEnable);
    void setFan(bool isEnable);
    void setMisting(bool isEnable);
    void setNutri(bool isEnable);
    void syncState();

    void setup();
    void loop();
} relays;

extern RelayController relays;

void RelayController::setup() {
  logStart("Relay Controller");
  if (cfg.pumpPin != NOT_A_PIN) {
    useOut(cfg.pumpPin);     offRelay(cfg.pumpPin);
  }
  if (cfg.oxygenPin != NOT_A_PIN) {
    useOut(cfg.oxygenPin);   offRelay(cfg.oxygenPin);
  }
  if (cfg.ledPin != NOT_A_PIN) {
    useOut(cfg.ledPin);      offRelay(cfg.ledPin);
  }
  if (cfg.fanPin != NOT_A_PIN) {
    useOut(cfg.fanPin);      offRelay(cfg.fanPin);
  }
  if (cfg.mistingPin != NOT_A_PIN) {
    useOut(cfg.mistingPin);  offRelay(cfg.mistingPin);
  }
  if (cfg.nutriPin != NOT_A_PIN) {
    useOut(cfg.nutriPin);    offRelay(cfg.nutriPin);
  }
  syncState();
}

void RelayController::loop() {
  // TODO: Check and stop nutri
}

void RelayController::syncState() {
  if (state.doc.containsKey("pump")) {
    state.pump = state.doc["pump"];
    this->setPump(state.pump);
  }
  if (state.doc.containsKey("oxygen")) {
    state.oxygen = state.doc["oxygen"];
    this->setOxygen(state.oxygen);
  }
  if (state.doc.containsKey("led")) {
    state.led = state.doc["led"];
    this->setLed(state.led);
  }
  if (state.doc.containsKey("fan")) {
    state.fan = state.doc["fan"];
    this->setFan(state.fan);
  }
  if (state.doc.containsKey("misting")) {
    state.misting = state.doc["misting"];
    this->setMisting(state.misting);
  }
  if (state.doc.containsKey("nutri")) {
    state.nutri = state.doc["nutri"];
    this->setNutri(state.nutri);
  }
}

void RelayController::setPump(bool isEnable) {
  if (isEnable && pump != NOT_A_PIN) onRelay(pump) else offRelay(pump);
}
void RelayController::setOxygen(bool isEnable) {
  if (isEnable && oxygen != NOT_A_PIN) onRelay(oxygen) else offRelay(oxygen);
}
void RelayController::setLed(bool isEnable) {
  if (isEnable && led != NOT_A_PIN) onRelay(led) else offRelay(led);
}
void RelayController::setFan(bool isEnable) {
  if (isEnable && fan != NOT_A_PIN) onRelay(fan) else offRelay(fan);
}
void RelayController::setMisting(bool isEnable) {
  if (isEnable && misting != NOT_A_PIN) onRelay(misting) else offRelay(misting);
}
void RelayController::setNutri(bool isEnable) {
  if (isEnable && nutri != NOT_A_PIN) onRelay(nutri) else offRelay(nutri);
}

#endif