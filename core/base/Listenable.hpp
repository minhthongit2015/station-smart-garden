
#pragma once
#ifndef BEYOND_GARDEN_LISTENABLE_H
#define BEYOND_GARDEN_LISTENABLE_H

#include <set>
#include <map>
#include <iterator>

#define DEFAULT_PRECISION 100
#define compareFloat(n1, n2, precision) (((int)(n1*precision)) == ((int)(n2*precision)))

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
union Data {
  struct {
    float temperature;
    float humidity;
    bool operator == (Data data) {
      return compareFloat(data.HuTemp.humidity, this->humidity, DEFAULT_PRECISION)
        && compareFloat(data.HuTemp.temperature, this->temperature, DEFAULT_PRECISION);
    }
    bool operator != (Data data) {
      return !compareFloat(data.HuTemp.humidity, this->humidity, DEFAULT_PRECISION)
        || !compareFloat(data.HuTemp.temperature, this->temperature, DEFAULT_PRECISION);
    }
    bool operator ! () {
      return isnan(this->humidity) || isnan(this->temperature);
    }
  } HuTemp;
  struct {
    uint16_t light;
    bool operator == (Data data) {
      return data.Light.light == this->light;
    }
    bool operator != (Data data) {
      return data.Light.light != this->light;
    }
    bool operator ! () {
      return isnan(this->light);
    }
  } Light;
  struct {
    bool moving;
    bool operator == (Data data) {
      return data.Moving.moving == this->moving;
    }
    bool operator != (Data data) {
      return data.Moving.moving != this->moving;
    }
    bool operator ! () {
      return isnan(this->moving);
    }
  } Moving;
  struct {
    uint8_t key;
    bool operator == (Data data) {
      return data.Key.key == this->key;
    }
    bool operator != (Data data) {
      return data.Key.key != this->key;
    }
    bool operator ! () {
      return this->key > 0 && this->key < 16;
    }
  } Key;

  Data operator = (Data &data) {
    memcpy(this, &data, sizeof(Data));
  }
};
typedef Data* pData;
struct Event {
  EventType type;
  Data data;
  bool isError() {
    return this->type == VALUE_CHANGE;
  }
};
typedef Event* pEvent;
typedef void (*EventListener)(Event event);

typedef std::set<EventListener> ListenerSet;
typedef ListenerSet* pListenerSet;
typedef std::pair<EventType, pListenerSet> ListenerPair;
typedef std::set<EventListener>::iterator ListenerIterator;
typedef std::map<EventType, pListenerSet>::iterator ListenerSetIterator;

class Listenable {
  protected:

  public:
    virtual EventType getDefaultEventType() {
      return ON_EVENT;
    }
    pEvent pLastEvent = NULL;
    Data prevData = { { 0 } };
    pData pDataz = NULL;
    std::set<EventListener> listeners;  // demo usage
    std::map<EventType, pListenerSet> listenersMap;
    Listenable() : Listenable(NULL) { }
    Listenable(pData pDataz) {
      if (pDataz) {
        this->pDataz = pDataz;
        prevData = *pDataz;
      }
      listenersMap.insert(ListenerPair(getDefaultEventType(), &listeners)); // demo usage
    }
    void onEvent(EventListener listener) {  // demo usage
      listeners.insert(listener);
    }
    void dispatch(Data data) {
      dispatch(data, getDefaultEventType());
    }
    void dispatch(Data data, EventType type) {
      Event event = { type, data };
      dispatch(event);
    }
    void dispatch(Event &event) {
      ListenerSetIterator listeners = listenersMap.find(event.type);
      if (listeners->second) {
        dispatch(*listeners->second, event);
      }
    }
    void dispatch(ListenerSet &listeners, Event &event) {
      pLastEvent = &event;
      prevData = event.data;
      for (ListenerIterator listener = listeners.begin();
          listener != listeners.end(); ++listener) {
        (*listener)(event);
      }
    }
};

#endif
