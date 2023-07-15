#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <vector>

#include "../toolkit/Central.h"
#include "../toolkit/base/Color.h"
#include "../toolkit/Tracer.h"
#include "./LauncherEntry.h"
#include "cairo.h"

struct Launcher {
  int x;
  int y;
  int width;
  int height;
  Color bgColor;

  Central *central;
  Display *display;
  Window window;
  Window rootWindow;

  cairo_surface_t *sfc;
  cairo_t *cr;
  Tracer *tr = nullptr;

  LauncherEntry startButton;

  std::vector<LauncherEntry> entries;

  Launcher(
    Central *ct
  );

  void handleXEvent(const XEvent evt);

  void draw();

  void handleClick();

  void launchProgram(const std::string &command);

 private:
  void createWindow();
  void setupCairo();
};
