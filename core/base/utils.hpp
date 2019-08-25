

#pragma once
#ifndef SMART_GARDEN_UTILS_H
#define SMART_GARDEN_UTILS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Wire.h>

#include "../variables/pinmap.hpp"

#define DEVELOPMENT

/*           LED           */
#define led1 2
#define led2 16
#define useIn(pin) pinMode(pin, INPUT);
#define useOut(pin) pinMode(pin, OUTPUT);
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
#define logz(moduleName, message) { pr("> [" moduleName "] ") prl(message) }
#define error(moduleName, errorMessage) { pr("<!> [" moduleName "] ") prl(errorMessage) }
#define logStart(moduleName) prl("<*> >>> " moduleName)

static bool logChannels[10] = {true};
#define toggleLogChannel(channel, state) logChannels[channel] = state;


/*         Performance       */
static unsigned long perf_last[10] = {0};
static bool perf_disable[10] = {false};
#define performance2(action, channel) {\
  if (!perf_disable[channel]) {\
    prf("[%d] +%d ms -> %s\r\n", channel, millis() - perf_last[channel], action);\
    perf_last[channel] = millis();\
  }\
}
#define performance(action) performance2(action, 0);
#define performanceDisable(channel) perf_disable[channel] = true;

/*           Setup           */
void helperSetup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  prl("\r\n\r\n");
  for (uint8_t t = 4; t > 0; --t) {
    prf("[BOOT] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  EEPROM.begin(512);
}


/*           Reset           */
void (*_reset)(void) = 0;
void reset() {
  prl("\r\n\r\n-----------------------------------");
  prl("</> [Sys] Resetting....");
  prl("-----------------------------------\r\n");
  _reset();
}


/*           i2c Scanner           */
void i2cScanner() {
  prf("\r\n<*> [I2C] Start scanning I2C (SCL: %d, SDA: %d)\r\n", SCL, SDA);
  Wire.begin(SDA, SCL);

  byte error, address;
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      pr("> [I2C] Found: 0x");
      if (address < 16) pr("0"); prl(address, HEX);
    } else if (error == 4) {
      pr("> [I2C] Unknown Error: 0x");
      if (address < 16) pr("0"); prl(address, HEX);
    }
  }
  prl("</> [I2C] Scanning I2C done!\r\n");
}


#endif
