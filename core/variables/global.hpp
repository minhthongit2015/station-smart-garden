
/**
 * Usage: Manually include after utils.hpp
 * - Define every global variable
 * EX:
 * #include "./base/utils.hpp"
 * #include "./variables/global.hpp"
*/

#pragma once
#ifndef SMART_GARDEN_GLOBAL_H
#define SMART_GARDEN_GLOBAL_H

#include "./config.hpp"

#include <SocketIoClient.h>
bool wsConnected = false;
SocketIoClient webSocket;

#include "../modules/LCDScreen.hpp"
LCDScreen lcd;

#include "../modules/TTP229.hpp"
TouchPadTTP229 touchPad;

#include "../modules/RelayController.hpp"
RelayController relayCtl;

#include "../modules/DHT22.hpp"
HuTempDHT22 dht;

#include "../modules/BH1750.hpp"
LightBH1750 bh1750;

#include "../modules/HCSR501.hpp"
MovingHCSR501 hcsr501;

#endif