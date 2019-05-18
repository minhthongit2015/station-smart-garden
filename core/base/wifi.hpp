

#pragma once
#ifndef SMART_GARDEN_WIFI_H
#define SMART_GARDEN_WIFI_H

#include "./utils.hpp"
#include "./wifi_info.hpp"

#define wifiConnected (WiFi.status() == WL_CONNECTED)

/** Đảm bảo kết nối đến mạng wifi vườn.
 * - Không xây dựng cơ chế giao tiếp đặc biệt với AP để có thể tương thích với các modem ngoài.
 * - 
 */
class GardenWifi {
  private:
  public:
  WifiInfo wifiList[30];

  GardenWifi() {
  }

  // Lấy danh sách wifi xung quanh
  int getWifiList(bool debug=false);
  
  // Kết nối đến mạng wifi
  wl_status_t _connect(String ssid, String pass);
  bool connectWifi(WifiInfo wifi, bool debug);
  bool connectWifi(WifiInfo wifi, byte maxTry=15, int wait=1000, bool debug=false);

  void setup();
  void loop();
};


/*                   Setup                  */
void GardenWifi::setup() {
  log("Wifi", "<*> Wifi Setup!");
  WiFi.hostname(DEVICE_ID);
  
  // Kiểm tra thông tin mạng có thay đổi không
  if (!checkGardenSignal(WiFi.SSID())) {
    log("Wifi", "<!> Garden Signal changes -> disconnect().");
    WiFi.disconnect();
  }

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  if (!wifiConnected) {
    int n = this->getWifiList(true);
    
    int count = 0;
    for (int i = 0; i < n; ++i) if (this->wifiList[i].isGarden) ++count;
    prf("[Wifi] > Found: %d AP (%d garden)\r\n", n, count);

    for (int i = 0; i < n; ++i) {
      if (this->wifiList[i].isGarden)
        this->connectWifi(this->wifiList[i], true);
    }
  } else {
    prf("[Wifi] > <*> Connected -> %s:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
  }
}


/*                   Loop                    */
void GardenWifi::loop() {
  // Kiểm tra có rớt mạng k, thử kết nối lại nếu cần thiết
}


int GardenWifi::getWifiList(bool debug) {
  if (debug) log("Wifi", "Scanning...");
  int n = WiFi.scanNetworks();

  if (n > 0) {
    for (register int i = 0; i < n; ++i) {
      wifiList[i].set(i);
    }
    if (debug) {
      prl("┌────────────────── Scan Result ──────────────────┐");
      for (register int i = 0; i < n; ++i) {
        Serial.printf("├ [%02d] > (%d) %3d : %s\r\n", i + 1,
          wifiList[i].isGarden,
          wifiList[i].rssi,
          wifiList[i].ssid.c_str());
      }
      prl("└─────────────────────────────────────────────────┘");
    }
  }

  return n;
}


wl_status_t GardenWifi::_connect(String ssid, String pass) {
  return WiFi.begin(ssid.c_str(), pass.c_str());
}

bool GardenWifi::connectWifi(WifiInfo wifi, bool debug) {
  this->connectWifi(wifi, 14, 500, debug);
}
bool GardenWifi::connectWifi(WifiInfo wifi, byte maxTry, int wait, bool debug) {
  if (debug) Serial.printf("[Wifi] > Joining to: %s:%s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());

  byte retry1 = maxTry/2;
  while (!wifiConnected) { // Chờ Kết nối trong 15s
    pr(retry1); pr("."); delay(wait);
    if (!--retry1) break;
  } prl();

  byte retry = maxTry;
  if (!wifiConnected &&_connect(wifi.ssid, wifi.pass) != WL_CONNECTED) {
    while (!wifiConnected) { // Chờ Kết nối trong 15s
      pr(retry); pr("."); delay(wait);
      if (!--retry) break;
    } prl();
  }

  if (debug) {
    if (wifiConnected) {
      Serial.printf("\r\n[Wifi] > Connected -> %s:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
    } else {
      Serial.printf("\r\n[Wifi] > Failed -> %s:%s\r\n", wifi.ssid.c_str(), wifi.pass.c_str());
    }
  }

  return wifiConnected;
}

#endif