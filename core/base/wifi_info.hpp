
#pragma once
#ifndef SMART_GARDEN_WIFI_INFO_H
#define SMART_GARDEN_WIFI_INFO_H

#include "./utils.hpp"

class WifiInfo {
  public:
  int32_t rssi;
  String ssid;
  String pass;

  bool isGarden;
  
  WifiInfo() {
    this->isGarden = false;
    this->pass = GARDEN_WIFI_PASSWORD;
  }

  WifiInfo(int32_t rssi, String ssid, String pass=GARDEN_WIFI_PASSWORD) {
    this->rssi = rssi;
    this->ssid = ssid;
    this->pass = pass;
    this->isGarden = false;
  }

  void set(int i) {
    this->rssi = WiFi.RSSI(i);
    this->ssid = WiFi.SSID(i);
    this->isGarden = this->ssid.indexOf(GARDEN_SIGNAL_STRING) >= 0;
  }
};

#endif