

#pragma once
#ifndef SMART_GARDEN_WIFI_STA_H
#define SMART_GARDEN_WIFI_STA_H

#include "./base/utils.hpp"
#include "./base/wifi.hpp"
#include "./socket_io.hpp"
#include "./emulator.hpp"
#include "./modules/Controller.hpp"

/**********************************************************************************************************
 *                                         Hướng dẫn thiết đặt ESP                                        *
 * 
 * "#define DEVICE_ID" để xác định id của thiết bị
 * "#define CONTROL {pin1, pin2,...}" để thiết đặt là trạm này dùng để nhận lệnh điều khiển
 * "#define SENSOR_DHT22 <pin>" để thiết đặt là đọc dữ liệu cảm biến DHT22 trên <pin>
 * "#define SENSOR_BH1750 <pin>" để thiết đặt là đọc dữ liệu cảm biến BH1750 trên <pin>
 * "#define SENSOR_HC_SR501 <pin>" để thiết đặt là đọc dữ liệu cảm biến HC_SR501 trên <pin>
 * 
 **********************************************************************************************************/

class SmartGardenStation {
  private:
  GardenWifi wifi;  // Giữ kết nối đến mạng wifi

  public:
  void setup();
  void loop();
};


/*                   Setup                  */
void SmartGardenStation::setup() {
  prl(" <1> Smart Garden Station Setup!");

  controller.setup();

  WiFi.mode(WIFI_STA);

  this->wifi.setup();

  websocketSetup();

  emulator.setup();
}


/*                   Loop                    */
void SmartGardenStation::loop() {
  // static unsigned long timer = millis();
  //  digitalWrite(equips[0], 1);
  // if (millis() - timer > 1800000) reset(); // Khởi động lại ESP mỗi 30p để tránh treo
  websocketLoop();
  
  emulator.loop();

  delay(LOOP_DELAY_TIME);
}


#endif
