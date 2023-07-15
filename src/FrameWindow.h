#pragma once

#include <X11/Xlib.h>

#include <cmath>

#include "toolkit/Central.h"
#include "toolkit/Tracer.h"
#include "toolkit/base/Color.h"

// TODO: Create Color Struct
// TODO: Create Small Title surface
// TODO:

class FrameWindow {
  struct CircleButton {
    int x;
    int y;
    int radius;
    Color color;

    const static int PI2 = 2 * M_PI;

    CircleButton(int colorHex);

    void draw(Tracer *tr);

    bool isHover(int _x, int _y);
  };

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
  Tracer *tr;

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

  CircleButton closeButton;
  CircleButton maximizeButton;
  CircleButton minimizeButton;

  bool isDragging;
  int dragInitX;
  int dragInitY;
  bool cursorChanged;
  bool isCairoMaximized;

  void setupCairo();

  void onExpose();

  void handleButtonPress(const XButtonPressedEvent evt);

  void handleButtonRelease(const XButtonReleasedEvent evt);

  void handleMotion(const XMotionEvent evt);

  void getWinAttrs();

  void drawTitle();

  void drawFrameButtons();

  void drawElements();

  void startDrag(int x, int y);
  void updateDrag(int x, int y);
  void stopDrag(int x, int y);
};
