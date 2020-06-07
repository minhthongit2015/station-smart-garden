
#pragma once
#ifndef BEYOND_GARDEN_DEVICE_INFO_H
#define BEYOND_GARDEN_DEVICE_INFO_H

#define DEVICE_ID "Alpha One"
#define DEVICE_INFO "\
{\
  \"_id\": \"5e9debf77c5062551cfb2654\",\
  \"name\": \"Alpha Once\",\
  \"model\": \"A1\"\
}"

struct DeviceInfo
{
  const char *id;
  const char *name;
  const char *model;
} deviceInfo = {
    "5e0760cafa35674becf0ff2d",
    "Alpha Once",
    "A1"};

#endif