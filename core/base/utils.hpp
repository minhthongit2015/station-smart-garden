

#pragma once
#ifndef SMART_GARDEN_UTILS_H
#define SMART_GARDEN_UTILS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#include "../variables/global.hpp"
#include "./list.hpp"

const String GARDEN_SIGNAL_STRING = String(GARDEN_SIGNAL);

/*           LED           */
#define led1 2
#define led2 16
#define useIn(pin) pinMode(led1, INPUT);
#define useOut(pin) pinMode(led1, OUTPUT);
#define setupLed useOut(led1); useOut(led2);

#define onLed1 digitalWrite(led1, 0);
#define offLed1 digitalWrite(led1, 1);
#define onLed2 digitalWrite(led2, 0);
#define offLed2 digitalWrite(led2, 1);
#define then

#define sleep(sec) delay(sec*1000);
#define d(ms) delay(ms);

/*           Print           */
#define pr(...) Serial.print(__VA_ARGS__);
#define prl(...) Serial.println(__VA_ARGS__);
#define prf(...) Serial.printf(__VA_ARGS__);
#define log(moduleName, message) { pr("[" moduleName "] > "); prl(message); }

/*           Setup           */
void helperSetup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  prl() prl() prl()
  for (uint8_t t = 4; t > 0; --t) {
    prf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  EEPROM.begin(512);
}


/*           Reset           */
void (*_reset)(void) = 0;
void reset() {
  prl("\r\n\r\n-----------------------------------");
  prl("[Sys] </> Restarting....");
  prl("-----------------------------------\r\n");
  _reset();
}


/*************** EEPROM **************/
// template<class T> int write(int pos, const T &value) {
//   const byte *p = (const byte*)&T;
//   EEPROM.write(addr, val);
// }
int writeEEPROM(String &value, int pos=0) {
  EEPROM.write(pos++, CHECK_BYTE); // check byte

  byte *p = (byte*)(value.c_str());
  int end = pos + value.length();
  for (register int i = pos; i < end; ++i) {
    EEPROM.write(i, *p++);
  }
}

String readEEPROM(int pos = 0) {
  String rs;
  if (EEPROM.read(pos++) != CHECK_BYTE) return rs;

  char c;
  while (c = EEPROM.read(pos)) {
    if (!isAlphaNumeric(c)) break;
    rs += c;
  }
  return rs;
}

bool checkGardenSignal(String ssid) {
  return ssid.indexOf(GARDEN_SIGNAL) >= 0;
}

#endif
