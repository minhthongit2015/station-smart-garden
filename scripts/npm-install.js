const colors = require('colors');
const utils = require('./utils');
const installDependencies = require('./install-dependencies');

class BeyondGardenStation {
  static async setup() {
    this.printWelcome();
    await this.installDependencies();
  }

  static printWelcome() {
    utils.log('\x1B[2J\r\n\r\n');
    utils.log(colors.rainbow(`${new Array(80).fill('▬').join('')}\r\n`));
    utils.center(`${colors.yellow('BEYOND GARDEN STATION')}\r\n`);
    utils.log(colors.rainbow(`${new Array(80).fill('▬').join('')}\r\n`));
  }

  static async installDependencies() {
    utils.info(`${colors.yellow('Start The Installation')}`);
    await installDependencies();
    utils.info(`${colors.yellow('Installation Completed!')}`);
  }
}

BeyondGardenStation.setup();
