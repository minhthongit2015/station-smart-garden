

#pragma once
#ifndef SMART_GARDEN_STATION_STATE_H
#define SMART_GARDEN_STATION_STATE_H

#include <string.h>

struct StationState {
  float temperature;
  float humidity;
  uint16_t light;
  bool moving;

  bool pump;
  bool led;
  bool fan;
  bool misting;
  long nutri; // milli second to open valve

  char* toJSON() {
    static char buf[300] = "";
    sprintf(buf, 
      "{"
        "\"temperature\": %f,"
        "\"humidity\": %f,"
        "\"light\": %f,"
        "\"moving\": %s,"
        "\"pump\": %s,"
        "\"led\": %s,"
        "\"fan\": %s,"
        "\"misting\": %s,"
        "\"nutri\": %d"
      "}",
      temperature, humidity, light,
      moving ? "true" : "false",
      pump ? "true" : "false",
      led ? "true" : "false",
      fan ? "true" : "false",
      misting ? "true" : "false",
      nutri);
    return buf;
  }
};

#endif
