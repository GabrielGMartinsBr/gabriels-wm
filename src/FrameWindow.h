#pragma once

#include <X11/Xlib.h>

#include "toolkit/Button.h"
#include "toolkit/Central.h"
#include "toolkit/base/Color.h"

// TODO: Create Color Struct
// TODO: Create Small Title surface
// TODO:

class FrameWindow {
 public:
  static const int topHeight = 24;
  static const int borderWidth = 3;
  static const unsigned long BG_COLOR_HEX = 0x3f7faf;

  Color bgColor;

  int x;
  int y;
  int width;
  int height;
  bool maximized;

  const char *winName;

  Window frameWindow;
  Window contentWindow;

  cairo_surface_t *sfc;
  cairo_t *cr;

  cairo_surface_t *titleSfc;

  FrameWindow(
    Central *ct,
    Window w
  );

  void handleMaximizeClick();
  void maximize();
  void restoreSize();

  void handleXEvent(const XEvent evt);

 private:
  Central *central;
  Display *display;

  Button *closeButton;
  Button *maximizeButton;
  Button *minimizeButton;

  bool dragging;
  int dragInitX;
  int dragInitY;

  void setupCairo();

  void onExpose();

  void handleButtonPress(const XButtonPressedEvent evt);

  void handleButtonRelease(const XButtonReleasedEvent evt);

  void handleMotion(const XMotionEvent evt);

  void getWinAttrs();

  void drawTitle();

  void drawElements();

  void updateButtonsPosition();

  void handleButtonEvent(bool status, int x, int y);
  void handleMotionEvent(int x, int y);

  void startDrag(int x, int y);
  void updateDrag(int x, int y);
  void stopDrag(int x, int y);
};