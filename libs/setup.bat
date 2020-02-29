@echo off

set LIBS_FOLDER=%USERPROFILE%/Documents/Arduino/libraries

REM Extract lib packages and install

REM set "sources=BH1750FVI DHT_sensor_library LiquidCrystal_I2C ttp229 ArduinoJson-6.x"
set "LIBS=BH1750FVI ArduinoJson-6.x"

(for %%lib in (%LIBS%) do (
  set command=xcopy "./%%lib" "%LIBS_FOLDER%/%%lib" /E /I /H /K /Y
  echo %command%
  %command%
))


REM --------------------------------------------------------------------------


REM Clone libs from github and install
set GIT_TTP229=https://github.com/arduino12/ttp229-arduino.git

set "LIB_GITS=%GIT_TTP229%"

(for %%lib in (%LIB_GITS%) do (
  set command=git clone %%lib "%LIBS_FOLDER%"
  echo %command%
  %command%
))
