
#pragma once
#ifndef BEYOND_GARDEN_SENSORS_CONTROLLER_H
#define BEYOND_GARDEN_SENSORS_CONTROLLER_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include "../variables/State.hpp"
#include "../controllers/ConfigManager.hpp"
#include "../base/pinmap.hpp"
#include "../base/events/Listenable.hpp"

#include "../modules/DHT22.hpp"
#include "../modules/BH1750.hpp"
#include "../modules/HCSR501.hpp"

#define PIN_DHT "PIN_DHT"
#define DHT_TYPE "DHT_TYPE"

#define PIN_HCSR_501 "PIN_HCSR_501"


void handleHuTempChange(pEvent event);
void handleLightChange(pEvent event);
void handleMovingChange(pEvent event);

class SensorsController : public Listenable {
  private:
  public:
    HuTempDHT22 dht;
    LightBH1750 bh1750;
    MotionDetectorHCSR501 hcsr501;
    ListenerSet hutempChangeListeners;
    ListenerSet lightChangeListeners;
    ListenerSet movingChangeListeners;

    SensorsController() : Listenable() {
      listenersMap.insert(ListenerPair(HUTEMP_CHANGE, &hutempChangeListeners));
      listenersMap.insert(ListenerPair(LIGHT_CHANGE, &lightChangeListeners));
      listenersMap.insert(ListenerPair(MOVING_CHANGE, &movingChangeListeners));
    }

    void onHuTempChange(EventListener listener) {
      hutempChangeListeners.insert(listener);
    }
    void onLightChange(EventListener listener) {
      lightChangeListeners.insert(listener);
    }
    void onMovingChange(EventListener listener) {
      movingChangeListeners.insert(listener);
    }

    void emulateState() {
      state.temperature = random(2500, 3500) / 100.0f;
      state.humidity = random(600, 900) / 10.0f;
      state.light = random(300, 1500);
    }

    void setup() {
      logStart("Sensors Controller");
      randomSeed(analogRead(0));

      bh1750.setup();

      cfg.setDefault(PIN_DHT, D5);
      cfg.setDefault(DHT_TYPE, DHT22);
      dht.setup(cfg.getUInt8(PIN_DHT), cfg.getUInt8(DHT_TYPE));

      cfg.setDefault(PIN_HCSR_501, NOT_A_PIN);
      // hcsr501.setup(cfg.getUInt8(PIN_HCSR_501));
      
      setupListeners();
    }

    void setupListeners() {
      bh1750.onChange(handleLightChange);
      dht.onChange(handleHuTempChange);
      // hcsr501.onChange(handleMovingChange);
    }

    void loop() {
      static unsigned long last = millis();
      static unsigned long dif = 0;

      bh1750.loop();
      dht.loop();
      // hcsr501.loop();
      // if (millis() - last > dif) {
      //   last = millis();
      //   dif = random(10000, 15000);
      //   this->emulateState();
      //   webSocket.emit("environment", state.toJSON());
      // }
    }
} sensors;

extern SensorsController sensors;

void handleHuTempChange(pEvent event) {
  if (logChannels[1]) prf("Temp: %f | Humi: %f\r\n",
    event->data.HuTemp.temperature, event->data.HuTemp.humidity);
  event->type = HUTEMP_CHANGE;
  sensors.dispatch(event);
}

void handleLightChange(pEvent event) {
  if (logChannels[1]) prf("Light: %d\r\n", event->data.Light.light);
  event->type = LIGHT_CHANGE;
  sensors.dispatch(event);
}

void handleMovingChange(pEvent event) {
  if (logChannels[1]) prf("Moving: %s\r\n", event->data.Moving.moving ? "true" : "false");
  event->type = MOVING_CHANGE;
  sensors.dispatch(event);
}

#endif