
#pragma once
#ifndef BEYOND_GARDEN_STATION_STATE_H
#define BEYOND_GARDEN_STATION_STATE_H

#include <ArduinoJson.h>

#define STATE_BUFFER_SIZE 384

#define toBool(val) (val ? "true" : "false")

class StationState {
  private:
    char outputBuffer[STATE_BUFFER_SIZE] = {0};
  public:
    StaticJsonDocument<STATE_BUFFER_SIZE> doc;

    float temperature = 0;
    float humidity = 0;
    uint16_t light = 0;
    bool moving = 0;

    bool pump = false;
    bool oxygen = false;
    bool led = false;
    bool fan = false;
    bool misting = false;
    long nutri = 0;

    char *toJSON();

    void fromDoc(StaticJsonDocument<STATE_BUFFER_SIZE> &doc) {
      if (doc.containsKey("pump")) pump = doc["pump"].as<bool>();
      if (doc.containsKey("oxygen")) oxygen = doc["oxygen"].as<bool>();
      if (doc.containsKey("led")) led = doc["led"].as<bool>();
      if (doc.containsKey("fan")) fan = doc["fan"].as<bool>();
      if (doc.containsKey("misting")) misting = doc["misting"].as<bool>();
      if (doc.containsKey("nutri")) nutri = doc["nutri"].as<long>();
    }
} state;

extern StationState state;

char *StationState::toJSON() {
  JsonObject obj = doc.to<JsonObject>();
  obj["temperature"] = temperature;
  obj["humidity"] = humidity;
  obj["light"] = light;
  obj["moving"] = moving;
  obj["pump"] = pump;
  obj["oxygen"] = oxygen;
  obj["led"] = led;
  obj["fan"] = fan;
  obj["misting"] = misting;
  obj["nutri"] = nutri;
  serializeJson(obj, outputBuffer);
  return outputBuffer;
}

#endif
