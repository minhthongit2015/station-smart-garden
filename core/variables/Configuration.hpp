
#pragma once
#ifndef BG_CONFIGURATION_H
#define BG_CONFIGURATION_H

#include <string>
#include <DHT.h>
#include "./pinmap.hpp"
#include "../base/fs.hpp"
#include "../base/types.hpp"

class Config;

typedef void (*configListener)();


#define CHECK_BYTE '\xff'
#define MAIN_LOOP_DELAY_TIME 10

#define CONFIG_FILE_PATH "/config.json"
#define TEMP_CONFIG_FILE_PATH "/config.json.temp"

#define CONFIG_BUFFER_SIZE 384

class Configuration : public Listenable {
  private:
    ListenerSet changeListeners;
  public:
    StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;

    String gardenHost;
    long gardenPort = 0;
    String gardenWifiSignal;
    String gardenWifiPassword;
    String sessionId;

    uint8_t DhtPin = D5;
    uint8_t DhtType = DHT22;
    uint8_t HcSr501Pin = NOT_A_PIN;
    uint8_t TTP229SclPin = NOT_A_PIN;
    uint8_t TTP229SdoPin = NOT_A_PIN;
    
    uint8_t pumpPin = D3;
    uint8_t oxygenPin = NOT_A_PIN;
    uint8_t ledPin = D4;
    uint8_t fanPin = NOT_A_PIN;
    uint8_t mistingPin = NOT_A_PIN;
    uint8_t nutriPin = NOT_A_PIN;

    Configuration() : Listenable() {
      listenersMap.insert(ListenerPair(CONFIGURATION_CHANGE, &changeListeners));
    }

    void onChange(EventListener listener) {
      changeListeners.insert(listener);
    }

    void loadConfigurations();
    bool saveConfigurations();
    void putConfigurations(Stream &stream);

    void toDoc();
    void fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc);
    
    void setup() {
      logStart("Configuration Manager");
      loadConfigurations();
    }
    void loop() {
      if (Serial.available()) {
        putConfigurations(Serial);
      }
    }
} cfg;

extern Configuration cfg;

void Configuration::loadConfigurations() {
  log("Config", "Load Configurations");
  if (fsz.exists(CONFIG_FILE_PATH)) {
    File file = fsz.open(CONFIG_FILE_PATH, "r");
    if (!file) {
      error("Config", "Cannot open config file (" CONFIG_FILE_PATH ")");
    }
    deserializeJson(doc, file);
    fromDoc(doc);
    serializeJsonPretty(doc, Serial); prl();
    file.close();
  } else {
    if (fsz.exists(TEMP_CONFIG_FILE_PATH)) {
      File file = fsz.open(TEMP_CONFIG_FILE_PATH, "r");
      if (!file) {
        error("Config", "Cannot open config file (" TEMP_CONFIG_FILE_PATH ")");
      }
      deserializeJson(doc, file);
      fromDoc(doc);
      serializeJsonPretty(doc, Serial); prl();
      file.close();
    }
  }
}

bool Configuration::saveConfigurations() {
  log("Config", "Save Configurations");
  File file = fsz.open(TEMP_CONFIG_FILE_PATH, "w");
  if (!file) {
    error("Config", "Cannot open config file (" TEMP_CONFIG_FILE_PATH ")");
  }
  toDoc();
  serializeJson(doc, file);
  file.close();

  fsz.remove(CONFIG_FILE_PATH);
  if (!fsz.rename(TEMP_CONFIG_FILE_PATH, CONFIG_FILE_PATH)) {
    error("Config", "Cannot rename " TEMP_CONFIG_FILE_PATH " to " CONFIG_FILE_PATH);
    return false;
  }
  return true;
}

void Configuration::putConfigurations(Stream &stream) {
  log("Config", "Put Configurations");
  deserializeJson(doc, stream);
  fromDoc(doc);
  serializeJsonPretty(doc, stream); prl();
  saveConfigurations();
  dispatch({{0}}, CONFIGURATION_CHANGE);
}

void Configuration::toDoc() {
  doc["gardenHost"] = gardenHost;
  doc["gardenPort"] = gardenPort;
  doc["gardenWifiSignal"] = gardenWifiSignal;
  doc["gardenWifiPassword"] = gardenWifiPassword;
  doc["sessionId"] = sessionId;
}

void Configuration::fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc) {
  if (doc.containsKey("gardenHost")) gardenHost = doc["gardenHost"].as<String>();
  if (doc.containsKey("gardenPort")) gardenPort = doc["gardenPort"].as<long>();
  if (doc.containsKey("gardenWifiSignal")) gardenWifiSignal = doc["gardenWifiSignal"].as<String>();
  if (doc.containsKey("gardenWifiPassword")) gardenWifiPassword = doc["gardenWifiPassword"].as<String>();
  if (doc.containsKey("sessionId")) sessionId = doc["sessionId"].as<String>();
}

#endif
