#pragma once
 #ifndef BEYOND_GARDEN_EMOTION_SLEEP5_H
 #define BEYOND_GARDEN_EMOTION_SLEEP5_H

 #include "../../base/types.hpp"
 
 unsigned char sleep5_cells[] = {
 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011100,0b00000000,
  0b00001000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000100,0b00001100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
 };
 unsigned char sleep5_coords[] = {
 15,2,0,
  17,2,1,
  18,2,2,
  16,3,3
 };
 ScreenFrame sleep5 = {
 4, sleep5_cells, sleep5_coords, 1000
 };

 #endif