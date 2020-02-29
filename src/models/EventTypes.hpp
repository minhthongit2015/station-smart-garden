
#pragma once
#ifndef BEYOND_GARDEN_EVENT_TYPES_H
#define BEYOND_GARDEN_EVENT_TYPES_H

#include <set>
#include <map>
#include <iterator>
#include <string.h>
#include "../utils/Constants.hpp"


#define DEFAULT_PRECISION 100
#define compareFloat(n1, n2, precision) (((int)(n1*precision)) == ((int)(n2*precision)))
#define maxNum(a, b) (a > b ? a : b)
#define findLength(str1, str2) maxNum(strlen(str1), strlen(str2))

union EventData {
  struct {
    float temperature;
    float humidity;
    bool operator == (EventData data) {
      return compareFloat(data.HuTemp.humidity, this->humidity, DEFAULT_PRECISION)
        && compareFloat(data.HuTemp.temperature, this->temperature, DEFAULT_PRECISION);
    }
    bool operator != (EventData data) {
      return !compareFloat(data.HuTemp.humidity, this->humidity, DEFAULT_PRECISION)
        || !compareFloat(data.HuTemp.temperature, this->temperature, DEFAULT_PRECISION);
    }
    bool operator ! () {
      return isnan(this->humidity) || isnan(this->temperature);
    }
  } HuTemp;

  struct {
    uint16_t light;
    bool operator == (EventData data) {
      return data.Light.light == this->light;
    }
    bool operator != (EventData data) {
      return data.Light.light != this->light;
    }
    bool operator ! () {
      return isnan(this->light) || this->light == 54612;
    }
  } Light;

  struct {
    bool moving;
    bool operator == (EventData data) {
      return data.Moving.moving == this->moving;
    }
    bool operator != (EventData data) {
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

  EventData operator = (EventData &data) {
    memcpy(this, &data, sizeof(EventData));
  }

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
    return this->type == ERROR;
  }
};
typedef Event *pEvent;
typedef void (*EventListener)(pEvent event);
// typedef std::function<void (pEvent event)> EventListener;
#define defineListener(name) void name(pEvent event)

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