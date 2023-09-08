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
  static bool isLResize(
    FInst f, int x, int y
  )
  {
    return y > f->topHeight && x < 2;
  }

  static bool isRResize(
    FInst f, int x, int y
  )
  {
    return y > f->topHeight
           && x > f->width - 2;
  }

  static bool isBResize(
    FInst f, int x, int y
  )
  {
    return y > f->height - 3
           && y < f->height;
  }

  static ResizeDirection getResizeDirection(
    FInst f, int x, int y
  )
  {
    const int w = f->width;
    const int h = f->height;
    const int headerHeight = f->topHeight;
    const int thick = 3;

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