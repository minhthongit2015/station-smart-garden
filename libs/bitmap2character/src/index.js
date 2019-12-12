
const fs = require('fs');
const path = require('path');
const bmp = require('bmp-js');
const walker = require('./walker');

function eq(colorA, colorB) {
  return Object.keys(colorA).every((k) => colorA[k] === colorB[k]);
}

function get(x, y, bitmap) {
  const pos = y * bitmap.width * 4 + x * 4;
  return {
    a: bitmap.data[pos],
    b: bitmap.data[pos + 1],
    g: bitmap.data[pos + 2],
    r: bitmap.data[pos + 3]
  };
}

class Converter {
  static async convertDir(dir) {
    const dirListing = walker(dir);

    dirListing.items.forEach((place) => {
      if (place.type === 'folder') {
        this.convertDir(dir);
      } else if (place.type === 'file') {
        this.convert(place);
      }
    });
  }

  static async convert(file) {
    if (path.extname(file.path) !== '.bmp') return null;
    const fileContent = fs.readFileSync(file.path);
    const bitmap = bmp.decode(fileContent);
    const gray = this.toGray(bitmap);
    const grid = this.grayToGrid(gray, bitmap);
    const cpp = this.gridToCpp(grid, file.path);
    return cpp;
  }

  static toGray(bitmap) {
    const bgColor = get(0, 0, bitmap);
    const gray = [];
    for (let y = 0; y < bitmap.height; y++) {
      if (!gray[y]) gray[y] = [];
      for (let x = 0; x < bitmap.width; x++) {
        const dot = get(x, y, bitmap);
        gray[y][x] = +!eq(dot, bgColor);
      }
    }
    return !bitmap.bottom_up ? gray.reverse() : gray;
  }

  static grayToGrid(gray, bitmap) {
    const grid = [];
    const rows = Math.ceil(bitmap.height / 8);
    const cols = Math.ceil(bitmap.width / 5);
    for (let r = 0; r < rows; r++) {
      if (!grid[r]) grid[r] = [];
      for (let c = 0; c < cols; c++) {
        grid[r][c] = this.getCell5x8(r * 8, c * 5, gray);
      }
    }
    return grid;
  }

  static getCell5x8(r, c, gray) {
    const cell = [];
    for (let row = 0; row < 8; row++) {
      if (!cell[row]) cell[row] = [];
      for (let col = 0; col < 5; col++) {
        cell[row][col] = gray[r + row][c + col];
      }
    }
    return cell;
  }

  static gridToCpp(grid, filePath) {
    filePath = filePath.replace(path.extname(filePath), '.hpp');

    const blackCells = [];
    grid.forEach((row, rowIndex) => {
      row.forEach((cell, colIndex) => {
        if (cell.some((r) => r.some((c) => c))) {
          blackCells.push({
            row: rowIndex,
            col: colIndex,
            cell
          });
        }
      });
    });
    const cellMap = blackCells.map((cell) => this.cellToCpp(cell.cell));
    const positionMap = blackCells.map((cell, cellIndex) => `${cell.col},${cell.row},${cellIndex}`);
    const basename = path.basename(filePath, '.hpp');

    const frameName = basename.replace(/-/g, '');
    const frameDelay = 500;
    const cpp = `
      #pragma once
      #ifndef BEYOND_GARDEN_EMOTION_${frameName.toUpperCase()}_H
      #define BEYOND_GARDEN_EMOTION_${frameName.toUpperCase()}_H

      #include "../../base/types.hpp"
      
      unsigned char ${frameName}_cells[] = {
        ${cellMap.join(',\r\n  ')}
      };
      unsigned char ${frameName}_coords[] = {
        ${positionMap.join(',\r\n  ')}
      };
      ScreenFrame ${frameName} = {
        ${blackCells.length}, ${frameName}_cells, ${frameName}_coords, ${frameDelay}
      };

      #endif
    `.trim().replace(/ {3,}/g, ' ');
    fs.writeFileSync(filePath.replace(/-/g, ''), cpp);
    return cpp;
  }

  static cellToCpp(cell) {
    const cpp = [];
    cell.forEach((r) => {
      cpp.push(`0b000${r.join('')}`);
    });
    return `${cpp.join(',')}`;
  }
}


const sleep = (ms = 999999999) => new Promise((resolve) => setTimeout(resolve, ms));
process.on('unhandledRejection', (reason) => {
  console.error(`Unhandled Rejection at: \t ${reason.stack || reason}`);
});
process.on('uncaughtException', (exeption) => {
  console.error(`Uncaught Exception at: \t ${exeption}`);
});

Converter.convertDir('images');

sleep();
