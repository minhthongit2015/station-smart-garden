
#pragma once
#ifndef BEYOND_GARDEN_TYPES_H
#define BEYOND_GARDEN_TYPES_H

#include <set>
#include <map>
#include <iterator>
#include "./Listenable.hpp"

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

#endif
