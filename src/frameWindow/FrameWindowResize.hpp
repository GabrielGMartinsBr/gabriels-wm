#include "FrameWindowBase.hpp"
#include "ResizeDirection.h"

typedef FrameResizeDirection ResizeDir;

class FrameWindowResize : public FrameWindowBase {
  int start_X;
  int start_Y;
  int startX;
  int startY;
  int startW;
  int startH;

 protected:
  ResizeDir resizeDirection;
  bool isResizing;

  bool startResize(int _x, int _y)
  {
    resizeDirection = getResizeDirection(_x, _y);
    isResizing = resizeDirection != ResizeDir::NONE;
    if (isResizing) {
      start_X = _x;
      start_Y = _y;
      startX = x;
      startY = y;
      startW = width;
      startH = height;
    }
    return isResizing;
  }

  void stopResize()
  {
    isResizing = false;
  }

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

  void handleResizeMotion(int _x, int _y)
  {
    if (!isResizing) {
      return;
    }

    int dx = _x - start_X;
    int dy = _y - start_Y;

    switch (resizeDirection) {
      case LEFT:
        x += dx;
        width = startW + (startX - x);
        break;
      case RIGHT:
        width = startW + dx;
        break;
      case UP:
        y += dy;
        height = startH + (startY - y);
        break;
      case DOWN:
        height = startH + dy;
        break;
      case UP_LEFT:
        x += dx;
        width = startW + (startX - x);
        y += dy;
        height = startH + (startY - y);
        break;
      case UP_RIGHT:
        width = startW + dx;
        y += dy;
        height = startH + (startY - y);
        break;
      case DOWN_LEFT:
        x += dx;
        width = startW + (startX - x);
        height = startH + dy;
        break;
      case DOWN_RIGHT:
        width = startW + dx;
        height = startH + dy;
        break;
      default:
        break;
    }

    if (width < 96) {
      width = 96;
      int eX = startX + startW - 96;
      if (x > eX) {
        x = eX;
      }
    }
    if (height < 64) {
      height = 64;
      int eY = startY + startH - 64;
      if (y > eY) {
        y = eY;
      }
    }
  }
};
