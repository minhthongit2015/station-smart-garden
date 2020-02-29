
#pragma once
#ifndef BEYOND_GARDEN_EMOTION_H
#define BEYOND_GARDEN_EMOTION_H

#include "../../libs/Listenable.hpp"
#include "../../models/ScreenFrame.hpp"
#include "../../modules/base/LiquidCrystal20x4.hpp"


class Emotion : public Listenable {
  public:
    Animation animation;
    AnimationFrame curFrame;
    AnimationFrame prevFrame;
    uint8_t offsetX = 0;
    uint8_t offsetY = 0;
    LiquidCrystal20x4 *pScreen;

    Emotion() : Listenable() {
      defineEvent(VALUE_CHANGE);
    }

    void setup(LiquidCrystal20x4 &screen) {
      curFrame = animation.end();
      prevFrame = animation.end();
      pScreen = &screen;
    }

    void insert(pScreenFrame frame);

    void play();
    void pause();
    void replay();
    void drawFrame(pScreenFrame frame);
    void prepareFrame(pScreenFrame frame);
    void clearPrevFrame();
    void onEnd(EventListener callback);
};


void Emotion::insert(ScreenFrame *frame) {
  animation.insert(frame);
}

void Emotion::play() {
  static timestamp_t last = 0;

  if (curFrame == animation.end()) {
    curFrame = animation.begin();
  }

  if (prevFrame == animation.end() || millis() - last > (*prevFrame)->delay) {
    drawFrame(*curFrame);
    last = millis();
    prevFrame = curFrame;
    ++curFrame;
  }
}

void Emotion::drawFrame(pScreenFrame pframe) {
  clearPrevFrame();
  prepareFrame(pframe);

  uint8_t size = pframe->size;
  uint8_t *coords = pframe->coords;

  for (int i = 0; i < size; i++) {
    (*pScreen).setCursor(coords[i*3] + offsetX, coords[i*3 + 1] + offsetY);
    (*pScreen).write(coords[i*3 + 2]);
  }
}

void Emotion::prepareFrame(pScreenFrame frame) {
  uint8_t size = (*frame).size;
  uint8_t *cells = (*frame).cells;

  for (uint8_t i = 0; i < size; i++) {
    (*pScreen).createChar(i, &cells[i * 8]);
  }
}

void Emotion::clearPrevFrame() {
  if (prevFrame == animation.end()) return;

  uint8_t size = (**prevFrame).size;
  uint8_t *coords = (**prevFrame).coords;

  for (int i = 0; i < size; i++) {
    (*pScreen).setCursor(coords[i*3] + offsetX, coords[i*3 + 1] + offsetY);
    (*pScreen).print(' ');
  }
}

void Emotion::onEnd(EventListener callback) {
  onEvent(VALUE_CHANGE, callback);
}

#endif
