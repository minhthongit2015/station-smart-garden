
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
    ListenerSet changeListeners;
    ListenerSet errorListeners;

  public:
    unsigned long CHECK_INTERVAL = 2000;

    BaseModule() : Listenable() {
      listenersMap.insert(ListenerPair(VALUE_CHANGE, &changeListeners));
      listenersMap.insert(ListenerPair(ERROR, &errorListeners));
    }

    void onChange(EventListener listener) {
      changeListeners.insert(listener);
    }
    void onError(EventListener listener) {
      errorListeners.insert(listener);
    }

    bool check() { // Check for new Data
      static unsigned long last = 0;
      if (millis() - last < CHECK_INTERVAL) {
        return false;
      }
      last = millis();
      return fetch();
    }
    Data read() {
      fetch();
      return prevData;
    }
    virtual bool fetch() { return false; }

    void setup() { }
    void loop() { }
};

#endif
