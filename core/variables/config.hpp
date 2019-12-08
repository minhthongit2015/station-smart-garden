
#pragma once
#ifndef SMART_STATION_CONFIG_H
#define SMART_STATION_CONFIG_H

#include <string>
#include <DHT.h>
#include <set>
#include "../base/fs.hpp"
#include "./pinmap.hpp"

class Config;

typedef void (*configListener)();

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

#define CONFIG_BUFFER_SIZE 256

class Config {
  private:
    static std::set<configListener> _onChange;
  public:
    static void onChange(configListener callback);
    static void dispatchEvent();

    static StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;
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
    
    static uint8_t pumpPin;
    static uint8_t ledPin;
    static uint8_t fanPin;
    static uint8_t mistingPin;
    static uint8_t nutriPin;

    static void setup(FileSystem &pfs);
    static void loadConfigurations();
    static bool saveConfigurations();
    static void putConfigurations(Stream &stream);

    static void toDoc();
    static void fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc);
};

std::set<configListener> Config::_onChange;
StaticJsonDocument<CONFIG_BUFFER_SIZE> Config::doc;
pFileSystem Config::fs;

String Config::gardenHost = "";
long Config::gardenPort = 0;
String Config::gardenWifiSignal = "";
String Config::gardenWifiPassword = "";

uint8_t Config::pumpPin = D7;
uint8_t Config::ledPin = D8;
uint8_t Config::fanPin = SD2;
uint8_t Config::mistingPin = SD2;
uint8_t Config::nutriPin = SD3;

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
    fromDoc(doc);
    serializeJsonPretty(doc, Serial); prl();
    file.close();
  }
}

bool Config::saveConfigurations() {
  logz("Config", "Save Configurations");
  File file = fs->fs.open(TEMP_CONFIG_FILE_PATH, "w");
  if (!file) {
    error("Config", "Cannot open config file (" TEMP_CONFIG_FILE_PATH ")");
  }
  toDoc();
  serializeJson(doc, file);
  file.close();

  fs->fs.remove(CONFIG_FILE_PATH);
  if (!fs->fs.rename(TEMP_CONFIG_FILE_PATH, CONFIG_FILE_PATH)) {
    error("Config", "Cannot rename " TEMP_CONFIG_FILE_PATH " to " CONFIG_FILE_PATH);
    return false;
  }
  return true;
}

void Config::putConfigurations(Stream &stream) {
  logz("Config", "Put Configurations");
  deserializeJson(doc, stream);
  fromDoc(doc);
  serializeJsonPretty(doc, stream); prl();
  saveConfigurations();
  dispatchEvent();
}

void Config::toDoc() {
  doc["gardenHost"] = gardenHost;
  doc["gardenPort"] = gardenPort;
  doc["gardenWifiSignal"] = gardenWifiSignal;
  doc["gardenWifiPassword"] = gardenWifiPassword;
}

void Config::fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc) {
  if (doc.containsKey("gardenHost")) gardenHost = doc["gardenHost"].as<String>();
  if (doc.containsKey("gardenPort")) gardenPort = doc["gardenPort"].as<long>();
  if (doc.containsKey("gardenWifiSignal")) gardenWifiSignal = doc["gardenWifiSignal"].as<String>();
  if (doc.containsKey("gardenWifiPassword")) gardenWifiPassword = doc["gardenWifiPassword"].as<String>();
}

void Config::onChange(configListener callback) {
  _onChange.insert(callback);
}

void Config::dispatchEvent() {
	for (std::set<configListener>::iterator p = _onChange.begin(); p != _onChange.end(); ++p) {
    (*p)();
  }
}

#endif
