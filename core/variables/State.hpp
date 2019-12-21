
#pragma once
#ifndef BEYOND_GARDEN_STATION_STATE_H
#define BEYOND_GARDEN_STATION_STATE_H

#include <ArduinoJson.h>

#define toBool(val) (val ? "true" : "false")

class StationState {
  private:
    char outputBuffer[256] = {0};
  public:
    StaticJsonDocument<256> doc;

    float temperature = 0;
    float humidity = 0;
    uint16_t light = 0;
    bool moving = 0;

    bool pump = 0;
    bool oxygen = 0;
    bool led = 0;
    bool fan = 0;
    bool misting = 0;
    long nutri = 0;

    char *toJSON();
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
