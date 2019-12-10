

#pragma once
#ifndef SMART_GARDEN_STATION_STATE_H
#define SMART_GARDEN_STATION_STATE_H

#include <ArduinoJson.h>

#define toBool(val) (val ? "true" : "false")

class StationState {
  private:
    static char outputBuffer[256];
  public:
    static StaticJsonDocument<256> doc;
    
    static float temperature;
    static float humidity;
    static uint16_t light;
    static bool moving;

    static bool pump;
    static bool oxygen;
    static bool led;
    static bool fan;
    static bool misting;
    static long nutri;

    static char* toJSON();
};

StaticJsonDocument<256> StationState::doc;
char StationState::outputBuffer[256] = {0};

float StationState::temperature = 0;
float StationState::humidity = 0;
uint16_t StationState::light = 0;
bool StationState::moving = 0;

bool StationState::pump = 0;
bool StationState::oxygen = 0;
bool StationState::led = 0;
bool StationState::fan = 0;
bool StationState::misting = 0;
long StationState::nutri = 0;


char* StationState::toJSON() {
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
