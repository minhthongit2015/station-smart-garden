#include "./core/base/utils.hpp"
#include "./core/station.hpp"

SmartGardenStation station;

void setup()
{
  helperSetup();

  prl("\x1B[2J\r\n\r\n\r\n\r\n");
  prl("           - - - - - - <*> - - - - - -");
  prl("           -     Smart Garden V1     -");
  prl("           - - - - - - - - - - - - - -\r\n");

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
