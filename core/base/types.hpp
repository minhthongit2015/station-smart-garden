
#pragma once
#ifndef SMART_GARDEN_TYPES_H
#define SMART_GARDEN_TYPES_H

typedef unsigned long timestamp_t;

struct ScreenFrame
{
  unsigned char size;
  unsigned char *cells;
  unsigned char *coords;
  timestamp_t delay;
};
typedef ScreenFrame *pScreenFrame;


#endif
