
#pragma once
#ifndef BEYOND_GARDEN_KEYBOARD_H
#define BEYOND_GARDEN_KEYBOARD_H

#include "../modules/base/TTP229.hpp"
#include "../helpers/ConfigHelper.hpp"

#define PIN_TTP229_SCL "PIN_TTP229_SCL"
#define PIN_TTP229_SDO "PIN_TTP229_SDO"

class Keyboard : public TouchPadTTP229 {
  public:
  void setup() override {
    logStart("Keyboard");
    cfg.setDefault(PIN_TTP229_SCL, NOT_A_PIN);
    cfg.setDefault(PIN_TTP229_SDO, NOT_A_PIN);
    TouchPadTTP229::setup(cfg.getUInt8(PIN_TTP229_SCL), cfg.getUInt8(PIN_TTP229_SDO));
  }
} keyboard;

extern Keyboard keyboard;

#endif