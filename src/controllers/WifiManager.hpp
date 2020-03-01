
#pragma once
#ifndef BEYOND_GARDEN_WIFI_MANAGER_H
#define BEYOND_GARDEN_WIFI_MANAGER_H

#include "../configs/ArduinoDefine.hpp"
#include <ESP8266WiFi.h>
#include "../utils/Utils.hpp"
#include "../models/WifiInfo.hpp"
#include "../helpers/ConfigHelper.hpp"
#include "../configs/DeviceInfo.hpp"

#define WIFI "Wifi"

#define GARDEN_WIFI_SIGNAL "gardenWifiSignal"
#define GARDEN_WIFI_PASSWORD "gardenWifiPassword"


bool checkGardenSignal(String ssid) {
  return cfg.getStr(GARDEN_WIFI_SIGNAL).charAt(0)
    && ssid.indexOf(cfg.getStr(GARDEN_WIFI_SIGNAL)) >= 0;
}

struct WifiManager {
  WifiInfo wifiList[30];
  unsigned long RETRY_INTERVAL = 15000;
  bool isConnected() { return WiFi.status() == WL_CONNECTED; }

  void setup();
  void loop();

  void scanAndConnect();
  int scanWifiNetworks();
  
  wl_status_t connectWifi(String &ssid, String &pass);
  bool connectWifi(WifiInfo &wifi);
  bool connectWifi(WifiInfo &wifi, byte maxTry, int timeDelay=1000);
} wifi;

extern WifiManager wifi;

void WifiManager::setup() {
  logStart(WIFI);
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname(DEVICE_ID);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  
  if (wifi.isConnected() && !checkGardenSignal(WiFi.SSID())) {
    log(WIFI, "Garden Signal changes -> disconnect().");
    WiFi.disconnect(true);
  }

  if (wifi.isConnected()) {
    logf(WIFI, "Connected ---> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
  } else {
    if (!isBlank(WiFi.SSID().c_str()) && !isBlank(WiFi.psk().c_str())) {
      logf(WIFI, "Connecting to ---> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
    }
  }
}

void WifiManager::loop() {
  static unsigned long last = 0;
  static unsigned long delayTime = 15000;

  if (!wifi.isConnected() && millis() - last > delayTime) {
    scanAndConnect();
    last = millis();
  }
}

void WifiManager::scanAndConnect() {
  int numNetworks = scanWifiNetworks();
  
  int count = 0;
  for (int i = 0; i < numNetworks; ++i) {
    if (checkGardenSignal(wifiList[i].ssid)) {
      ++count;
    }
  }
  logf(WIFI, "Found: %d garden / %d AP\r\n", count, numNetworks);

  if (count > 0) {
    for (int i = 0; i < numNetworks; ++i) {
      if (checkGardenSignal(wifiList[i].ssid))
        connectWifi(wifiList[i]);
    }
  }
}


int WifiManager::scanWifiNetworks() {
  log("Wifi", "Scanning...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks > 0) {
    for (register int i = 0; i < numNetworks; ++i) {
      wifiList[i].set(WiFi.RSSI(i), WiFi.SSID(i), cfg.getStr(GARDEN_WIFI_PASSWORD));
    }
    #ifdef EVN_DEV
    prl("┌────────────────── Scan Result ──────────────────┐");
    for (register int i = 0; i < numNetworks; ++i) {
      Serial.printf("├ [%02d] > (%s) %3d : %s\r\n", i + 1,
        checkGardenSignal(wifiList[i].ssid) ? "Garden" : "------",
        wifiList[i].rssi,
        wifiList[i].ssid.c_str());
    }
    prl("└─────────────────────────────────────────────────┘");
    #endif
  }

  return numNetworks;
}

bool WifiManager::connectWifi(WifiInfo &wifiInfo) {
  this->connectWifi(wifiInfo, 14, 500);
}

bool WifiManager::connectWifi(WifiInfo &wifiInfo, byte maxTry, int timeDelay) {
  logf(WIFI, "Joining to -~=> %s : %s\r\n", wifiInfo.ssid.c_str(), wifiInfo.pass.c_str());

  // Chờ Kết nối ngầm vào mạng cũ trong 6s
  byte retry1 = maxTry/2;
  waitFor(wifi.isConnected(), retry1, timeDelay);

  // Nếu không kết nối được mạng cũ thì thử kết nối với mạng mới
  if (!wifi.isConnected() && connectWifi(wifiInfo.ssid, wifiInfo.pass) != WL_CONNECTED) {
    byte retry = maxTry;
    waitFor(wifi.isConnected(), retry, timeDelay); // Chờ Kết nối trong 15s
  }

  #ifdef ENV_DEV
  if (wifi.isConnected()) {
    Serial.printf("\r\n> [Wifi] Connected ===> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
  } else {
    Serial.printf("\r\n> [Wifi] Failed -/-> %s : %s\r\n", wifiInfo.ssid.c_str(), wifiInfo.pass.c_str());
  }
  #endif

  return wifi.isConnected();
}

wl_status_t WifiManager::connectWifi(String &ssid, String &pass) {
  return WiFi.begin(ssid.c_str(), pass.c_str());
}

#endif