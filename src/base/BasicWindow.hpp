#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <stdexcept>

#include "types.h"

namespace App {

class BasicWindow {
 public:
  BasicWindow(Display* display, Window parent)
  {
    this->display = display;
    this->parent = parent;
  }

  int x() { return _x; }
  BasicWindow& x(int v)
  {
    _x = v;
    updatePosition();
    return *this;
  }

  int y() { return _y; }
  BasicWindow& y(int v)
  {
    _y = v;
    updatePosition();
    return *this;
  }

  uint width() { return _width; }
  BasicWindow& width(uint v)
  {
    _width = v;
    updateSize();
    return *this;
  }

  uint height() { return _height; }
  BasicWindow& height(uint v)
  {
    _height = v;
    updateSize();
    return *this;
  }

  uint borderWidth() { return _borderWidth; }
  BasicWindow& borderWidth(uint v)
  {
    _borderWidth = v;
    updateBorderWidth();
    return *this;
  }

  ulong borderColor() { return _borderColor; }
  BasicWindow& borderColor(ulong v)
  {
    _borderColor = v;
    updateBorderColor();
    return *this;
  }

  ulong backgroundColor() { return _backgroundColor; }
  BasicWindow& backgroundColor(ulong v)
  {
    _backgroundColor = v;
    updateBackground();
    return *this;
  }

  BasicWindow& position(int x, int y)
  {
    _x = x;
    _y = y;
    updatePosition();
    return *this;
  }

  BasicWindow& rect(int x, int y, int width, int height)
  {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    updateSizeAndPosition();
    return *this;
  }

  BasicWindow& create()
  {
    createWindow();
    return *this;
  }

  BasicWindow& show()
  {
    mapWindow();
    return *this;
  }

 private:
  int _x = 0;
  int _y = 0;
  uint _width = 0;
  uint _height = 0;
  uint _borderWidth = 0;
  ulong _borderColor = 0x0;
  ulong _backgroundColor = 0x0;

  Display* display;
  Window parent;
  Window window;

  bool created = false;

  void createWindow()
  {
    window = XCreateSimpleWindow(
      display, parent,
      _x, _y, _width, _height,
      _borderWidth, _borderColor, _backgroundColor
    );
    created = true;
  }

  void mapWindow()
  {
    if (!created) {
      throw std::runtime_error("Window was not created yet.");
    }
    XMapWindow(display, window);
  }

  void updatePosition()
  {
    if (!created) {
      return;
    }
    XMoveWindow(display, window, _x, _y);
  }

  void updateSize()
  {
    if (!created) {
      return;
    }
    XResizeWindow(display, window, _width, _height);
  }

  void updateSizeAndPosition()
  {
    if (!created) {
      return;
    }
    XMoveResizeWindow(display, window, _x, _y, _width, _height);
  }

  void updateBorderWidth()
  {
    if (!created) {
      return;
    }
    XSetWindowBorderWidth(display, window, _borderWidth);
  }

  void updateBorderColor()
  {
    if (!created) {
      return;
    }
    XSetWindowBorder(display, window, _borderColor);
  }

  void updateBackground()
  {
    if (!created) {
      return;
    }
    XSetWindowBackground(display, window, _backgroundColor);
  }
};

}  // namespace App