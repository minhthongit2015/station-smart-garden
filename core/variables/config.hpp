
#ifndef DEVICE_ID
#define DEVICE_ID "ESP_Device"
#endif

#ifndef CONTROL
#define CONTROL {}
#endif

#ifdef SENSOR_DHT22
#include "DHT.h"
#ifndef DHTTYPE
#define DHTTYPE DHT22
#endif
DHT dht(SENSOR_DHT22, DHTTYPE);
#endif

#ifdef SENSOR_BH1750
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter(0x23);
#ifndef SDA
#define SDA 4
#endif
#ifndef SCL
#define SCL 5
#endif
#endif

#ifdef SENSOR_HC_SR501
int sensorPin = 12;    // select the input pin for the potentiometer
int ledPin = 2;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
#endif