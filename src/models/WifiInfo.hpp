
#pragma once
#ifndef BEYOND_GARDEN_WIFI_INFO_H
#define BEYOND_GARDEN_WIFI_INFO_H

#include <Arduino.h>

struct WifiInfo {
  int32_t rssi;
  String ssid;
  String pass;
  bool isGarden;

  void set(int32_t rssi, String ssid, String pass, bool isGarden) {
    this->rssi = rssi;
    this->ssid = ssid;
    this->pass = pass;
    this->isGarden = isGarden;
  }
};

#endif
