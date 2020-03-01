
#pragma once
#ifndef BEYOND_GARDEN_DEVICE_INFO_H
#define BEYOND_GARDEN_DEVICE_INFO_H

#define DEVICE_ID "Alpha One"
#define DEVICE_INFO "\
{\
  \"_id\": \"5e0760cafa35674becf0ff2d\",\
  \"__t\": \"Station\",\
  \"garden\": \"5e073c59fa35674becf0ff28\",\
  \"name\": \"Alpha Once\",\
  \"model\": \"A1\"\
}"

struct DeviceInfo {
  const char *id;
  const char *__t;
  const char *garden;
  const char *name;
  const char *model;
} deviceInfo = {
  "5e0760cafa35674becf0ff2d",
  "station",
  "5e073c59fa35674becf0ff28",
  "Alpha Once"
};



#endif