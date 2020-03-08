
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
#include "../utils/Constants.hpp"
#include "../configs/Common.hpp"
#include "../configs/DeviceInfo.hpp"

#define WEBSOCKET "WebSocket"

#define declareWSListener(name) void name(const char *payload, size_t length)

declareWSListener(handleConnectEvent);
declareWSListener(handleDisconnectEvent);
declareWSListener(handleAuthorizedEvent);
declareWSListener(handleUnauthorizedEvent);


class SuperWebsocket : public Listenable {
  private:
  bool testServer() {
    static WiFiClient testSocket;
    bool isServerUp = testSocket.connect(gardenHost, gardenPort);
    testSocket.stop();
    if (isServerUp) {
      log(WEBSOCKET, "Server is up!");
    } else {
      log(WEBSOCKET, "Server is down!")
    }
    return isServerUp;
  }
  bool isInitialized = false;

  bool sleeping = false;
  timestamp_t waitBeforeSleep = 30000;
  timestamp_t checkOnSleepInterval = 30000;
  int maxRetryTimes = 10;

  const char *gardenHost;
  long gardenPort;

  const char *token;
  bool authorized = false;

  public:
  SocketIoClient socket;
  bool isConnected() { return socket.isConnected; }

  bool isAuthorized() { return authorized; }
  void setAuthorized(bool authorized) {
    this->authorized = authorized;
  }

  bool isSleeping() { return sleeping; }
  void setSleeping(bool sleeping) {
    this->sleeping = sleeping;
  }

  SuperWebsocket() : Listenable() {
    defineEvent(CONNECTED);
    defineEvent(DISCONNECTED);
  }

  void setup(const char *gardenHost, long gardenPort, const char *token) {
    logStart(WEBSOCKET);
    configEndpoint(gardenHost, gardenPort, token);
    if (!isValidEndpoint(gardenHost, gardenPort)) {
      logf(WEBSOCKET, "Unknow Garden: %s : %d\r\n", gardenHost, gardenPort);
      return;
    }
    logf(WEBSOCKET, "Set Garden to -~=> %s : %d\r\n", gardenHost, gardenPort);
    logf(WEBSOCKET, "> with Token: %s\r\n", token);
    if (wifi.isConnected()) {
      initialize();
    }
    Console::time(WS_DISCONNECTED);
  }

  void configEndpoint(const char *gardenHost, long gardenPort, const char *token) {
    this->gardenHost = gardenHost;
    this->gardenPort = gardenPort;
    this->setToken(token);
  }

  bool isValidEndpoint(const char *gardenHost, long gardenPort) {
    return gardenPort > 0 && !isBlank(gardenHost) && strlen(gardenHost) > 5;
  }

  void setToken(const char *token) {
    this->token = token;
    setAuthorized(!isBlank(token));
  }

  bool initialize() {
    if (!isValidEndpoint(gardenHost, gardenPort)) {
      logf(WEBSOCKET, "Unknow Garden: %s : %d\r\n", gardenHost, gardenPort);
      return false;
    }
    log(WEBSOCKET, "Initializing...");
    if (testServer()) {
      isInitialized = true;
      socket.begin(gardenHost, gardenPort, getConnectionUrlPath());
      setupEventListeners();
    }
    return isInitialized;
  }

  void setupEventListeners() {
    on(WSEvent.CONNECT, handleConnectEvent);
    on(WSEvent.DISCONNECT, handleDisconnectEvent);
    on(WSEvent.AUTHORIZED, handleAuthorizedEvent);
    on(WSEvent.UNAUTHORIZED, handleUnauthorizedEvent);
  }

  void loop() {
    static int retryTimes = 0;
    if (!wifi.isConnected()) {
      return;
    }

    if (!isConnected()) {
      if (!sleeping) {
        if (Console::timeOver(WS_DISCONNECTED, waitBeforeSleep)) {
          log(WEBSOCKET, "Sleeping...");
          setSleeping(true);
          retryTimes = 0;
          Console::time(WS_CHECK_ON_SLEEP, "WS_CHECK_ON_SLEEP");
        }
      } else {
        if (Console::timeOver(WS_CHECK_ON_SLEEP, checkOnSleepInterval)) {
          log(WEBSOCKET, "Wake up at the mid night");
          if (++retryTimes > maxRetryTimes) {
            return reset();
          }
          if (!isInitialized) {
            if (initialize()) {
              setSleeping(false);
            }
          } else {
            if (testServer()) {
              authorizeStationWithServer();
              setSleeping(false);
            }
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

  void authorizeStationWithServer() {
    if (isConnected()) {
      emit(POST AuthorizeStationEndpoint, DEVICE_INFO);
    }
  }

  void emit(const char *event, const char *payload) {
    socket.emit(event, payload);
  }
  
  template<typename Lambda>
  void on(const char *event, Lambda &callback) {
    socket.on(event, callback);
  }

  void onConnect(EventListener listener) {
    onEvent(CONNECTED, listener);
  }

  void onDisconnect(EventListener listener) {
    onEvent(DISCONNECTED, listener);
  }

  const char *getConnectionUrlPath() {
    static char urlPathBuffer[WS_HOST_URL_PATH_SIZE];
    #define DEFAULT_URL_PATH "/socket.io/?EIO=3&transport=websocket&fromStation=true"
    #define URL_PATH_WITH_TOKEN DEFAULT_URL_PATH "&token=%s"
    
    if (!isBlank(this->token)) {
      sprintf(urlPathBuffer, URL_PATH_WITH_TOKEN, this->token);
      return urlPathBuffer;
    } else {
      return DEFAULT_URL_PATH;
    }
  }
} ws;

extern SuperWebsocket ws;

declareWSListener(handleConnectEvent) {
  log(WEBSOCKET, "Connected to the Garden!");
  ws.setSleeping(false);
  if (!ws.isAuthorized()) {
    ws.authorizeStationWithServer();
  } else {
    ws.dispatch(CONNECTED);
  }
}

declareWSListener(handleDisconnectEvent) {
  if (ws.isConnected()) {
    log(WEBSOCKET, "Disconnected from the Garden!");
    Console::time(WS_DISCONNECTED, "WS_DISCONNECTED");
    ws.dispatch(DISCONNECTED);
  }
}

declareWSListener(handleAuthorizedEvent) {
  log(WEBSOCKET, "Garden authorized!");
  static char token[WS_TOKEN_SIZE];
  strcpy(token, payload);
  EventData data;
  data.Payload.payload = token;
  data.Payload.length = length;
  ws.setToken(token);
  ws.setAuthorized(true);
  ws.dispatch(CONNECTED, data);
}

declareWSListener(handleUnauthorizedEvent) {
  log(WEBSOCKET, "Garden unauthorized!");
  delay(5000);
  ws.authorizeStationWithServer();
}

#endif
