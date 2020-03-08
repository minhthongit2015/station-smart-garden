
#pragma once
#ifndef BEYOND_GARDEN_NETWORK_CONTROLLER_H
#define BEYOND_GARDEN_NETWORK_CONTROLLER_H


#include "../utils/Utils.hpp"
#include "../helpers/ConfigHelper.hpp"
#include "../models/StationState.hpp"
#include "../configs/Common.hpp"
#include "../controllers/WifiManager.hpp"
#include "../utils/superws.hpp"

#define NETWORK_CONTROLLER "Network Controller"

#define GARDEN_HOST "gardenHost"
#define GARDEN_PORT "gardenPort"
#define SESSION_ID "sessionId"

declareListener(handleConnected);


struct Network {
  void setup() {
    logStart(NETWORK_CONTROLLER);
    wifi.setup();

    cfg.setDefault(GARDEN_HOST, DEFAULT_HOST);
    cfg.setDefault(GARDEN_PORT, DEFAULT_PORT);
    ws.setup(cfg.getCStr(GARDEN_HOST), cfg.getLong(GARDEN_PORT), cfg.getCStr(SESSION_ID));
    ws.onConnect(handleConnected);
  }

  void loop() {
    wifi.loop();
    ws.loop();
  }
} network;

extern Network network;

declareListener(handleConnected) {
  if (!isBlank(event->data.Payload.payload)) {
    cfg.set(SESSION_ID, event->data.Payload.payload).save();
  }
  ws.emit(POST RecordsEndpoint, state.toJSON());
}

#endif