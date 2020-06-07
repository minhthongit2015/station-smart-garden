const colors = require('colors');

const LINE_LENGTH = 80;
function txtLen(text = '') {
  const trueText = Object.values(colors.styles)
    .reduce((txt, color) => {
      const regOpen = new RegExp(`${color.open.replace('[', '\\[')}`, 'g');
      const regClose = new RegExp(`${color.close.replace('[', '\\[')}`, 'g');
      return txt.replace(regOpen, '').replace(regClose, '');
    }, text)
    .replace(/\r|\n/g, '');
  return trueText.length;
}

module.exports = {
  colors: colors,
  log: console.log.bind(console),
  info: (text) => console.log(` > ${text}`),
  center: (text) => {
    console.log(''.padStart((LINE_LENGTH - txtLen(text)) / 2, ' ') + text);
  },
}