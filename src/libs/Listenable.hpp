
#pragma once
#ifndef BEYOND_GARDEN_LISTENABLE_H
#define BEYOND_GARDEN_LISTENABLE_H

#include <map>
#include "../models/EventTypes.hpp"

struct Listenable {
  virtual EventType getDefaultEventType() {
    return ON_EVENT;
  }
  pEvent pLastEvent = NULL;
  EventData data = EMPTY_EVENT_DATA;
  EventData prevData = EMPTY_EVENT_DATA;
  ListenerMap listenerMap;

  Listenable() {
    defineEvent(getDefaultEventType());
  }

  pListenerSet defineEvent(EventType type) {
    return defineEvent(type, new ListenerSet());
  }

  pListenerSet defineEvent(EventType type, pListenerSet listenerSet) {
    listenerMap.insert(ListenerEntry(type, listenerSet));
    return listenerSet;
  }

  pListenerSet getListenerSet(EventType type) {
    return listenerMap.find(type)->second;
  }

  void onEvent(EventListener listener) {
    onEvent(getDefaultEventType(), listener);
  }

  void onEvent(EventType type, EventListener listener) {
    pListenerSet listenerSet = getListenerSet(type);
    if (listenerSet) {
      listenerSet->insert(listener);
    }
  }
  
  void dispatch(EventData data) {
    dispatch(getDefaultEventType(), data);
  }

  void dispatch(EventType type) {
    dispatch(type, EMPTY_EVENT_DATA);
  }

  void dispatch(EventType type, EventData data) {
    Event event = { type, data };
    dispatch(event);
  }

  void dispatch(Event &event) {
    dispatch(&event);
  }

  void dispatch(pEvent event) {
    pListenerSet listenerSet = getListenerSet(event->type);
    if (listenerSet) {
      dispatch(*listenerSet, *event);
    }
  }

  void dispatch(ListenerSet &listenerSet, Event &event) {
    pLastEvent = &event;
    for (ListenerIterator listener = listenerSet.begin();
        listener != listenerSet.end(); ++listener) {
      (*listener)(&event);
    }
    prevData = data;
  }
};

#endif
