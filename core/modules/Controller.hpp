
#pragma once
#ifndef SMART_GARDEN_CONTROLLER_H
#define SMART_GARDEN_CONTROLLER_H

#include "../base/utils.hpp"
#include "../stationstate.hpp"

class StationController {

public:
  int pump = 13;
  int led = 15;
  int fan = -1;
  int misting = -1;
  int nutri = -1;

  void setup() {
    prl("[Controller] <*> Setup");
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
    digitalWrite(pump, !isOn ? HIGH : LOW);
    // prf("Pump %s\r\n", !isOn ? "ON" : "OFF");
  }
  void setLed(bool isOn) {
    digitalWrite(led, !isOn ? HIGH : LOW);
    // prf("Led %s\r\n", !isOn ? "ON" : "OFF");
  }
  void setFan(bool isOn) {
    digitalWrite(fan, !isOn ? HIGH : LOW);
    // prf("Fan %s\r\n", !isOn ? "ON" : "OFF");
  }
  void setMisting(bool isOn) {
    digitalWrite(misting, !isOn ? HIGH : LOW);
    // prf("Misting %s\r\n", !isOn ? "ON" : "OFF");
  }
  void setNutri(bool isOn) {
    digitalWrite(nutri, !isOn ? HIGH : LOW);
    // prf("Nutri %s\r\n", !isOn ? "ON" : "OFF");
  }
} controller;

#endif