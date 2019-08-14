
#pragma once
#ifndef SMART_GARDEN_SCREEN_CONTROLLER_H
#define SMART_GARDEN_SCREEN_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"
#include "./pikachu1.hpp"
#include "./pikachu2.hpp"
#include "./pikachu3.hpp"
#include "./emotion.hpp"

void screenOnKeyDown(uint8_t key);

class ScreenController {
  public:
    bool busy = false;
    Emotion pikachuEmotion;

    ScreenController()
      :pikachuEmotion(lcd)
    {

    }

    void setup();
    void loop();
} screenCtl;

void screenOnKeyDown(uint8_t key) {
  if (key == 16) {
    screenCtl.busy = false;
  }
}

void ScreenController::setup() {
  touchPad.onKeyDown(screenOnKeyDown);
  
  lcd.setup();
  lcd.lcd.noCursor();
  lcd.lcd.noBlink();

  lcd.printCenterLine("(^-^)", 2); delay(200);
  lcd.printCenterLine("Hello I Am Baymax!", 1); delay(250);
  lcd.printCenterLine("                  ", 1); delay(250);
  lcd.printCenterLine("Hello I Am Baymax!", 1); delay(250);
  lcd.printCenterLine("                  ", 1); delay(250);
  lcd.printCenterLine("Hello I Am Baymax!", 1);
  delay(2000);
  lcd.clear();

  pikachuEmotion.insert(&pikachu1);
  pikachuEmotion.insert(&pikachu2);
  pikachuEmotion.insert(&pikachu3);
}

void ScreenController::loop() {
  pikachuEmotion.play();
}

#endif
