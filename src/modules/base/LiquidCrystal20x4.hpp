
#pragma once
#ifndef BEYOND_GARDEN_LIQUID_CRYSTAL_I2C_20x4_H
#define BEYOND_GARDEN_LIQUID_CRYSTAL_I2C_20x4_H

#include "../../utils/Utils.hpp"
#include <LiquidCrystal_I2C.h>


class LiquidCrystal20x4 : public LiquidCrystal_I2C {
  private:
  public:
    uint8_t width = 20;
    uint8_t height = 4;
    char screen[4][20] = {
      {0}, {0}, {0}, {0}
    };

    LiquidCrystal20x4() : LiquidCrystal20x4(0x27, 20, 4) {
    }

    LiquidCrystal20x4(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
      : LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows) {
    }

    void setup();
    void loop();
    // void hScroll(const char *message, int delayTime, int shiftCol = -1, int8_t row = -1);
    // void vScroll(const char *message, int delayTime, int shiftRow = 1, int8_t col = -1);
    
    void print(const char *text);
    void print(char c);
    void printCenterLine(const char *text, uint8_t line, uint8_t lineLength = 20);
    void printLeftLine(const char *text, uint8_t line, uint8_t lineLength = 20);
    void printCenter(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t width = 20, uint8_t height = 4);
};

void LiquidCrystal20x4::setup() {
  logStart("Display (LiquidCrystal_I2C 20x4)");

  init();
  backlight(); delay(500);
  noBacklight(); delay(500);
  backlight();
  
  cursor();
  blink();
}

void LiquidCrystal20x4::loop() {
}

void LiquidCrystal20x4::print(const char *text) {
  printstr(text);
}

void LiquidCrystal20x4::print(char c) {
  LiquidCrystal_I2C::print(c);
}

void LiquidCrystal20x4::printCenterLine(const char *text, uint8_t line, uint8_t lineLength) {
  uint8_t length = strlen(text);
  setCursor((lineLength - length) / 2, line);
  print(text);
}
void LiquidCrystal20x4::printLeftLine(const char *text, uint8_t line, uint8_t lineLength) {
  uint8_t length = strlen(text);
  setCursor(0, line);
  print(text);
}
void LiquidCrystal20x4::printCenter(const char *text, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
} 

// void scrollText(int row, String message, int delayTime, int lcdColumns) {
//   for (int i=0; i < lcdColumns; i++) {
//     message = " " + message;  
//   } 
//   message = message + " "; 
//   for (int pos = 0; pos < message.length(); pos++) {
//     lcd->setCursor(0, row);
//     lcd->print(message.substring(pos, pos + lcdColumns));
//     delay(delayTime);
//   }
// }

#endif
