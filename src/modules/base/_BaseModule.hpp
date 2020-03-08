
#pragma once
#ifndef BEYOND_GARDEN_BASE_MODULE_H
#define BEYOND_GARDEN_BASE_MODULE_H

#include <Arduino.h>
#include "../../libs/Listenable.hpp"


struct BaseModule : Listenable {
  EventData data = EMPTY_EVENT_DATA;
  EventData newData = EMPTY_EVENT_DATA;
  EventData prevData = EMPTY_EVENT_DATA;
  unsigned long checkInterval = 2000;
  unsigned long last = 0;
  virtual EventType getDefaultEventType() override {
    return VALUE_CHANGE;
  }
  virtual bool isReady() {
    return true;
  }
  
  BaseModule() : Listenable() {
    defineEvent(ERROR_EVENT);
    defineEvent(getDefaultEventType());
  }

  void onChange(EventListener listener) {
    onEvent(listener);
  }

  void onError(EventListener listener) {
    onEvent(ERROR_EVENT, listener);
  }

  virtual void setup() { }
  virtual void loop() {
    if (millis() - last < checkInterval || !isReady()) {
      return;
    }
    read();
    if (hasChange()) {
      dispatchModuleEvent();
      afterChange();
      prevData = data;
    }
    last = millis();
  }

  virtual EventData read() {
    fetch(newData);
    if (validate(newData)) {
      data = newData;
    }
    return data;
  }

  virtual void fetch(EventData &newData) {
    // assign new value to newData
  }

  virtual bool validate(EventData &newData) {
    return false;
  }

  virtual bool hasChange() {
    return data != prevData;
  }

  virtual void dispatchModuleEvent() {
    dispatch(getDefaultEventType(), data);
  }

  virtual void afterChange() {

  }
};

#endif
