
#pragma once
#ifndef SMART_GARDEN_CONTROLLER_H
#define SMART_GARDEN_CONTROLLER_H

#include "../base/utils.hpp"
#include "../stationstate.hpp"

class StationController {

public:
  int pump = 3;
  int led = 4;
  int fan = 5;
  int misting = 6;
  int nutri = 7;

  void setup() {
    useOut(pump);
    useOut(led);
    useOut(fan);
    useOut(misting);
    useOut(nutri);
  }

  void loop() {
    // TODO: Check and stop nutri
  }

  void executeCommand() {
    this->setPump(state.pump);
    this->setLed(state.led);
    this->setFan(state.fan);
    this->setMisting(state.misting);
    this->setNutri(state.nutri);
  }

  void setPump(bool isOn) {
    digitalWrite(pump, isOn ? HIGH : LOW);
  }
  void setLed(bool isOn) {
    digitalWrite(led, isOn ? HIGH : LOW);
  }
  void setFan(bool isOn) {
    digitalWrite(fan, isOn ? HIGH : LOW);
  }
  void setMisting(bool isOn) {
    digitalWrite(misting, isOn ? HIGH : LOW);
  }
  void setNutri(bool isOn) {
    digitalWrite(nutri, isOn ? HIGH : LOW);
  }
} controller;

#endif