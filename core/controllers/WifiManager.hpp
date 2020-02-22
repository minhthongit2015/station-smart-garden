

#pragma once
#ifndef BEYOND_GARDEN_WIFI_MANAGER_H
#define BEYOND_GARDEN_WIFI_MANAGER_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include "../controllers/ConfigManager.hpp"
#include "../variables/constants.hpp"
#include "../variables/DeviceInfo.hpp"

#define GARDEN_WIFI_SIGNAL "GARDEN_WIFI_SIGNAL"
#define GARDEN_WIFI_PASSWORD "GARDEN_WIFI_PASSWORD"

bool checkGardenSignal(String ssid) {
  return cfg.getStr(GARDEN_WIFI_SIGNAL).charAt(0)
    && ssid.indexOf(cfg.getStr(GARDEN_WIFI_SIGNAL)) >= 0;
}

class WifiManager {
  private:
  public:
  WifiInfo wifiList[30];
  unsigned long RETRY_INTERVAL = 15000;

  int scanWifiNetworks(bool debug=false);
  
  wl_status_t connectWifi(String ssid, String pass);
  bool connectWifi(WifiInfo wifi, bool debug);
  bool connectWifi(WifiInfo wifi, byte maxTry=15, int wait=1000, bool debug=false);

  void scanAndConnect();

  bool isConnected() { return WiFi.status() == WL_CONNECTED; }

  void setup();
  void loop();
} wifiMgr;

void WifiManager::setup() {
  logStart("Wifi");
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname(DEVICE_ID);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  
  if (wifiMgr.isConnected() && !checkGardenSignal(WiFi.SSID())) {
    log("Wifi", "Garden Signal changes -> disconnect().");
    WiFi.disconnect(true);
  }

  if (wifiMgr.isConnected()) {
    prf("> [Wifi] Connected ---> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
  }
}

void WifiManager::loop() {
  static unsigned long last = 0;
  static unsigned long delayTime = 15000;

  if (!wifiMgr.isConnected() && millis() - last > delayTime) {
    scanAndConnect();
    last = millis();
  }
}


void WifiManager::scanAndConnect() {
  int n = this->scanWifiNetworks(true);
  
  int count = 0;
  for (int i = 0; i < n; ++i) {
    if (checkGardenSignal(this->wifiList[i].ssid)) {
      ++count;
    }
  }
  prf("> [Wifi] Found: %d garden / %d AP\r\n", count, n);

  if (count > 0) {
    for (int i = 0; i < n; ++i) {
      if (checkGardenSignal(this->wifiList[i].ssid))
        this->connectWifi(this->wifiList[i], true);
    }
  }
}


int WifiManager::scanWifiNetworks(bool debug) {
  if (debug) log("Wifi", "Scanning...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks > 0) {
    for (register int i = 0; i < numNetworks; ++i) {
      wifiList[i].set(WiFi.RSSI(i), WiFi.SSID(i), cfg.getStr(GARDEN_WIFI_PASSWORD));
    }
    if (debug) {
      prl("┌────────────────── Scan Result ──────────────────┐");
      for (register int i = 0; i < numNetworks; ++i) {
        Serial.printf("├ [%02d] > (%s) %3d : %s\r\n", i + 1,
          checkGardenSignal(wifiList[i].ssid) ? "Garden" : "------",
          wifiList[i].rssi,
          wifiList[i].ssid.c_str());
      }
      prl("└─────────────────────────────────────────────────┘");
    }
  }

  return numNetworks;
}


wl_status_t WifiManager::connectWifi(String ssid, String pass) {
  return WiFi.begin(ssid.c_str(), pass.c_str());
}

bool WifiManager::connectWifi(WifiInfo wifi, bool debug) {
  this->connectWifi(wifi, 14, 500, debug);
}
bool WifiManager::connectWifi(WifiInfo wifi, byte maxTry, int wait, bool debug) {
  if (debug) Serial.printf("> [Wifi] Joining to -~=> %s : %s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());

  byte retry1 = maxTry/2;
  while (!wifiMgr.isConnected()) { // Chờ Kết nối trong 15s
    pr(retry1); pr("."); delay(wait);
    if (!--retry1) break;
  } prl();

  byte retry = maxTry;
  if (!wifiMgr.isConnected() && connectWifi(wifi.ssid, wifi.pass) != WL_CONNECTED) {
    while (!wifiMgr.isConnected()) { // Chờ Kết nối trong 15s
      pr(retry); pr("."); delay(wait);
      if (!--retry) break;
    } prl();
  }

  if (debug) {
    if (wifiMgr.isConnected()) {
      Serial.printf("\r\n> [Wifi] Connected ===> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
    } else {
      Serial.printf("\r\n> [Wifi] Failed -/-> %s : %s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());
    }
  }

  return wifiMgr.isConnected();
}

#endif