#include "FrameWindow.h"

typedef const FrameWindow* FInst;

enum ResizeDirection {
  NONE,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  HORIZONTAL,
  VERTICAL,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
};

struct FrameUtils {
  static ResizeDirection getResizeDirection(
    FInst f, int x, int y
  )
  {
    const int w = f->width;
    const int h = f->height;
    const int thick = 3;

    if (
      x < thick && y < thick
    ) {
      return ResizeDirection::UP_LEFT;
    }
    if (
      x > w - thick && x < w
      && y < thick
    ) {
      return ResizeDirection::UP_RIGHT;
    }
    if (
      x < thick
      && y > h - thick && y < h
    ) {
      return ResizeDirection::DOWN_LEFT;
    }
    if (
      x < w && x > w - thick
      && y > h - thick && y < h
    ) {
      return ResizeDirection::DOWN_RIGHT;
    }
    if (x < thick) {
      return ResizeDirection::LEFT;
    }
    if (x < w && x > w - thick) {
      return ResizeDirection::RIGHT;
    }
    if (y < thick) {
      return ResizeDirection::UP;
    }
    if (y > h - thick && y < h) {
      return ResizeDirection::DOWN;
    }
    return ResizeDirection::NONE;
  }
};