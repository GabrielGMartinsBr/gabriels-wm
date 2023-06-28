#pragma once

#include <X11/Xlib.h>

#include "toolkit/Button.h"
#include "toolkit/Central.h"

class FrameWindow {
 public:
  static const int topHeight = 24;
  static const int borderWidth = 3;
  static const unsigned long bgColor = 0x3f7faf;

  int x;
  int y;
  int width;
  int height;
  bool maximized;

  Window frameWindow;
  Window contentWindow;

  FrameWindow(
    Central *ct,
    Window w
  );

  void handleMaximizeClick();
  void maximize();
  void restoreSize();

  void handleButtonEvent(bool status, int x, int y);
  void handleMotionEvent(int x, int y);

 private:
  Central *central;
  Display *display;

  Button *closeButton;
  Button *maximizeButton;
  Button *minimizeButton;

  bool dragging;
  int dragInitX;
  int dragInitY;

  void updateButtonsPosition();

  void startDrag(int x, int y);
  void updateDrag(int x, int y);
  void stopDrag(int x, int y);
};