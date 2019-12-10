
#pragma once
#ifndef SMART_BOARD_PIN_MAP_H
#define SMART_BOARD_PIN_MAP_H

#include <Arduino.h>

// #ifndef D1
//   #define D1 5
// #endif
// #ifndef D2
//   #define D2 4
// #endif

#ifndef D3
  #define D3 0
#endif
#ifndef D4
  #define D4 2
#endif

#ifndef D5
  #define D5 14
#endif
#ifndef D6
  #define D6 12
#endif

#ifndef D7
  #define D7 13
#endif
#ifndef D8
  #define D8 15
#endif

#define SCL 5
#define SDA 4

#define SD2 9
#define SD3 10

#ifndef NOT_A_PIN
  #define NOT_A_PIN 0
#endif

#endif
