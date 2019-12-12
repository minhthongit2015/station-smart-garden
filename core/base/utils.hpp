

#pragma once
#ifndef BEYOND_GARDEN_UTILS_H
#define BEYOND_GARDEN_UTILS_H

#define ARDUINO 10809

#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <HardwareSerial.h>

#include "../variables/pinmap.hpp"
#include "../variables/constants.hpp"

#define ENV_PROD


/*           LED           */
#define useIn(pin) pinMode(pin, INPUT);
#define useOut(pin) pinMode(pin, OUTPUT);

#define onRelay(pin) digitalWrite(pin, LOW);
#define offRelay(pin) digitalWrite(pin, HIGH);

#define useOutOn(pin) { useOut(pin); onRelay(pin); }
#define useOutOff(pin) { useOut(pin); offRelay(pin); }

#define setupLed useOut(LED1); useOut(led2);
#define onLed1 digitalWrite(LED1, 0);
#define offLed1 digitalWrite(LED1, 1);
#define onLed2 digitalWrite(LED2, 0);
#define offLed2 digitalWrite(LED2, 1);
#define then

#define sleep(sec) delay(sec*1000);
#define d(ms) delay(ms);

/*           Print           */
#define pr(...) Serial.print(__VA_ARGS__);
#define prl(...) Serial.println(__VA_ARGS__);
#define prf(...) Serial.printf(__VA_ARGS__);
#define log(moduleName, message) { pr("> [" moduleName "] ") prl(message) }
#define logBlock(moduleName, message) { pr("> [" moduleName "] ") prl(message ":") }
#define error(moduleName, errorMessage) { pr("<!> [" moduleName "] ") prl(errorMessage) }
#define logStart(moduleName) prl("<*> >>> " moduleName)
#define logEnd(moduleName) prl("</> <<< " moduleName)

static bool logChannels[10] = {true};
#define toggleLogChannel(channel, state) logChannels[channel] = state;


/*         Performance       */
static unsigned long perfLast[10] = {0};
static bool perfChannels[10] = {true};
#define performance2(action, channel) {\
  if (perfChannels[channel]) {\
    prf("[%d] +%d ms -> %s\r\n", channel, millis() - perfLast[channel], action);\
    perfLast[channel] = millis();\
  }\
}
#define performance(action) performance2(action, 0);
#define togglePerformanceChannel(channel, state) perfChannels[channel] = state;

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
  prf("\r\n<*> [I2C] Start scanning I2C (SCL: %d, SDA: %d):\r\n", SCL, SDA);
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
