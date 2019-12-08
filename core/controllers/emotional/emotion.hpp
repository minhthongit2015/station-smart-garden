

#pragma once
#ifndef SMART_GARDEN_EMOTION_H
#define SMART_GARDEN_EMOTION_H

#include <set>
#include "../../base/types.hpp"
#include "../../modules/LiquidCrystal20x4.hpp"

typedef void (*emotionEndEventListener)();

class Emotion {
  private:
    std::set<emotionEndEventListener> _onEnd;
  public:
    std::set<pScreenFrame> frames;
    LiquidCrystal20x4 *_screen = NULL;
    std::set<pScreenFrame>::iterator curFrame;
    std::set<pScreenFrame>::iterator prevFrame;
    uint8_t offsetX = 0;
    uint8_t offsetY = 0;

    void setup(LiquidCrystal20x4 &screen) {
      this->_screen = &screen;
      curFrame = frames.end();
      prevFrame = frames.end();
    }

    void insert(pScreenFrame frame);

    void play();
    void pause();
    void replay();
    void drawFrame(pScreenFrame frame);
    void prepareFrame(pScreenFrame frame);
    void clearPrevFrame();
    void onEnd(emotionEndEventListener callback);
};


void Emotion::insert(ScreenFrame *frame) {
  frames.insert(frame);
}

void Emotion::play() {
  static timestamp_t last = 0;

  if (curFrame == frames.end()) {
    curFrame = frames.begin();
  }

  if (prevFrame == frames.end() || millis() - last > (**prevFrame).delay) {
    drawFrame(*curFrame);
    last = millis();
    prevFrame = curFrame;
    ++curFrame;
  }
}

void Emotion::drawFrame(ScreenFrame *frame) {
  clearPrevFrame();
  prepareFrame(frame);

  uint8_t size = (*frame).size;
  uint8_t *coords = (*frame).coords;

  for (int i = 0; i < size; i++) {
    (*_screen).setCursor(coords[i*3] + offsetX, coords[i*3 + 1] + offsetY);
    (*_screen).lcd->write(coords[i*3 + 2]);
  }
}

void Emotion::prepareFrame(pScreenFrame frame) {
  uint8_t size = (*frame).size;
  uint8_t *cells = (*frame).cells;

  for (uint8_t i = 0; i < size; i++) {
    (*_screen).lcd->createChar(i, &cells[i * 8]);
  }
}

void Emotion::clearPrevFrame() {
  if (prevFrame == frames.end()) return;

  uint8_t size = (**prevFrame).size;
  uint8_t *coords = (**prevFrame).coords;

  for (int i = 0; i < size; i++) {
    (*_screen).setCursor(coords[i*3] + offsetX, coords[i*3 + 1] + offsetY);
    (*_screen).lcd->print(' ');
  }
}

void Emotion::onEnd(emotionEndEventListener callback) {
  _onEnd.insert(callback);
}

#endif
