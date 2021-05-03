
#pragma once
#ifndef BEYOND_GARDEN_EVENT_TYPES_H
#define BEYOND_GARDEN_EVENT_TYPES_H

#include <Arduino.h>
#include <set>
#include <map>
#include <iterator>
#include <string.h>
#include "../utils/Constants.hpp"


#define DEFAULT_PRECISION 0.01 // đúng tới 2 chữ số sau dấu .
#define abs(a, b) (a - b > 0 ? a - b : b - a)
#define isEqualz(n1, n2, precision) (abs(n1, n2) < precision)
#define isEqual(n1, n2) isEqualz(n1, n2, DEFAULT_PRECISION)
#define maxNum(a, b) (a > b ? a : b)
#define findLength(str1, str2) maxNum(strlen(str1), strlen(str2))

union EventData {
  struct {
    float temperature;
    float humidity;
    bool operator == (EventData &data) {
      return ((!!data.HuTemp || !!(*this)) && !(!data.HuTemp ^ !(*this)))
        && isEqual(data.HuTemp.humidity, this->humidity)
        && isEqual(data.HuTemp.temperature, this->temperature);
    }
    bool operator != (EventData &data) {
      return !((*this) == data);
    }
    bool operator ! () {
      return isnan(this->humidity) || isnan(this->temperature);
    }
  } HuTemp;

  struct {
    uint16_t light;
    bool operator == (EventData &data) {
      return data.Light.light == this->light;
    }
    bool operator != (EventData &data) {
      return data.Light.light != this->light;
    }
    bool operator ! () {
      return isnan(this->light) || this->light == 54612;
    }
    bool operator = (uint16_t light) {
      return this->light = (!isnan(light) && light != 54612) ? light : 0;
    }
  } Light;

  struct {
    float soilMoisture;
    bool operator == (EventData &data) {
      return ((!!data.SoilMoisture || !!(*this)) && !(!data.SoilMoisture ^ !(*this)))
        && isEqual(data.SoilMoisture.soilMoisture, this->soilMoisture);
    }
    bool operator != (EventData &data) {
      return !((*this) == data);
    }
    bool operator ! () {
      return this->soilMoisture < 0 || this->soilMoisture > 100;
    }
    bool operator = (float soilMoisture) {
      return this->soilMoisture = soilMoisture;
    }
  } SoilMoisture;

  struct {
    bool moving;
    bool operator == (EventData &data) {
      return data.Moving.moving == this->moving;
    }
    bool operator != (EventData &data) {
      return data.Moving.moving != this->moving;
    }
    bool operator ! () {
      return isnan(this->moving) || !this->moving;
    }
  } Moving;
  
  struct {
    uint8_t key;
    bool operator == (EventData data) {
      return data.Key.key == this->key;
    }
    bool operator != (EventData data) {
      return data.Key.key != this->key;
    }
    bool operator ! () {
      return this->key < 0 || this->key >= 16;
    }
  } Key;
  
  struct {
    const char *payload;
    unsigned long length;
    bool operator == (EventData data) {
      return memcmp(data.Payload.payload, this->payload, maxNum(data.Payload.length, this->length)) == 0;
    }
    bool operator != (EventData data) {
      return memcmp(data.Payload.payload, this->payload, maxNum(data.Payload.length, this->length)) != 0;
    }
    bool operator ! () {
      return !this->payload || this->length <= 0;
    }
  } Payload;

  // EventData& operator = (EventData &data) {
  //   memcpy(this, &data, sizeof(EventData));
  //   return *this;
  // }

  bool operator == (EventData &data) {
    return memcmp(this, &data, sizeof(EventData)) == 0;
  }

  bool operator != (EventData &data) {
    return memcmp(this, &data, sizeof(EventData)) != 0;
  }
};

typedef EventData *pEventData;
struct Event {
  EventType type;
  EventData data;
  bool isError() {
    return this->type == ERROR_EVENT;
  }
};
typedef Event *pEvent;
typedef void (*EventListener)(pEvent event);
// typedef std::function<void (pEvent event)> EventListener;
#define declareListener(name) void name(pEvent event)

typedef std::set<EventListener> ListenerSet;
typedef ListenerSet *pListenerSet;
typedef std::set<EventListener>::iterator ListenerIterator;

typedef std::map<EventType, pListenerSet> ListenerMap;
typedef ListenerMap *pListenerMap;
typedef std::pair<EventType, pListenerSet> ListenerEntry;
typedef ListenerMap::iterator ListenerMapIterator;

EventData EMPTY_EVENT_DATA = { { 0 } };
Event EMPTY_EVENT = { NONE, EMPTY_EVENT_DATA };

#endif