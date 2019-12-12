
#pragma once
#ifndef BEYOND_GARDEN_SENSORS_CONTROLLER_H
#define BEYOND_GARDEN_SENSORS_CONTROLLER_H

#include "../base/utils.hpp"
#include "../base/types.hpp"
#include "../variables/State.hpp"

#include "../modules/DHT22.hpp"
#include "../modules/BH1750.hpp"
#include "../modules/HCSR501.hpp"

void onHuTempChange1(Event event);
void onLightChange1(Event event);
void onMovingChange1(Event event);

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
      dht.setup();
      // hcsr501.setup();
      
      setupListeners();
    }

    void setupListeners() {
      bh1750.onChange(onLightChange1);
      dht.onChange(onHuTempChange1);
      // hcsr501.onChange(onMovingChange);
    }

    void loop() {
      static unsigned long last = millis();
      static unsigned long dif = 0;

      dht.loop();
      bh1750.loop();
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

void onHuTempChange1(Event event) {
  if (logChannels[1]) prf("Temp: %.2f | Humi: %.2f\r\n",
    event.data.HuTemp.temperature, event.data.HuTemp.humidity);
  sensors.dispatch(event);
}

void onLightChange1(Event event) {
  if (logChannels[1]) prf("Light: %d\r\n", event.data.Light.light);
  sensors.dispatch(event);
}

void onMovingChange1(Event event) {
  if (logChannels[1]) prf("Moving: %s\r\n", event.data.Moving.moving ? "true" : "false");
  sensors.dispatch(event);
}

#endif