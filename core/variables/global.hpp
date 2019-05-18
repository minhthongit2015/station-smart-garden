

#pragma once
#ifndef SMART_GARDEN_GLOBAL_H
#define SMART_GARDEN_GLOBAL_H


/* Global Variables */

#define CHECK_BYTE '\xff'

#define SERVER_PORT 4445
#define GARDEN_SIGNAL "moidoiten"
#define GARDEN_WIFI_PASSWORD "passla123"
#define LOOP_DELAY_TIME 100

#define DEVICE_ID "Alpha One"

#define DEVICE_INFO "\
{\
  \"id\": \"A1-01\", \
  \"role\": \"station\", \
  \"name\": \"Bản Mẫu Alpha Once\", \
  \"type\": \"A1\", \
  \"env_type\": [\"hydroponics\"], \
  \"roles\": [\"pump\", \"misting\", \"sensors\"], \
  \"equipments\": [\"sensors\"]\
}"
/*
  role: [<station>, <equipment>] Vai trò trong vườn
  type: Tên kiểu mẫu
  env_type: chỉ dùng cho station - loại môi trườnrg
  name: Tên mặc định thiết bị
  roles: Các khả năng

*/


#endif