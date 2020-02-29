
#pragma once
#ifndef BEYOND_GARDEN_NETWORK_CONTROLLER_H
#define BEYOND_GARDEN_NETWORK_CONTROLLER_H


#include "../utils/Utils.hpp"
#include "../helpers/ConfigHelper.hpp"
#include "../models/StationState.hpp"
#include "../controllers/WifiManager.hpp"
#include "../utils/superws.hpp"

#define NETWORK_CONTROLLER "Network Controller"

#define GARDEN_HOST "GARDEN_HOST"
#define GARDEN_PORT "GARDEN_PORT"
#define SESSION_ID "SESSION_ID"

defineListener(handleConnected);


struct Network {
  void setup() {
    logStart(NETWORK_CONTROLLER);
    wifiMgr.setup();
    cfg.setDefault(GARDEN_HOST, "beyond-garden.herokuapp.com");
    cfg.setDefault(GARDEN_PORT, 5000);
    ws.setup(cfg.getCStrz(GARDEN_HOST), cfg.getLong(GARDEN_PORT), cfg.getCStrz(SESSION_ID));
    ws.onConnect(handleConnected);
  }
  void loop() {
    wifiMgr.loop();
    ws.loop();
  }
} network;

extern Network network;

void handleConnected(pEvent event) {
  cfg.set(SESSION_ID, event->data.Payload.payload).save();
  ws.emit(POST RecordsEndpoint, state.toJSON());
}

#endif