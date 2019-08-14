
#pragma once
#ifndef SMART_STATION_CONFIG_H
#define SMART_STATION_CONFIG_H


#define ARDUINO 10809

/* Device Info */
#define DEVICE_ID "Alpha One"
#define DEVICE_INFO "\
{\
  \"id\": \"A1-01\", \
  \"role\": \"station\", \
  \"name\": \"Alpha Once\", \
  \"type\": \"A1\", \
  \"env_type\": [\"hydroponics\"], \
  \"roles\": [\"pump\", \"misting\", \"sensors\"], \
  \"physical_address\": [\"STA_A1_01\"], \
  \"secret_key\": \"Secret_STA_A1_01\"\
}"
/*
  role: [<station>, <equipment>] Vai trò trong vườn
  type: Tên kiểu mẫu
  env_type: chỉ dùng cho station - loại môi trườnrg
  name: Tên mặc định thiết bị
  roles: Các khả năng

*/

#define CHECK_BYTE '\xff'

// #define GARDEN_SIGNAL "Garden_"
// #define GARDEN_WIFI_PASSWORD "12345678"
// #define GARDEN_SIGNAL "IUHYRA"
// #define GARDEN_WIFI_PASSWORD "iuhyra@12345"
#define LOOP_DELAY_TIME 10


const char GARDEN_HOST[] = "192.168.1.12";
const long GARDEN_PORT = 4000;
const char GARDEN_SIGNAL[] = "moidoiten";
const char GARDEN_WIFI_PASSWORD[] = "passla123";


#endif
