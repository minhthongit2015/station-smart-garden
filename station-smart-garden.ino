#include "./core/base/utils.hpp"
#include "./core/station.hpp"

SmartGardenStation station;

void setup()
{
  helperSetup();
  #ifdef ARDUINO
    pr("\r\n ---> ARDUINO: "); prl(ARDUINO);
  #endif

  // setupLed then sleep(2) offLed1 offLed2
  
  station.setup();
}

void loop()
{
  station.loop();
}
