
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

    void setup() {
      lcd.setup();
      lcd.noCursor();
      lcd.noBlink();

      lcd.clear();
      lcd.printCenter("(^-^)", 2); delay(200);
      lcd.printCenter("Hello I Am Baymax!", 1); delay(250);
      lcd.printCenter("                  ", 1); delay(250);
      lcd.printCenter("Hello I Am Baymax!", 1); delay(250);
      lcd.printCenter("                  ", 1); delay(250);
      lcd.printCenter("Hello I Am Baymax!", 1);
      delay(2000);
      lcd.clear();

      // pikachuEmotion.setup(lcd);
      // pikachuEmotion.insert(&pikachu1);
      // pikachuEmotion.insert(&pikachu2);
      // pikachuEmotion.insert(&pikachu3);
      // pikachuEmotion.insert(&pikachu4);

      // sleepEmotion.setup(lcd);
      // sleepEmotion.insert(&sleep1);
      // sleepEmotion.insert(&sleep2);
      // sleepEmotion.insert(&sleep3);
      // sleepEmotion.insert(&sleep4);
      // sleepEmotion.insert(&sleep5);

      // prevEmotion = &sleepEmotion;
    }

    void loop() {
      // if (state.moving) {
      //   if (prevEmotion != &pikachuEmotion) {
      //     (*prevEmotion).clearPrevFrame();
      //     prevEmotion = &pikachuEmotion;
      //   }
      //   pikachuEmotion.play();
      // } else {
      //   if (prevEmotion != &sleepEmotion) {
      //     (*prevEmotion).clearPrevFrame();
      //     prevEmotion = &sleepEmotion;
      //   }
      //   sleepEmotion.play();
      // }
    }
} display;

extern Display display;


#endif
