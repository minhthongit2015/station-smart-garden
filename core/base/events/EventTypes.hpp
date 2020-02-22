
#pragma once
#ifndef BEYOND_GARDEN_EVENT_TYPES_H
#define BEYOND_GARDEN_EVENT_TYPES_H

#include <set>
#include <map>
#include <iterator>


enum EventType {
  ERROR = 1,
  ON_EVENT,
  CONFIGURATION_CHANGE,
  VALUE_CHANGE,
  HUTEMP_CHANGE,
  LIGHT_CHANGE,
  MOVING_CHANGE,
  AFTER_MOVING,
  KEY_DOWN,
  KEY_UP,
  KEY_PRESS,
};

#define DEFAULT_PRECISION 100
#define compareFloat(n1, n2, precision) (((int)(n1*precision)) == ((int)(n2*precision)))

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
    return this->type == VALUE_CHANGE;
  }
};
typedef Event *pEvent;
typedef void (*EventListener)(pEvent event);

typedef std::set<EventListener> ListenerSet;
typedef ListenerSet *pListenerSet;
typedef std::pair<EventType, pListenerSet> ListenerPair;
typedef std::set<EventListener>::iterator ListenerIterator;
typedef std::map<EventType, pListenerSet>::iterator ListenerSetIterator;

Event EMPTY_EVENT;
EventData EMPTY_EVENT_DATA = { { 0 } };

#endif