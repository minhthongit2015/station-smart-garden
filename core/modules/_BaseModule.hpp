
#pragma once
#ifndef BEYOND_GARDEN_BASE_MODULE_H
#define BEYOND_GARDEN_BASE_MODULE_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include <set>
#include <map>
#include <iterator>


class BaseModule : public Listenable {
  protected:
    ListenerSet changeListeners;  // built-in supported listener
    ListenerSet errorListeners;   // built-in supported listener

  public:
    EventData newData = EMPTY_EVENT_DATA;
    unsigned long checkInterval = 2000;
    unsigned long last = 0;
    virtual EventType getDefaultEventType() override {
      return VALUE_CHANGE;
    }
    
    BaseModule() : Listenable() {
      listenersMap.insert(ListenerPair(getDefaultEventType(), &changeListeners));
      listenersMap.insert(ListenerPair(ERROR, &errorListeners));
    }

    void onChange(EventListener listener) {
      changeListeners.insert(listener);
    }

    void onError(EventListener listener) {
      errorListeners.insert(listener);
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

    virtual void setup() { }
    virtual void loop() {
      if (millis() - last < checkInterval) {
        return;
      }
      read();
      if (hasChange()) {
        dispatch(data);
      }
      last = millis();
    }
};

#endif
