
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
    unsigned long CHECK_INTERVAL = 2000;
    unsigned long last = 0;
    virtual EventType getDefaultEventType() override {
      return VALUE_CHANGE;
    }
    
    BaseModule() : BaseModule(NULL) { }
    BaseModule(pData pDataz) : Listenable(pDataz) {
      listenersMap.insert(ListenerPair(getDefaultEventType(), &changeListeners));
      listenersMap.insert(ListenerPair(ERROR, &errorListeners));
    }

    void onChange(EventListener listener) {
      changeListeners.insert(listener);
    }
    void onError(EventListener listener) {
      errorListeners.insert(listener);
    }

    virtual bool check() { // Check for new Data
      if (millis() - last < CHECK_INTERVAL) {
        return false;
      }
      last = millis();
      // prl("<Checking Data>");
      return fetch();
    }
    virtual Data read() {
      // prl("<Reading Data>");
      fetch();
      return pDataz ? *pDataz : prevData; // return Data;
    }
    virtual bool fetch() {
      // prl("<Fetching Data>");
      return false;
    }

    virtual void setup() { }
    virtual void loop() { }
};

#endif
