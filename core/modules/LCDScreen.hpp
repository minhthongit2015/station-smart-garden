

#pragma once
#ifndef SMART_GARDEN_LCDScreen_H
#define SMART_GARDEN_LCDScreen_H

#include "../base/utils.hpp"
#include <LiquidCrystal_I2C.h>

class LCDScreen {
  private:
  public:
    uint8_t width = 20;
    uint8_t height = 4;
    char screen[4][20] = {
      {0}, {0}, {0}, {0}
    };
    LiquidCrystal_I2C *lcd;

    void setup();
    void loop();
    // void hScroll(const char *message, int delayTime, int shiftCol = -1, int8_t row = -1);
    // void vScroll(const char *message, int delayTime, int shiftRow = 1, int8_t col = -1);
    
    void clear();
    void setCursor(uint8_t x = 0, uint8_t y = 0);
    void print(const char *text);
    void printc(char c);
    void printCenterLine(const char *text, uint8_t line, uint8_t lineLength = 20);
    void printLeftLine(const char *text, uint8_t line, uint8_t lineLength = 20);
    void printCenter(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t width = 20, uint8_t height = 4);
    void printLeft(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t width = 20, uint8_t height = 4);
    void printRight(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t width = 20, uint8_t height = 4);
    void printBottom(const char *text, uint8_t x = 0, uint8_t y = 0, uint8_t width = 20, uint8_t height = 4);
};

void LCDScreen::setup() {
  logStart("Display (LiquidCrystal_I2C 20x4)");

  lcd = new LiquidCrystal_I2C(0x27, 20, 4);

  lcd->init();
  lcd->backlight(); delay(500);
  lcd->noBacklight(); delay(500);
  lcd->backlight();
  
  lcd->cursor();
  lcd->blink();
}

void LCDScreen::loop() {
  static int rowIndex = 0;
  if (Serial.available()) {
    lcd->setCursor(0, rowIndex % 4);
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n') rowIndex++;
      if (c != '\n' && c != '\r')
        lcd->write(c);
    }
  }
}

void LCDScreen::clear() {
  lcd->clear();
}
void LCDScreen::setCursor(uint8_t x, uint8_t y) {
  lcd->setCursor(x, y);
}

void LCDScreen::print(const char *text) {
  lcd->printstr(text);
}

void LCDScreen::printc(char c) {
  lcd->print(c);
}

void LCDScreen::printCenterLine(const char *text, uint8_t line, uint8_t lineLength) {
  uint8_t length = strlen(text);
  lcd->setCursor((lineLength - length) / 2, line);
  lcd->print(text);
}
void LCDScreen::printLeftLine(const char *text, uint8_t line, uint8_t lineLength) {
  uint8_t length = strlen(text);
  lcd->setCursor(0, line);
  lcd->print(text);
}
void LCDScreen::printCenter(const char *text, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
} 
void LCDScreen::printLeft(const char *text, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  lcd->setCursor(x, y);
  lcd->print(text);
}
void LCDScreen::printRight(const char *text, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {

}
void LCDScreen::printBottom(const char *text, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {

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
