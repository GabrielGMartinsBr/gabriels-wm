#pragma once

#include <X11/Xlib.h>
#include <X11/extensions/render.h>

#include "cairo.h"

struct Compositor {
  Display* display;
  Window root;

  Window srcWindow;
  Window dstWindow;
  Window child;

  int w;
  int h;

  bool drawn = false;


  Picture srcPicture;
  Picture dstPicture;

  cairo_surface_t* surface;
  cairo_t* cr;

  Compositor();
  ~Compositor();

  int run();

  void drawOnSrc();

  void render();
};