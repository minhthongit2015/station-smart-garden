#pragma once
 #ifndef SMART_GARDEN_EMOTION_SLEEP3_H
 #define SMART_GARDEN_EMOTION_SLEEP3_H

 #include "../../base/types.hpp"
 
 unsigned char sleep3_cells[] = {
 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000011,0b00000010,0b00000000,0b00011100,0b00000000,
  0b00000000,0b00010000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000100,0b00000110,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
 };
 unsigned char sleep3_coords[] = {
 15,2,0,
  17,2,1,
  18,2,2,
  16,3,3
 };
 ScreenFrame sleep3 = {
 4, sleep3_cells, sleep3_coords, 1200
 };

 #endif