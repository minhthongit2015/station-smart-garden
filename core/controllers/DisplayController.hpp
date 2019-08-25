
#pragma once
#ifndef SMART_GARDEN_DISPLAY_CONTROLLER_H
#define SMART_GARDEN_DISPLAY_CONTROLLER_H

#include "../base/utils.hpp"
#include "../variables/global.hpp"

#include "./emotional/emotion.hpp"
#include "./emotional/pikachu.hpp"
#include "./emotional/sleep.hpp"

void screenOnKeyDown(uint8_t key);
void screenOnHuTempChange(float temperature, float humidity);
void screenOnLightChange(uint16_t light);

void printMovingDetection();

class DisplayController {
  public:
    bool busy = false;
    Emotion pikachuEmotion;
    Emotion sleepEmotion;
    Emotion *prevEmotion = NULL;

    void setup();
    void loop();

    void printStationState();
} screenCtl;

void DisplayController::setup() {
  Global::touchPad.onKeyDown(screenOnKeyDown);
  Global::dht.onChange(screenOnHuTempChange);
  Global::bh1750.onChange(screenOnLightChange);
  
  Global::lcd.setup();
  Global::lcd.lcd->noCursor();
  Global::lcd.lcd->noBlink();

  Global::lcd.printCenterLine("(^-^)", 2); delay(200);
  Global::lcd.printCenterLine("Hello I Am Baymax!", 1); delay(250);
  Global::lcd.printCenterLine("                  ", 1); delay(250);
  Global::lcd.printCenterLine("Hello I Am Baymax!", 1); delay(250);
  Global::lcd.printCenterLine("                  ", 1); delay(250);
  Global::lcd.printCenterLine("Hello I Am Baymax!", 1);
  delay(2000);
  Global::lcd.clear();

  pikachuEmotion.setup(Global::lcd);
  pikachuEmotion.insert(&pikachu1);
  pikachuEmotion.insert(&pikachu2);
  pikachuEmotion.insert(&pikachu3);
  pikachuEmotion.insert(&pikachu4);

  sleepEmotion.setup(Global::lcd);
  sleepEmotion.insert(&sleep1);
  sleepEmotion.insert(&sleep2);
  sleepEmotion.insert(&sleep3);
  sleepEmotion.insert(&sleep4);
  sleepEmotion.insert(&sleep5);

  prevEmotion = &sleepEmotion;
}

void DisplayController::loop() {
  if (Global::state.moving) {
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
    Global::lcd.lcd->clear();
    Global::lcd.printCenterLine("Resetting...", 1);
    Global::lcd.printCenterLine("(._.)zZ", 2);
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

void DisplayController::printStationState() {
  Global::lcd.setCursor(0, 0);
  Global::lcd.lcd->printf("%5.2f ", Global::state.temperature);
    Global::lcd.lcd->print('\xdf');
    Global::lcd.lcd->print('C');
  Global::lcd.setCursor(0, 1);
  Global::lcd.lcd->printf("%5.2f %%", Global::state.humidity);
  Global::lcd.setCursor(0, 2);
  Global::lcd.lcd->printf("%5d lx", Global::state.light);
  Global::lcd.setCursor(2, 3);
  if (Global::state.moving) {
    printMovingDetection();
  } else {
    Global::lcd.print("----");
  }
}

void printMovingDetection() {
  static unsigned long last = 0;
  static uint8_t posMap[4] = {0};
  static uint8_t lastPos = 0;
  if (millis() - last > 200) {
    for (int i=0; i < 4; ++i) {
      Global::lcd.lcd->printf("%c", i == lastPos ? '>' : ' ');
    }
    last = millis();
    lastPos = ++lastPos % 4;
  }
}

#endif
