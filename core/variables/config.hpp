
#pragma once
#ifndef SMART_STATION_CONFIG_H
#define SMART_STATION_CONFIG_H

#include <string>
#include <DHT.h>
#include "../base/fs.hpp"

#define ARDUINO 10809


#define DEVICE_ID "Alpha One"
#define DEVICE_INFO "\
{\
  \"id\": \"A1-01\", \
  \"role\": \"station\", \
  \"name\": \"Alpha Once\", \
  \"type\": \"A1\", \
  \"env_type\": [\"hydroponics\"], \
  \"roles\": [\"pump\", \"misting\", \"sensors\"], \
  \"physical_address\": [\"STA_A1_01\"], \
  \"secret_key\": \"Secret_STA_A1_01\"\
}"

#define CHECK_BYTE '\xff'
#define LOOP_DELAY_TIME 10

class Config {
  public:
    static String gardenHost;
    static long gardenPort;
    static String gardenWifiSignal;
    static String gardenWifiPassword;

    static uint8_t DhtPin;
    static uint8_t DhtType;
    static uint8_t HcSr501Pin;
    static uint8_t TTP229SclPin;
    static uint8_t TTP229SdoPin;

    static void loadConfig(FileSystem &fsz);
};

String Config::gardenHost = "192.168.1.12";
long Config::gardenPort = 4000;
String Config::gardenWifiSignal = "moidoiten";
String Config::gardenWifiPassword = "passla123";

uint8_t Config::DhtPin = D6;
uint8_t Config::DhtType = DHT22;
uint8_t Config::HcSr501Pin = D5;
uint8_t Config::TTP229SclPin = D4;
uint8_t Config::TTP229SdoPin = D3;

void Config::loadConfig(FileSystem &fsz) {
  if (fsz.fs.exists("/config.json")) {

  }
}

#endif
