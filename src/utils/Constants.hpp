
#pragma once
#ifndef BEYOND_GARDEN_CONSTANTS_H
#define BEYOND_GARDEN_CONSTANTS_H

#define POST "POST"
#define GET "GET"
#define PUT "PUT"
#define PATCH "PATCH"
#define DELETE "DELETE"

#define ApiEndpoint "/api/v1"
#define GardenEndpoint ApiEndpoint "/garden"
#define GardensEndpoint GardenEndpoint "/gardens"
#define StationsEndPoint GardenEndpoint "/stations"
#define RecordsEndpoint GardenEndpoint "/records"
#define AuthorizeStationEndpoint StationsEndPoint "/verify"


enum EventType {
  NONE = 0,
  ERROR_EVENT = 1,
  DEFAULT_EVENT,
  CONFIGURATION_CHANGE,
  VALUE_CHANGE,
  HUTEMP_CHANGE,
  LIGHT_CHANGE,
  MOVING_CHANGE,
  AFTER_MOVING,
  KEY_DOWN,
  KEY_UP,
  KEY_PRESS,
  CONNECTED,
  DISCONNECTED
};

enum TimerIdType {
  DEFAULT_TIMER = 0,
  WS_DISCONNECTED,
  WS_CHECK_ON_SLEEP,
  MODULE_HRSR501_AFTER_MOVING
};

struct {
  const char *CONNECT;
  const char *DISCONNECT;
  const char *AUTHORIZED;
  const char *UNAUTHORIZED;
  const char *SET_STATE;
} WSEvent = {
  "connect",
  "disconnect",
  "authorized",
  "unauthorized",
  "setState"
};

enum TextAlign {
  LEFT = 1,
  RIGHT,
  CENTER
};

#endif
