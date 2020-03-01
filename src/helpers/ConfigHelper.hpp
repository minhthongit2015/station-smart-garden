
#pragma once
#ifndef BEYOND_GARDEN_CONFIG_HELPER_H
#define BEYOND_GARDEN_CONFIG_HELPER_H

#include <string>
#include <DHT.h>
#include "../utils/Utils.hpp"
#include "../utils/fs.hpp"
#include "../configs/Common.hpp"
#include "../libs/Listenable.hpp"

String BLANK_STRING = "";

#define CONFIG_HELPER "Config Helper"

#define returnIfNotContains(key, value) if (!doc.containsKey(key)) { log("Config not found", key); return value; }
#define returnNull(key) returnIfNotContains(key, NULL)
#define returnNegative(key) returnIfNotContains(key, -1)
#define returnBlank(key) returnIfNotContains(key, BLANK_STRING)

struct ConfigManager : Listenable {
  StaticJsonDocument<CONFIG_BUFFER_SIZE> doc;

  ConfigManager() : Listenable() {
    defineEvent(CONFIGURATION_CHANGE);
  }

  void onChange(EventListener listener) {
    onEvent(CONFIGURATION_CHANGE, listener);
  }

  bool loadConfigurations();
  bool loadConfigurations(const char* filePath);
  bool save();
  void putConfigurations(Stream &stream);
  void dumpConfigurations();

  void toDoc();
  void fromDoc(StaticJsonDocument<CONFIG_BUFFER_SIZE> &doc);

  template<typename Type, typename KeyType>
  ConfigManager &set(KeyType key, Type value) {
    doc[key] = value;
    return *this;
  }

  template<typename Type, typename KeyType>
  ConfigManager &setDefault(KeyType key, Type value) {
    if (!doc.containsKey(key)) {
      logf(CONFIG_HELPER, "Set default \"%s\" -> ", key); prl(value);
      doc[key] = value;
    }
    return *this;
  }

  template<typename Type>
  Type get(const char *key) {
    return doc[key].as<Type>();
  }

  String getStr(const char *key) {
    returnBlank(key);
    return doc[key].as<String>();
  }

  const char* getCStr(const char *key) {
    returnNull(key);
    return doc[key].as<const char *>();
  }

  char* getCStrz(const char *key) {
    returnNull(key);
    return (char*)getCStr(key);
  }
  
  uint8_t getUInt8(const char *key) {
    returnNegative(key);
    return doc[key].as<uint8_t>();
  }
  
  long getLong(const char *key) {
    returnNegative(key);
    return doc[key].as<long>();
  }
  
  void setup() {
    logStart(CONFIG_HELPER);
    loadConfigurations();
    Serial.flush();
  }

  void loop() {
    if (Serial.available()) {
      putConfigurations(Serial);
    }
  }
} cfg;

extern ConfigManager cfg;

bool ConfigManager::loadConfigurations() {
  log(CONFIG_HELPER, "Load Configurations");
  if (fsz.exists(CONFIG_FILE_PATH)) {
    if (loadConfigurations(CONFIG_FILE_PATH)) {
      return true;
    } else {
      error(CONFIG_HELPER, "Cannot open config file (" CONFIG_FILE_PATH ")");
    }
  }
  if (fsz.exists(CONFIG_TEMP_FILE_PATH)) {
    if (loadConfigurations(CONFIG_TEMP_FILE_PATH)) {
      fsz.rename(CONFIG_TEMP_FILE_PATH, CONFIG_FILE_PATH);
      return true;
    } else {
      error(CONFIG_HELPER, "Cannot open config temporary file (" CONFIG_TEMP_FILE_PATH ")");
    }
  }
  if (fsz.exists(CONFIG_BACKUP_FILE_PATH)) {
    if (loadConfigurations(CONFIG_BACKUP_FILE_PATH)) {
      fsz.rename(CONFIG_BACKUP_FILE_PATH, CONFIG_FILE_PATH);
      return true;
    } else {
      error(CONFIG_HELPER, "Cannot open config backup file (" CONFIG_BACKUP_FILE_PATH ")");
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

bool ConfigManager::save() {
  log(CONFIG_HELPER, "Save Configurations");
  File file = fsz.open(CONFIG_TEMP_FILE_PATH, "w");
  if (!file) {
    error(CONFIG_HELPER, "Cannot open config file (" CONFIG_TEMP_FILE_PATH ")");
    return false;
  }
  serializeJson(doc, file);
  file.close();

  fsz.remove(CONFIG_BACKUP_FILE_PATH);
  fsz.rename(CONFIG_FILE_PATH, CONFIG_BACKUP_FILE_PATH);
  if (!fsz.rename(CONFIG_TEMP_FILE_PATH, CONFIG_FILE_PATH)) {
    error(CONFIG_HELPER, "Cannot rename " CONFIG_TEMP_FILE_PATH " to " CONFIG_FILE_PATH);
    return false;
  }
  return true;
}

void ConfigManager::putConfigurations(Stream &stream) {
  static StaticJsonDocument<CONFIG_BUFFER_SIZE> newDoc;
  deserializeJson(newDoc, stream);
  if (newDoc.isNull()) {
    return;
  }
  log(CONFIG_HELPER, "Put Configurations");
  serializeJsonPretty(newDoc, Serial); prl();

  JsonObject root = newDoc.as<JsonObject>();
  for (JsonPair entry : root) {
    doc[entry.key().c_str()] = newDoc[entry.key().c_str()];
  }

  save();
  log(CONFIG_HELPER, "Saved Configurations");
  dumpConfigurations();
  dispatch(CONFIGURATION_CHANGE);
}

void ConfigManager::dumpConfigurations() {
  #ifdef ENV_DEV
  serializeJsonPretty(doc, Serial); prl();
  #endif
}

#endif
