#include "./core/base/utils.hpp"
#include "./core/station.hpp"

SmartGardenStation station;

void setup()
{
  helperSetup();
  #ifdef ARDUINO
    pr(" ---> ARDUINO: "); prl(ARDUINO);
  #endif

  // setupLed then sleep(2) offLed1 offLed2
  
  performanceDisable(0);
  station.setup();
}

void loop()
{
  station.loop();
}
