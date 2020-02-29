
#pragma once
#ifndef BEYOND_GARDEN_SUPER_WEBSOCKET_H
#define BEYOND_GARDEN_SUPER_WEBSOCKET_H

// #define NODEBUG_SOCKETIOCLIENT
#include "../libs/socket-io-client.hpp"
#include "../libs/Listenable.hpp"
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <set>
#include <map>
#include <iterator>
#include "../utils/Utils.hpp"
#include "../utils/Console.hpp"
#include "../configs/DeviceInfo.hpp"

#define WEBSOCKET "WebSocket"

#define defineWSListener(name) void name(const char *payload, size_t length)

defineWSListener(handleConnectEvent);
defineWSListener(handleDisconnectEvent);
defineWSListener(handleAcceptEvent);


class SuperWebsocket : public Listenable {
  private:
  bool testServer() {
    static WiFiClient testSocket;
    if (testSocket.connected()) {
      return true;
    }
    bool isServerUp = testSocket.connect(gardenHost, gardenPort);
    testSocket.stop();
    return isServerUp;
  }
  bool isInitialized = false;
  bool sleeping = false;
  timestamp_t waitBeforeSleep = 30000;
  timestamp_t checkOnSleepInterval = 30000;
  int maxRetryTimes = 10;
  char *gardenHost;
  long gardenPort;
  char *token;
  bool connected = false;

  public:
  SocketIoClient socket;
  bool isConnected() { return connected; }
  void setConnected(bool connected) {
    this->connected = connected;
  }
  bool isSleeping() { return sleeping; }
  void setSleeping(bool sleeping) {
    this->sleeping = sleeping;
  }

  SuperWebsocket() : Listenable() {
    defineEvent(CONNECTED);
    defineEvent(DISCONNECTED);
  }

  void setup(char *gardenHost, long gardenPort, char *token) {
    logStart(WEBSOCKET);
    configEndpoint(gardenHost, gardenPort, token);
    prf("> [" WEBSOCKET "] Connecting to -~=> %s : %d\r\n", gardenHost, gardenPort);
    if (wifiMgr.isConnected()) {
      initialize();
    }
  }

  void loop() {
    static int retryTimes = 0;
    if (!wifiMgr.isConnected()) {
      return;
    }

    if (!connected) {
      if (!sleeping) {
        if (Console::timeOver(WS_DISCONNECTED, waitBeforeSleep)) {
          sleeping = true;
          retryTimes = 0;
          Console::time(WS_CHECK_ON_SLEEP, "WS_CHECK_ON_SLEEP");
        }
      } else {
        if (Console::timeOver(WS_CHECK_ON_SLEEP, checkOnSleepInterval)) {
          if (++retryTimes > maxRetryTimes) {
            return reset();
          }
          if (!isInitialized) {
            initialize();
          } else {
            emit(POST VerifyStationEndpoint, DEVICE_INFO);
          }
          Console::time(WS_CHECK_ON_SLEEP);
        } else {
          return;
        }
      }
    }
    if (isInitialized) {
      socket.loop();
    }
  }

  bool initialize() {
    log(WEBSOCKET, "Initializing...");
    if (testServer()) {
      isInitialized = true;
      socket.begin(gardenHost, gardenPort, getConnectionUrlPath());
      setupEventListeners();
    }
    return isInitialized;
  }

  void emit(const char *event, const char *payload) {
    socket.emit(event, payload);
  }
  
  template<typename Lambda>
  void on(const char *event, Lambda &func) {
    socket.on(event, func);
  }

  void onConnect(EventListener listener) {
    onEvent(CONNECTED, listener);
  }

  void onDisconnect(EventListener listener) {
    onEvent(DISCONNECTED, listener);
  }

  void setupEventListeners() {
    on("connect", handleConnectEvent);
    on("disconnect", handleDisconnectEvent);
    on("accept", handleDisconnectEvent);
  }

  void configEndpoint(char *gardenHost, long gardenPort, char *token) {
    this->gardenHost = gardenHost;
    this->gardenPort = gardenPort;
    this->setToken(token);
  }

  void setToken(char *token) {
    this->token = token;
  }

  const char *getConnectionUrlPath() {
    static char urlPathBuffer[100];
    #define DEFAULT_URL_PATH "/socket.io/?EIO=3&transport=websocket"
    #define WITH_SESSION_URL_PATH DEFAULT_URL_PATH "&token=%s"
    
    if (!isBlank(this->token)) {
      sprintf(urlPathBuffer, WITH_SESSION_URL_PATH, this->token);
      return urlPathBuffer;
    } else {
      return DEFAULT_URL_PATH;
    }
  }
} ws;

extern SuperWebsocket ws;

void handleConnectEvent(const char *payload, size_t length) {
  log(WEBSOCKET, "Connected to the Garden!");
  ws.setSleeping(false);
  ws.emit(POST VerifyStationEndpoint, DEVICE_INFO);
}

void handleDisconnectEvent(const char *payload, size_t length) {
  if (ws.isConnected()) {
    log(WEBSOCKET, "Disconnected from the Garden!");
    Console::time(WS_DISCONNECTED, "WS_DISCONNECTED");
    ws.setConnected(false);
    ws.dispatch(DISCONNECTED);
  }
}

void handleAcceptEvent(const char *payload, size_t length) {
  log(WEBSOCKET, "Garden accepted!");
  static char token[50];
  strcpy(token, payload);
  ws.data.Payload.payload = token;
  ws.data.Payload.length = length;
  ws.setConnected(true);
  ws.setToken(token);
  ws.dispatch(CONNECTED, ws.data);
}

#endif
