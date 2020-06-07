const path = require('path');
const simpleGit = require('simple-git/promise')(path.resolve('.'));
const utils = require('./utils');
const fs = require('fs-extra');


const InstallationType = {
  GIT: 'GIT'
};

const installationList = {
  ArduinoJSON: {
    type: InstallationType.GIT,
    path: 'https://github.com/bblanchon/ArduinoJson.git'
  },
  DHT: {
    type: InstallationType.GIT,
    path: 'https://github.com/adafruit/DHT-sensor-library.git'
  },
  LiquidCrystal_I2C: {
    type: InstallationType.GIT,
    path: 'https://github.com/johnrickman/LiquidCrystal_I2C.git'
  },
  TTP229: {
    type: InstallationType.GIT,
    path: 'https://github.com/arduino12/ttp229b-arduino.git',
    srcFolder: 'TTP229'
  },
  ArduinoWebSockets: {
    type: InstallationType.GIT,
    path: 'https://github.com/Links2004/arduinoWebSockets.git'
  },
  BH1750FVI: {
    type: InstallationType.GIT,
    path: 'https://github.com/PeterEmbedded/BH1750FVI.git'
  },
  AdafruitSensor: {
    type: InstallationType.GIT,
    path: 'https://github.com/adafruit/Adafruit_Sensor.git'
  },
  TDSSensor: {
    type: InstallationType.GIT,
    path: 'https://github.com/DFRobot/GravityTDS.git'
  },
  MFRC522: {
    type: InstallationType.GIT,
    path: 'https://github.com/miguelbalboa/rfid.git'
  }
};

const userProfile = process.env[(process.platform == 'win32') ? 'USERPROFILE' : 'HOME'];
const arduinoLibsFolder = `${userProfile}/Documents/Arduino/libraries`;

async function installDependencies() {
  return Promise.all(
    Object.entries(installationList)
      .map(([name, lib]) => {
        switch (lib.type) {
        case InstallationType.GIT: {
          utils.info(`${utils.colors.yellow('Installing')} "${name}"`);
          utils.info(`  ${lib.path}`);
          const targetFolder = path.join(arduinoLibsFolder, name);
          if (fs.existsSync(targetFolder)) {
            return null;
          }
          fs.removeSync(targetFolder);
          return simpleGit.clone(lib.path, targetFolder)
            .then(() => {
              if (lib.srcFolder) {
                fs.copySync(path.join(targetFolder, lib.srcFolder), targetFolder);
              }
            });
        }
        default:
          break;
        }
      })
  );
}

module.exports = installDependencies;
