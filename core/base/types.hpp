
#pragma once
#ifndef SMART_GARDEN_TYPES_H
#define SMART_GARDEN_TYPES_H

#include <set>
#include <map>
#include <iterator>

typedef unsigned long timestamp_t;

struct ScreenFrame
{
  unsigned char size;
  unsigned char *cells;
  unsigned char *coords;
  timestamp_t delay;
};
typedef ScreenFrame *pScreenFrame;

class WifiInfo {
  public:
  int32_t rssi;
  String ssid;
  String pass;
  
  WifiInfo() {
    this->pass;
  }

  WifiInfo(int32_t rssi, String ssid, String pass) {
    this->rssi = rssi;
    this->ssid = ssid;
    this->pass = pass;
  }

  void set(int32_t rssi, String ssid, String pass) {
    this->rssi = rssi;
    this->ssid = ssid;
    this->pass = pass;
  }
};

// Event - Listener

enum EventType {
  ERROR = 1,
  VALUE_CHANGE,
  KEY_DOWN,
  KEY_UP,
  KEY_PRESS
};
union Data {
  struct {
    float temperature;
    float humidity;
    bool operator == (Data data) {
      return data.HuTemp.humidity == this->humidity
        && data.HuTemp.humidity == this->temperature;
    }
    bool operator != (Data data) {
      return data.HuTemp.humidity != this->humidity
        || data.HuTemp.humidity != this->temperature;
    }
    bool operator ! () {
      return isnan(this->humidity) && isnan(this->temperature);
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
      return isnan(this->key);
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

typedef Event ErrorEvent;
typedef ErrorEvent* pErrorEvent;

typedef Event ChangeEvent;
typedef ChangeEvent* pChangeEvent;
typedef Event KeyEvent;
typedef KeyEvent* pKeyEvent;

typedef void (*EventListener)(Event event);
typedef void (*ErrorListener)(ErrorEvent event);
typedef void (*ChangeListener)(ChangeEvent event);
typedef void (*KeyListener)(ChangeEvent event);

typedef std::set<EventListener> ListenerSet;
typedef ListenerSet* pListenerSet;
typedef std::pair<EventType, pListenerSet> ListenerPair;
typedef std::set<EventListener>::iterator ListenerIterator;
typedef std::map<EventType, pListenerSet>::iterator ListenerSetIterator;

#endif
