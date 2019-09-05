
#pragma once
#ifndef SMART_GARDEN_RELAY_CONTROLLER_H
#define SMART_GARDEN_RELAY_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"

class RelayController {

public:
  int pump = 13;
  int led = 15;
  int fan = -1;
  int misting = -1;
  int nutri = -1;

  void setup() {
    logStart("Relay Controller");
    useOut(Global::cfg.pumpPin);     offRelay(Global::cfg.pumpPin);
    useOut(Global::cfg.ledPin);      offRelay(Global::cfg.ledPin);
    useOut(Global::cfg.fanPin);      offRelay(Global::cfg.fanPin);
    useOut(Global::cfg.mistingPin);  offRelay(Global::cfg.mistingPin);
    useOut(Global::cfg.nutriPin);    offRelay(Global::cfg.nutriPin);
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
    if (isEnable) onRelay(pump) else offRelay(pump);
  }
  void setLed(bool isEnable) {
    if (isEnable) onRelay(led) else offRelay(led);
  }
  void setFan(bool isEnable) {
    if (isEnable) onRelay(fan) else offRelay(fan);
  }
  void setMisting(bool isEnable) {
    if (isEnable) onRelay(misting) else offRelay(misting);
  }
  void setNutri(bool isEnable) {
    if (isEnable) onRelay(nutri) else offRelay(nutri);
  }
} relayCtl;

#endif