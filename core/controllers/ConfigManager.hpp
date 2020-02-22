
#pragma once
#ifndef BEYOND_GARDEN_CONFIG_MANAGER_H
#define BEYOND_GARDEN_CONFIG_MANAGER_H

#include <string>
#include <DHT.h>
#include "../base/utils.hpp"
#include "../base/fs.hpp"
#include "../base/types.hpp"
#include "../variables/Configs.hpp"

#define CONFIG_BUFFER_SIZE 384

class ConfigManager : public Listenable {
  private:
    ListenerSet changeListeners;
  public:
    StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;
    
    uint8_t pumpPin = D3;
    uint8_t oxygenPin = NOT_A_PIN;
    uint8_t ledPin = D4;
    uint8_t fanPin = NOT_A_PIN;
    uint8_t mistingPin = NOT_A_PIN;
    uint8_t nutriPin = NOT_A_PIN;

    ConfigManager() : Listenable() {
      listenersMap.insert(ListenerPair(CONFIGURATION_CHANGE, &changeListeners));
    }

    void onChange(EventListener listener) {
      changeListeners.insert(listener);
    }

    bool loadConfigurations();
    bool loadConfigurations(const char* filePath);
    bool saveConfigurations();
    void putConfigurations(Stream &stream);
    void dumpConfigurations();

    void toDoc();
    void fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc);

    template<typename Type, typename KeyType>
    void set(KeyType key, Type value) {
      doc[key] = value;
    }
    template<typename Type, typename KeyType>
    void setDefault(KeyType key, Type value) {
      if (!doc.containsKey(key)) {
        doc[key] = value;
      }
    }

    template<typename Type>
    Type get(char *key) {
      return doc[key].as<Type>();
    }

    String getStr(char *key) {
      return doc[key].as<String>();
    }

    const char* getCStr(char *key) {
      return doc[key].as<String>().c_str();
    }
    
    uint8_t getUInt8(char *key) {
      return doc[key].as<uint8_t>();
    }
    
    long getLong(char *key) {
      return doc[key].as<long>();
    }
    
    void setup() {
      logStart("Configurations Manager");
      loadConfigurations();
    }

    void loop() {
      if (Serial.available()) {
        putConfigurations(Serial);
      }
    }
} cfg;

extern ConfigManager cfg;

bool ConfigManager::loadConfigurations() {
  log("Config", "Load Configurations");
  if (fsz.exists(CONFIG_FILE_PATH)) {
    if (loadConfigurations(CONFIG_FILE_PATH)) {
      return true;
    } else {
      error("Config", "Cannot open config file (" CONFIG_FILE_PATH ")");
    }
  }
  if (fsz.exists(CONFIG_TEMP_FILE_PATH)) {
    if (loadConfigurations(CONFIG_TEMP_FILE_PATH)) {
      fsz.rename(CONFIG_TEMP_FILE_PATH, CONFIG_FILE_PATH);
      return true;
    } else {
      error("Config", "Cannot open config temporary file (" CONFIG_TEMP_FILE_PATH ")");
    }
  }
  if (fsz.exists(CONFIG_BACKUP_FILE_PATH)) {
    if (loadConfigurations(CONFIG_BACKUP_FILE_PATH)) {
      fsz.rename(CONFIG_BACKUP_FILE_PATH, CONFIG_FILE_PATH);
      return true;
    } else {
      error("Config", "Cannot open config backup file (" CONFIG_BACKUP_FILE_PATH ")");
    }
  }
}

bool ConfigManager::loadConfigurations(const char* filePath) {
  if (!fsz.exists(filePath)) {
    return false;
  }
  File file = fsz.open(filePath, "r");
  if (!file) {
    return false;
  }
  deserializeJson(doc, file);
  file.close();
  dumpConfigurations();
  return true;
}

bool ConfigManager::saveConfigurations() {
  log("Config", "Save Configurations");
  File file = fsz.open(CONFIG_TEMP_FILE_PATH, "w");
  if (!file) {
    error("Config", "Cannot open config file (" CONFIG_TEMP_FILE_PATH ")");
    return false;
  }
  serializeJson(doc, file);
  file.close();

  fsz.remove(CONFIG_BACKUP_FILE_PATH);
  fsz.rename(CONFIG_FILE_PATH, CONFIG_BACKUP_FILE_PATH);
  if (!fsz.rename(CONFIG_TEMP_FILE_PATH, CONFIG_FILE_PATH)) {
    error("Config", "Cannot rename " CONFIG_TEMP_FILE_PATH " to " CONFIG_FILE_PATH);
    return false;
  }
  return true;
}

void ConfigManager::putConfigurations(Stream &stream) {
  log("Config", "Put Configurations");
  static StaticJsonDocument<CONFIG_BUFFER_SIZE> newDoc;
  deserializeJson(newDoc, stream);

  JsonObject root = newDoc.as<JsonObject>();
  for (JsonPair entry : root) {
    doc[entry.key().c_str()] = entry.value();
  }

  saveConfigurations();
  dumpConfigurations();
  dispatch(CONFIGURATION_CHANGE);
}

void ConfigManager::dumpConfigurations() {
  #ifdef ENV_DEV
  serializeJsonPretty(doc, Serial); prl();
  #endif
}

#endif
