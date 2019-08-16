
#pragma once
#ifndef SMART_GARDEN_SCREEN_CONTROLLER_H
#define SMART_GARDEN_SCREEN_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"
#include "./emotional/pikachu.hpp"
#include "./emotional/sleep.hpp"
#include "./emotional/emotion.hpp"

void screenOnKeyDown(uint8_t key);
void screenOnHuTempChange(float temperature, float humidity);
void screenOnLightChange(uint16_t light);

void printMovingDetection();

class ScreenController {
  public:
    bool busy = false;
    Emotion pikachuEmotion;
    Emotion sleepEmotion;
    Emotion *prevEmotion = NULL;

    void setup();
    void loop();

    void printStationState();
} screenCtl;

void ScreenController::setup() {
  touchPad.onKeyDown(screenOnKeyDown);
  dht.onChange(screenOnHuTempChange);
  bh1750.onChange(screenOnLightChange);
  
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

  pikachuEmotion.setup(lcd);
  pikachuEmotion.insert(&pikachu1);
  pikachuEmotion.insert(&pikachu2);
  pikachuEmotion.insert(&pikachu3);
  pikachuEmotion.insert(&pikachu4);

  sleepEmotion.setup(lcd);
  sleepEmotion.insert(&sleep1);
  sleepEmotion.insert(&sleep2);
  sleepEmotion.insert(&sleep3);
  sleepEmotion.insert(&sleep4);
  sleepEmotion.insert(&sleep5);

  prevEmotion = &sleepEmotion;
}

void ScreenController::loop() {
  if (state.moving) {
    if (prevEmotion != &pikachuEmotion) {
      (*prevEmotion).clearPrevFrame();
      prevEmotion = &pikachuEmotion;
    }
    pikachuEmotion.play();
  } else {
    if (prevEmotion != &sleepEmotion) {
      (*prevEmotion).clearPrevFrame();
      prevEmotion = &sleepEmotion;
    }
    sleepEmotion.play();
  }
  printStationState();
}

// --------------------------------------------------

void screenOnKeyDown(uint8_t key) {
  if (key == 16) {
    screenCtl.busy = false;
    lcd.lcd.clear();
    lcd.printCenterLine("Resetting...", 1);
    lcd.printCenterLine("(._.)zZ", 2);
    delay(1000);
    reset();
  }
}

void screenOnHuTempChange(float temperature, float humidity) {
  screenCtl.printStationState();
}

void screenOnLightChange(uint16_t light) {
  screenCtl.printStationState();
}

// --------------------------------------------------

void ScreenController::printStationState() {
  lcd.setCursor(0, 0);
  lcd.lcd.printf("%5.2f ", state.temperature); lcd.lcd.print('\xdf'); lcd.lcd.print('C');
  lcd.setCursor(0, 1);
  lcd.lcd.printf("%5.2f %%", state.humidity);
  lcd.setCursor(0, 2);
  lcd.lcd.printf("%5d lx", state.light);
  lcd.setCursor(2, 3);
  if (state.moving) {
    printMovingDetection();
  } else {
    lcd.print("----");
  }
}

void printMovingDetection() {
  static unsigned long last = 0;
  static uint8_t posMap[4] = {0};
  static uint8_t lastPos = 0;
  if (millis() - last > 200) {
    for (int i=0; i < 4; ++i) {
      lcd.lcd.printf("%c", i == lastPos ? '>' : ' ');
    }
    last = millis();
    lastPos = ++lastPos % 4;
  }
}

#endif
