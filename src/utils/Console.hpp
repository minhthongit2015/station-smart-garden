
#pragma once
#ifndef BEYOND_GARDEN_CONSOLE_H
#define BEYOND_GARDEN_CONSOLE_H

#include "./Utils.hpp"
#include "./Constants.hpp"
#include "../models/BaseTypes.hpp"


struct Console {
  static timestamp_t timers[20];
  static char *labels[20];

  static timestamp_t time(TimerIdType timerId = DEFAULT_TIMER, char *label = NULL) {
    timers[timerId] = millis();
    if (label) {
      labels[timerId] = label;
    }
    return timers[timerId];
  }

  static timestamp_t timePass(TimerIdType timerId) {
    return millis() - timers[timerId];
  }

  static bool timeOver(TimerIdType timerId, timestamp_t timeout) {
    return millis() - timers[timerId] > timeout;
  }

  static timestamp_t timeLog(TimerIdType timerId) {
    static timestamp_t dif = 0;
    dif = timePass(timerId);
    prf("> [%s] : %d\r\n", labels[timerId], dif);
    return dif;
  }

  static timestamp_t timeEnd(TimerIdType timerId) {
    return timeLog(timerId);
  }
};

unsigned long Console::timers[] = { 0 };
char *Console::labels[] = { 0 };


#endif