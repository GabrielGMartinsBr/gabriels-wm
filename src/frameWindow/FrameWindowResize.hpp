#include "FrameWindowBase.hpp"
#include "ResizeDirection.h"

typedef FrameResizeDirection ResizeDir;

class FrameWindowResize : public FrameWindowBase {
 protected:
  ResizeDir getResizeDirection(int _x, int _y)
  {
    if (
      _x < border
      && _y < border
    ) {
      return ResizeDir::UP_LEFT;
    }
    if (
      _x > width - border
      && _x < width
      && _y < border
    ) {
      return ResizeDir::UP_RIGHT;
    }
    if (
      _x < border
      && _y > height - border
      && _y < height
    ) {
      return ResizeDir::DOWN_LEFT;
    }
    if (
      _x < width
      && _x > width - border
      && _y > height - border
      && _y < height
    ) {
      return ResizeDir::DOWN_RIGHT;
    }
    if (_x < border) {
      return ResizeDir::LEFT;
    }
    if (
      _x < width
      && _x > width - border
    ) {
      return ResizeDir::RIGHT;
    }
    if (_y < border) {
      return ResizeDir::UP;
    }
    if (
      _y > height - border
      && _y < height
    ) {
      return ResizeDir::DOWN;
    }
    return ResizeDir::NONE;
  }
};
