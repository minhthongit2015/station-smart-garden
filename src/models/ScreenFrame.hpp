
#pragma once
#ifndef BEYOND_GARDEN_SCREEN_FRAME_H
#define BEYOND_GARDEN_SCREEN_FRAME_H

#include <set>
#include "./BaseTypes.hpp"

struct ScreenFrame
{
  unsigned char size;
  unsigned char *cells;
  unsigned char *coords;
  timestamp_t delay;
};
typedef ScreenFrame *pScreenFrame;
typedef std::set<pScreenFrame> Animation;
typedef Animation::iterator AnimationFrame;

#endif
