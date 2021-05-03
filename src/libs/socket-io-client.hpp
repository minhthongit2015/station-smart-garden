
#pragma once
#ifndef __SOCKET_IO_CLIENT_H__
#define __SOCKET_IO_CLIENT_H__

#define SSL_AXTLS

#include <Arduino.h>
#include <map>
#include <vector>
#include <WebSocketsClient.h>

#ifndef NODEBUG_SOCKETIOCLIENT
   #define SOCKETIOCLIENT_DEBUG(...) Serial.printf(__VA_ARGS__);
#else
   #define SOCKETIOCLIENT_DEBUG(...)
#endif

#define PING_INTERVAL 20000 //TODO: use socket.io server response

//#define SOCKETIOCLIENT_USE_SSL
#ifdef SOCKETIOCLIENT_USE_SSL
  #define DEFAULT_PORT 443
#else
  #define DEFAULT_PORT 80
#endif
#define DEFAULT_URL "/socket.io/?transport=websocket"
#define DEFAULT_FINGERPRINT ""


#define PING "2"
#define PONG "3"

struct EventNameComparator {
  bool operator () (const char *eventName1, const char *eventName2) const {
    return strcmp(eventName1, eventName2) < 0;
  }
};

class SocketIoClient {
private:
  char buffer[256];
  WebSocketsClient _webSocket;
  int _lastPing = -PING_INTERVAL;
  std::map<const char*, std::function<void (const char *payload, size_t length)>, EventNameComparator> _events;

  void trigger(const char *event, const char *payload, size_t length);
  void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
  void initialize();
  void ping();
  void pong();
public:
  bool isConnected = false;
  void beginSSL(const char *host, const int port = DEFAULT_PORT, const char *url = DEFAULT_URL, const char *fingerprint = DEFAULT_FINGERPRINT);
  void begin(const char *host, const int port = DEFAULT_PORT, const char *url = DEFAULT_URL);
  void loop();
  void on(const char *event, std::function<void (const char *payload, size_t length)>);
  void emit(const char *event, const char *payload = NULL);
  void remove(const char *event);
  void disconnect();
  void setAuthorization(const char *user, const char *password);
};

// --------------------------------------------------------------------


char *getEventName(char *payload) {
  size_t length = strlen((char*)payload);
  int end = 0;
  for (uint8_t i = 4; i < length; i++) {
    if (payload[i] == '\"') {
      end = i;
      break;
    }
  }
  payload[end] = '\0';
  return &payload[4];
}

char *getEventPayload(char *payload) {
  size_t end = strlen(payload) - 1;
  if (end < 0) return NULL;
  payload[end] = '\0';
  if (payload[end - 1] == '\"') {
    payload[end - 1] = '\0';
  }
  if (payload[0] == '\"') {
    return payload + 1;
  }
  return payload;
}

static void hexdump(const uint32_t* src, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    Serial.printf("%08x ", *src);
    ++src;
    if ((i + 1) % 4 == 0) {
      Serial.printf("\n");
    }
  }
}

void SocketIoClient::webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  #define check1(payload, c1) (payload[0] == c1)
  #define check2(payload, c1, c2) (payload[0] == c1 && payload[1] == c2)
  #define isEvent(payload) check2(payload, '4', '2')
  #define isEventResponse(payload) check2(payload, '4', '3')
  #define isPing(payload) check1(payload, '2')
  #define isConnectEvent(payload) check2(payload, '4', '0')
  #define isDisconnectEvent(payload) check2(payload, '4', '1')
  static char *eventName, *eventPayload;

  switch(type) {
    case WStype_DISCONNECTED:
      isConnected = false;
      SOCKETIOCLIENT_DEBUG("[SIoC] Disconnected from url %s!\n", payload);
      trigger("disconnect", NULL, 0);
      break;
    case WStype_CONNECTED:
      SOCKETIOCLIENT_DEBUG("[SIoC] Connected to url: %s\n",  payload);
      break;
    case WStype_TEXT:
      SOCKETIOCLIENT_DEBUG("[SIoC] > Receive: %s\n", payload);
      if (isEvent(payload)) {
        eventName = getEventName((char*)payload);
        eventPayload = getEventPayload((char*)(payload) + strlen(eventName) + 6);
        trigger(eventName, eventPayload, length);
      } else if (isPing(payload)) {
        pong();
        SOCKETIOCLIENT_DEBUG("[SIoC] Pong!\n");
      } else if (isConnectEvent(payload)) {
        isConnected = true;
        _lastPing = -PING_INTERVAL;
        ping();
        trigger("connect", NULL, 0);
      } else if (isDisconnectEvent(payload)) {
        isConnected = false;
        trigger("disconnect", NULL, 0);
      }
      break;
    case WStype_BIN:
      SOCKETIOCLIENT_DEBUG("[SIoC] get binary length: %u\n", length);
      hexdump((uint32_t*) payload, length);
    break;
  }
}

void SocketIoClient::beginSSL(const char *host, const int port, const char *url, const char *fingerprint) {
  // invalid conversion from 'const char*' to 'const uint8_t*
  // const uint8_t* hostz = new uint8_t[strlen(host)];
  // size_t i = 0;
  // const char* beg = msg1;
  // const char* end = msg1 + length;
  // for (; beg != end; ++beg, ++i) {
  //   hostz[i] = (uint8_t)(*beg);
  // }
  _webSocket.beginSSL(host, port, url, fingerprint);
  initialize();
}

void SocketIoClient::begin(const char *host, const int port, const char *url) {
  _webSocket.begin(host, port, url);
  initialize();
}

void SocketIoClient::initialize() {
  _webSocket.onEvent(std::bind(&SocketIoClient::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void SocketIoClient::loop() {
  _webSocket.loop();
  ping();
}

void SocketIoClient::ping() {
  if (isConnected && millis() - _lastPing > PING_INTERVAL) {
    _webSocket.sendTXT(PING);
    _lastPing = millis();
  }
}

void SocketIoClient::pong() {
  _webSocket.sendTXT(PONG);
}

void SocketIoClient::on(const char *event, std::function<void (const char *payload, size_t length)> func) {
  _events[event] = func;
  // _events.insert(event, func);
}

void SocketIoClient::emit(const char *event, const char *payload) {
  if (!isConnected) {
    return;
  }
  static char buffer[512] = { "\0" };
  sprintf(buffer, "42[\"%s\",%s]", event, payload ? payload : "");
  SOCKETIOCLIENT_DEBUG("[SIoC] > Emit: %s\n", buffer);
  _webSocket.sendTXT(buffer);
}

void SocketIoClient::remove(const char *event) {
  auto evt = _events.find(event);
  if (evt != _events.end()) {
    _events.erase(evt);
  } else {
    SOCKETIOCLIENT_DEBUG("[SIoC] event %s not found, can not be removed", event);
  }
}

void SocketIoClient::trigger(const char *event, const char *payload, size_t length) {
  auto evt = _events.find(event);
  if (evt != _events.end()) {
    SOCKETIOCLIENT_DEBUG("[SIoC] trigger event %s\n", event);
    evt->second(payload, length);
  } else {
    SOCKETIOCLIENT_DEBUG("[SIoC] event %s not found. %d events available\n", event, _events.size());
  }
}

void SocketIoClient::disconnect() {
  _webSocket.disconnect();
  trigger("disconnect", NULL, 0);
}

void SocketIoClient::setAuthorization(const char *user, const char *password) {
  _webSocket.setAuthorization(user, password);
}

#endif
