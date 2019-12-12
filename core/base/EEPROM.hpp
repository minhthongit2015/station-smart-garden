
#pragma once
#ifndef BEYOND_GARDEN_EEPROM_H
#define BEYOND_GARDEN_EEPROM_H

#include <string.h>

/*************** EEPROM **************/
// template<class T> int write(int pos, const T &value) {
//   const byte *p = (const byte*)&T;
//   EEPROM.write(addr, val);
// }
int writeEEPROM(String &value, int pos=0) {
  EEPROM.write(pos++, CHECK_BYTE); // check byte

  byte *p = (byte*)(value.c_str());
  int end = pos + value.length();
  for (register int i = pos; i < end; ++i) {
    EEPROM.write(i, *p++);
  }
}

String readEEPROM(int pos = 0) {
  String rs;
  if (EEPROM.read(pos++) != CHECK_BYTE) return rs;

  char c;
  while (c = EEPROM.read(pos)) {
    if (!isAlphaNumeric(c)) break;
    rs += c;
  }
  return rs;
}

#endif

