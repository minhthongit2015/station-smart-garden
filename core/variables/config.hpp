
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

#define CONFIG_FILE_PATH "/config.json"
#define TEMP_CONFIG_FILE_PATH "/config.json.temp"

class Config {
  public:
    static StaticJsonDocument<256> doc;
    static pFileSystem fs;
    static String gardenHost;
    static long gardenPort;
    static String gardenWifiSignal;
    static String gardenWifiPassword;

    static uint8_t DhtPin;
    static uint8_t DhtType;
    static uint8_t HcSr501Pin;
    static uint8_t TTP229SclPin;
    static uint8_t TTP229SdoPin;

    static void setup(FileSystem &pfs);
    static void loadConfigurations();
    static void saveConfigurations();

    static void toDoc();
    static void fromDoc(StaticJsonDocument<256> &doc);
};

StaticJsonDocument<256> Config::doc;
pFileSystem Config::fs;

String Config::gardenHost = "192.168.1.12";
long Config::gardenPort = 4000;
String Config::gardenWifiSignal = "moidoiten";
String Config::gardenWifiPassword = "passla123";

uint8_t Config::DhtPin = D6;
uint8_t Config::DhtType = DHT22;
uint8_t Config::HcSr501Pin = D5;
uint8_t Config::TTP229SclPin = D4;
uint8_t Config::TTP229SdoPin = D3;

void Config::setup(FileSystem &pfs) {
  logStart("Config");
  fs = &pfs;
}

void Config::loadConfigurations() {
  logz("Config", "Load Configurations");
  if (fs->fs.exists(CONFIG_FILE_PATH)) {
    File file = fs->fs.open(CONFIG_FILE_PATH, "r");
    if (!file) {
      error("Config", "Cannot open config file (" CONFIG_FILE_PATH ")");
    }
    deserializeJson(doc, file);
    serializeJsonPretty(doc, Serial); prl();
    file.close();
  }
}

void Config::saveConfigurations() {
  logz("Config", "Save Configurations");
  File file = fs->fs.open(TEMP_CONFIG_FILE_PATH, "w");
  if (!file) {
    error("Config", "Cannot open config file (" TEMP_CONFIG_FILE_PATH ")");
  }
  toDoc();
  serializeJson(doc, file);
  fs->fs.rename(TEMP_CONFIG_FILE_PATH, CONFIG_FILE_PATH);
  file.close();
}


void Config::toDoc() {
  doc["gardenHost"] = gardenHost;
  doc["gardenPort"] = gardenPort;
  doc["gardenWifiSignal"] = gardenWifiSignal;
  doc["gardenWifiPassword"] = gardenWifiPassword;
}

void Config::fromDoc(StaticJsonDocument<256> &doc) {
  gardenHost = doc["gardenHost"].as<String>();
  gardenPort = doc["gardenPort"].as<long>();
  gardenWifiSignal = doc["gardenWifiSignal"].as<String>();
  gardenWifiPassword = doc["gardenWifiPassword"].as<String>();
}

#endif
