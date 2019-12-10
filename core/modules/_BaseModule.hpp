
#pragma once
#ifndef BEYOND_GARDEN_BASE_MODULE_H
#define BEYOND_GARDEN_BASE_MODULE_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include <set>
#include <map>
#include <iterator>


class BaseModule {
  protected:
    std::set<ChangeListener> changeListeners;
    std::set<ErrorListener> errorListeners;
    std::map<EventType, pListenerSet> listenersMap;

  public:
    unsigned long CHECK_INTERVAL = 2000;
    pChangeEvent lastEvent = NULL;
    pData data = NULL;

    BaseModule() {
      listenersMap.insert(ListenerPair(VALUE_CHANGE, &changeListeners));
      listenersMap.insert(ListenerPair(ERROR, &errorListeners));
    }

    void onChange(ChangeListener listener) {
      changeListeners.insert(listener);
    }
    void onError(ErrorListener listener) {
      errorListeners.insert(listener);
    }
    void dispatch(ListenerSet &listeners, Event &event) {
      for (ListenerIterator listener = listeners.begin();
          listener != listeners.end(); ++listener) {
        (*listener)(event);
      }
    }
    void dispatch(Event event) {
      for (ListenerSetIterator listeners = listenersMap.begin();
          listeners != listenersMap.end(); ++listeners) {
        if (event.type == listeners->first) {
          dispatch(*listeners->second, event);
        }
      }
    }
    void dispatch(Data data, EventType type) {
      Event event = { type, data };
      BaseModule::dispatch(event);
    }

    void setup() { }
    void loop() { }

    bool check() { // Check for new Data
      static unsigned long last = 0;
      if (millis() - last >= CHECK_INTERVAL) {
        last = millis();
        return fetch();
      }
      return false;
    }
    Data read() {
      fetch();
      return *data;
    }
    virtual bool fetch() { return false; }    // Override to 
};

#endif
