
#pragma once
#ifndef SMART_GARDEN_RELAY_CONTROLLER_H
#define SMART_GARDEN_RELAY_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"

class RelayController {

public:
  int pump = NOT_A_PIN;
  int oxygen = NOT_A_PIN;
  int led = NOT_A_PIN;
  int fan = NOT_A_PIN;
  int misting = NOT_A_PIN;
  int nutri = NOT_A_PIN;

  void setup() {
    logStart("Relay Controller");
    if (Global::cfg.pumpPin != NOT_A_PIN) {
      useOut(Global::cfg.pumpPin);     offRelay(Global::cfg.pumpPin);
    }
    if (Global::cfg.oxygenPin != NOT_A_PIN) {
      useOut(Global::cfg.oxygenPin);   offRelay(Global::cfg.oxygenPin);
    }
    if (Global::cfg.ledPin != NOT_A_PIN) {
      useOut(Global::cfg.ledPin);      offRelay(Global::cfg.ledPin);
    }
    if (Global::cfg.fanPin != NOT_A_PIN) {
      useOut(Global::cfg.fanPin);      offRelay(Global::cfg.fanPin);
    }
    if (Global::cfg.mistingPin != NOT_A_PIN) {
      useOut(Global::cfg.mistingPin);  offRelay(Global::cfg.mistingPin);
    }
    if (Global::cfg.nutriPin != NOT_A_PIN) {
      useOut(Global::cfg.nutriPin);    offRelay(Global::cfg.nutriPin);
    }
    syncState();
  }

  void loop() {
    // TODO: Check and stop nutri
  }

  void syncState() {
    if (Global::state.doc.containsKey("pump")) {
      Global::state.pump = Global::state.doc["pump"];
      this->setPump(Global::state.pump);
    }
    if (Global::state.doc.containsKey("oxygen")) {
      Global::state.oxygen = Global::state.doc["oxygen"];
      this->setOxygen(Global::state.oxygen);
    }
    if (Global::state.doc.containsKey("led")) {
      Global::state.led = Global::state.doc["led"];
      this->setLed(Global::state.led);
    }
    if (Global::state.doc.containsKey("fan")) {
      Global::state.fan = Global::state.doc["fan"];
      this->setFan(Global::state.fan);
    }
    if (Global::state.doc.containsKey("misting")) {
      Global::state.misting = Global::state.doc["misting"];
      this->setMisting(Global::state.misting);
    }
    if (Global::state.doc.containsKey("nutri")) {
      Global::state.nutri = Global::state.doc["nutri"];
      this->setNutri(Global::state.nutri);
    }
  }

  void setPump(bool isEnable) {
    if (isEnable && pump != NOT_A_PIN) onRelay(pump) else offRelay(pump);
  }
  void setOxygen(bool isEnable) {
    if (isEnable && oxygen != NOT_A_PIN) onRelay(oxygen) else offRelay(oxygen);
  }
  void setLed(bool isEnable) {
    if (isEnable && led != NOT_A_PIN) onRelay(led) else offRelay(led);
  }
  void setFan(bool isEnable) {
    if (isEnable && fan != NOT_A_PIN) onRelay(fan) else offRelay(fan);
  }
  void setMisting(bool isEnable) {
    if (isEnable && misting != NOT_A_PIN) onRelay(misting) else offRelay(misting);
  }
  void setNutri(bool isEnable) {
    if (isEnable && nutri != NOT_A_PIN) onRelay(nutri) else offRelay(nutri);
  }
} relayCtl;

#endif