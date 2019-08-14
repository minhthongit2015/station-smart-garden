#pragma once
 #ifndef SMART_GARDEN_EMOTION_PIKACHU3_H
 #define SMART_GARDEN_EMOTION_PIKACHU3_H

 #include "../base/types.hpp"
 
 unsigned char pikachu3_cells[] = {
 0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000111,0b00001100,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00011000,0b00001100,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00000001,0b00000011,0b00000010,0b00000000,
  0b00000000,0b00000000,0b00000000,0b00000000,0b00011000,0b00001100,0b00000110,0b00000000,
  0b00000000,0b00000011,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
  0b00000000,0b00011000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
 };
 unsigned char pikachu3_coords[] = {
 8,1,0,
  9,1,1,
  10,1,2,
  11,1,3,
  9,2,4,
  10,2,5
 };
 ScreenFrame pikachu3 = {
 6, pikachu3_cells, pikachu3_coords, 1000
 };

 #endif