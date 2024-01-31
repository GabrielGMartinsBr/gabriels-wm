#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include "types.h"

namespace App {

class BaseWindow {
 public:
  BaseWindow(Display* display, Window parent)
  {
    this->display = display;
    this->parent = parent;
  }

  int x() { return _x; }
  BaseWindow& x(int v)
  {
    _x = v;
    updatePosition();
    return *this;
  }

  int y() { return _y; }
  BaseWindow& y(int v)
  {
    _y = v;
    updatePosition();
    return *this;
  }

  uint width() { return _width; }
  BaseWindow& width(uint v)
  {
    _width = v;
    updateSize();
    return *this;
  }

  uint height() { return _height; }
  BaseWindow& height(uint v)
  {
    _height = v;
    updateSize();
    return *this;
  }

  uint borderWidth() { return _borderWidth; }
  BaseWindow& borderWidth(uint v)
  {
    _borderWidth = v;
    updateBorderWidth();
    return *this;
  }

  ulong borderColor() { return _borderColor; }
  BaseWindow& borderColor(ulong v)
  {
    _borderColor = v;
    updateBorderColor();
    return *this;
  }

  ulong backgroundColor() { return _backgroundColor; }
  BaseWindow& backgroundColor(ulong v)
  {
    _backgroundColor = v;
    updateBackground();
    return *this;
  }

  BaseWindow& position(int x, int y)
  {
    _x = x;
    _y = y;
    updatePosition();
    return *this;
  }

  BaseWindow& rect(int x, int y, int width, int height)
  {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    updateSizeAndPosition();
    return *this;
  }

  BaseWindow& create()
  {
    createWindow();
    return *this;
  }

  BaseWindow& show()
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

  void createWindow()
  {
    window = XCreateSimpleWindow(
      display, parent,
      _x, _y, _width, _height,
      _borderWidth, _borderColor, _backgroundColor
    );
  }

  void mapWindow()
  {
    XMapWindow(display, window);
  }

  void updatePosition()
  {
    XMoveWindow(display, window, _x, _y);
  }

  void updateSize()
  {
    XResizeWindow(display, window, _width, _height);
  }

  void updateSizeAndPosition()
  {
    XMoveResizeWindow(display, window, _x, _y, _width, _height);
  }

  void updateBorderWidth()
  {
    XSetWindowBorderWidth(display, window, _borderWidth);
  }

  void updateBorderColor()
  {
    XSetWindowBorder(display, window, _borderColor);
  }

  void updateBackground()
  {
    XSetWindowBackground(display, window, _backgroundColor);
  }
};

}  // namespace App