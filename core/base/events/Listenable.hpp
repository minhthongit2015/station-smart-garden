
#pragma once
#ifndef BEYOND_GARDEN_LISTENABLE_H
#define BEYOND_GARDEN_LISTENABLE_H

#include <set>
#include <map>
#include <iterator>
#include "./EventTypes.hpp"

class Listenable {
  protected:

  public:
    virtual EventType getDefaultEventType() {
      return ON_EVENT;
    }
    pEvent pLastEvent = NULL;
    EventData data = EMPTY_EVENT_DATA;
    EventData prevData = EMPTY_EVENT_DATA;
    std::map<EventType, pListenerSet> listenersMap;
    ListenerSet listeners;  // demo usage

    Listenable() {
      listenersMap.insert(ListenerPair(getDefaultEventType(), &listeners)); // demo usage
    }

    void onEvent(EventListener listener) {  // demo usage
      listeners.insert(listener);
    }
    
    void dispatch(EventData data) {
      dispatch(data, getDefaultEventType());
    }

    void dispatch(EventType type) {
      Event event = { type };
      dispatch(event);
    }

    void dispatch(EventData data, EventType type) {
      Event event = { type, data };
      dispatch(event);
    }

    void dispatch(Event &event) {
      ListenerSetIterator listeners = listenersMap.find(event.type);
      if (listeners->second) {
        dispatch(*listeners->second, event);
      }
    }

    void dispatch(pEvent event) {
      ListenerSetIterator listeners = listenersMap.find(event->type);
      if (listeners->second) {
        dispatch(*listeners->second, *event);
      }
    }

    void dispatch(ListenerSet &listeners, Event &event) {
      pLastEvent = &event;
      for (ListenerIterator listener = listeners.begin();
          listener != listeners.end(); ++listener) {
        (*listener)(&event);
      }
      prevData = data;
    }
};

#endif
