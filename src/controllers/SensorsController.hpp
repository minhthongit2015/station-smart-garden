
#pragma once
#ifndef BEYOND_GARDEN_SENSORS_CONTROLLER_H
#define BEYOND_GARDEN_SENSORS_CONTROLLER_H

#include "../utils/Utils.hpp"
#include "../libs/Listenable.hpp"
#include "../models/StationState.hpp"
#include "../helpers/ConfigHelper.hpp"
#include "../utils/PinMap.hpp"

#include "../modules/base/DHT22.hpp"
#include "../modules/base/BH1750.hpp"
#include "../modules/base/HCSR501.hpp"

#define PIN_DHT "PIN_DHT"
#define DHT_TYPE "DHT_TYPE"

#define PIN_HCSR_501 "PIN_HCSR_501"


void handleHuTempChange1(pEvent event);
void handleLightChange1(pEvent event);
void handleMovingChange1(pEvent event);

class SensorsController : public Listenable {
  private:
  public:
    HuTempDHT22 dht;
    LightBH1750 bh1750;
    MotionDetectorHCSR501 hcsr501;

    SensorsController() : Listenable() {
      defineEvent(HUTEMP_CHANGE);
      defineEvent(LIGHT_CHANGE);
      defineEvent(MOVING_CHANGE);
      defineEvent(VALUE_CHANGE);
    }

    void onHuTempChange(EventListener listener) {
      onEvent(HUTEMP_CHANGE, listener);
    }
    void onLightChange(EventListener listener) {
      onEvent(LIGHT_CHANGE, listener);
    }
    void onMovingChange(EventListener listener) {
      onEvent(MOVING_CHANGE, listener);
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
      bh1750.onChange(handleLightChange1);
      dht.onChange(handleHuTempChange1);
      // hcsr501.onChange(handleMovingChange1);
    }

    void loop() {
      bh1750.loop();
      dht.loop();
      // hcsr501.loop();
    }
} sensors;

extern SensorsController sensors;

void handleHuTempChange1(pEvent event) {
  prf("Temp: %f | Humi: %f\r\n",
    event->data.HuTemp.temperature, event->data.HuTemp.humidity);
  event->type = HUTEMP_CHANGE;
  sensors.dispatch(event);
}

void handleLightChange1(pEvent event) {
  prf("Light: %d\r\n", event->data.Light.light);
  event->type = LIGHT_CHANGE;
  sensors.dispatch(event);
}

void handleMovingChange1(pEvent event) {
  prf("Moving: %s\r\n", event->data.Moving.moving ? "true" : "false");
  event->type = MOVING_CHANGE;
  sensors.dispatch(event);
}

#endif