
#pragma once
#ifndef BEYOND_GARDEN_LISTENABLE_H
#define BEYOND_GARDEN_LISTENABLE_H

#include <map>
#include "../models/EventTypes.hpp"

struct Listenable {
  virtual EventType getDefaultEventType() {
    return DEFAULT_EVENT;
  }
  ListenerMap listenerMap;

  Listenable() {
    defineEvent(getDefaultEventType());
  }

  pListenerSet defineEvent(EventType type) {
    return defineEvent(type, new ListenerSet());
  }

  pListenerSet defineEvent(EventType type, pListenerSet listenerSet) {
    listenerMap[type] = listenerSet;
    // listenerMap.insert(ListenerEntry(type, listenerSet));
    return listenerSet;
  }

  pListenerSet getListenerSet(EventType type) {
    pListenerSet listenerSet = listenerMap[type];
    if (!listenerSet) {
      prf("> Event \"%d\" was not defined\r\n", type);
    }
    return listenerSet;
  }

  void removeAllListeners() {
    ListenerMapIterator end = listenerMap.end();
    for (ListenerMapIterator listenerSet = listenerMap.begin(); listenerSet != end; ++listenerSet) {
      if (listenerSet->second) {
        delete listenerSet->second;
      }
    }
    listenerMap.clear();
  }

  void listListeners() {
    ListenerMapIterator end = listenerMap.end();
    for (ListenerMapIterator listenerSet = listenerMap.begin(); listenerSet != end; ++listenerSet) {
      prf("> Defined Event: %d\r\n", listenerSet->first);
    }
  }

  void onEvent(EventListener listener) {
    onEvent(getDefaultEventType(), listener);
  }

  void onEvent(EventType type, EventListener listener) {
    pListenerSet listenerSet = getListenerSet(type);
    if (!listenerSet) {
      prf("> Please define event \"%d\" before setup its listeners\r\n", type);
      return;
    }
    listenerSet->insert(listener);
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
    } else {
      prf("Cannot dispatch undefined event type %d\r\n", event->type);
    }
  }

  void dispatch(ListenerSet &listenerSet, Event &event) {
    ListenerIterator end = listenerSet.end();
    for (ListenerIterator listener = listenerSet.begin(); listener != end; ++listener) {
      (*listener)(&event);
    }
  }
};

#endif
