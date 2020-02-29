
#pragma once
#ifndef BEYOND_GARDEN_DISPLAY_H
#define BEYOND_GARDEN_DISPLAY_H

#include "../utils/Utils.hpp"
#include "../models/StationState.hpp"

#include "../assets/emotional/emotion.hpp"
#include "../assets/emotional/pikachu/pikachu.hpp"
#include "../assets/emotional/sleep/sleep.hpp"


void screenOnKeyDown(Event event);
void screenOnHuTempChange(Event event);
void screenOnLightChange(Event event);

class Display {
  public:
    bool busy = false;
    Emotion *prevEmotion = NULL;
    LiquidCrystal20x4 lcd;
    Emotion sleepEmotion;
    Emotion pikachuEmotion;


    void setup();
    void loop();

    void printStationState();
    void printMovingDetection();
} display;

extern Display display;

void Display::setup() {
  lcd.setup();
  lcd.noCursor();
  lcd.noBlink();

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

void Display::loop() {
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

void Display::printStationState() {
  lcd.setCursor(0, 0);
  lcd.printf("%5.2f ", state.temperature);
    lcd.print('\xdf');
    lcd.print('C');
  lcd.setCursor(0, 1);
  lcd.printf("%5.2f %%", state.humidity);
  lcd.setCursor(0, 2);
  lcd.printf("%5d lx", state.light);
  lcd.setCursor(2, 3);
  if (state.moving) {
    printMovingDetection();
  } else {
    lcd.print("----");
  }
}

void Display::printMovingDetection() {
  static unsigned long last = 0;
  static uint8_t posMap[4] = {0};
  static uint8_t lastPos = 0;
  if (millis() - last > 200) {
    for (int i=0; i < 4; ++i) {
      display.lcd.printf("%c", i == lastPos ? '>' : ' ');
    }
    last = millis();
    lastPos = ++lastPos % 4;
  }
}

#endif
