#pragma once
 #ifndef SMART_GARDEN_EMOTION_PIKACHU4_H
 #define SMART_GARDEN_EMOTION_PIKACHU4_H

 #include "../../base/types.hpp"
 
 unsigned char pikachu4_cells[] = {
 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011111,0b00010001,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b00001101,0b00001000,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00011000,0b00000000,
  0b00000000,0b00001111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
 };
 unsigned char pikachu4_coords[] = {
 14,2,0,
  15,2,1,
  16,2,2,
  17,2,3,
  18,2,4,
  16,3,5
 };
 ScreenFrame pikachu4 = {
 6, pikachu4_cells, pikachu4_coords, 500
 };

 #endif