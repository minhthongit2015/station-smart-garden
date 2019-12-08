

#pragma once
#ifndef SMART_GARDEN_WIFI_CONTROLLER_H
#define SMART_GARDEN_WIFI_CONTROLLER_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include "../variables/global.hpp"

#define wifiConnected (WiFi.status() == WL_CONNECTED)

bool checkGardenSignal(String ssid) {
  return Global::cfg.gardenWifiSignal.charAt(0) && ssid.indexOf(Global::cfg.gardenWifiSignal) >= 0;
}

class WifiController {
  private:
  public:
  WifiInfo wifiList[30];

  int scanWifiNetworks(bool debug=false);
  
  wl_status_t connectWifi(String ssid, String pass);
  bool connectWifi(WifiInfo wifi, bool debug);
  bool connectWifi(WifiInfo wifi, byte maxTry=15, int wait=1000, bool debug=false);

  void scanAndConnect();

  void setup();
  void loop();
} wifiCtl;

void WifiController::setup() {
  logStart("Wifi");
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname(DEVICE_ID);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  
  if (wifiConnected && !checkGardenSignal(WiFi.SSID())) {
    logz("Wifi", "Garden Signal changes -> disconnect().");
    WiFi.disconnect(true);
  }

  if (wifiConnected) {
    prf("> [Wifi] Connected ---> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
  }
}

void WifiController::loop() {
  static unsigned long last = 0;
  static unsigned long delayTime = 15000;

  if (!wifiConnected && millis() - last > delayTime) {
    scanAndConnect();
    last = millis();
  }
}


void WifiController::scanAndConnect() {
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


int WifiController::scanWifiNetworks(bool debug) {
  if (debug) logz("Wifi", "Scanning...");
  int n = WiFi.scanNetworks();

  if (n > 0) {
    for (register int i = 0; i < n; ++i) {
      wifiList[i].set(WiFi.RSSI(i), WiFi.SSID(i), Global::cfg.gardenWifiPassword);
    }
    if (debug) {
      prl("┌────────────────── Scan Result ──────────────────┐");
      for (register int i = 0; i < n; ++i) {
        Serial.printf("├ [%02d] > (%s) %3d : %s\r\n", i + 1,
          checkGardenSignal(wifiList[i].ssid) ? "Garden" : "------",
          wifiList[i].rssi,
          wifiList[i].ssid.c_str());
      }
      prl("└─────────────────────────────────────────────────┘");
    }
  }

  return n;
}


wl_status_t WifiController::connectWifi(String ssid, String pass) {
  return WiFi.begin(ssid.c_str(), pass.c_str());
}

bool WifiController::connectWifi(WifiInfo wifi, bool debug) {
  this->connectWifi(wifi, 14, 500, debug);
}
bool WifiController::connectWifi(WifiInfo wifi, byte maxTry, int wait, bool debug) {
  if (debug) Serial.printf("> [Wifi] Joining to -~=> %s : %s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());

  byte retry1 = maxTry/2;
  while (!wifiConnected) { // Chờ Kết nối trong 15s
    pr(retry1); pr("."); delay(wait);
    if (!--retry1) break;
  } prl();

  byte retry = maxTry;
  if (!wifiConnected && connectWifi(wifi.ssid, wifi.pass) != WL_CONNECTED) {
    while (!wifiConnected) { // Chờ Kết nối trong 15s
      pr(retry); pr("."); delay(wait);
      if (!--retry) break;
    } prl();
  }

  if (debug) {
    if (wifiConnected) {
      Serial.printf("\r\n> [Wifi] Connected ---> %s : %s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
    } else {
      Serial.printf("\r\n> [Wifi] Failed -/-> %s : %s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());
    }
  }

  return wifiConnected;
}

#endif