#pragma once

#include <X11/Xlib.h>

#include <memory>

#include "base/BasicWindow.hpp"
#include "base/Color.hpp"
#include "base/consts.h"
#include "cairo-xlib.h"
#include "cairo.h"
#include "toolkit/base/Color.h"

namespace App {

class FrameComponent {
 public:
  FrameComponent(Display* dpy, Window parent) :
      window(dpy, parent)
  {
    this->display = dpy;
    this->parent = parent;
    createWindow();
    createSurface();
  }

  void show()
  {
    window.show();
    drawContent();
  }

 private:
  Display* display;
  Window parent;
  cairo_surface_t* sfc;
  cairo_t* cr;

  BasicWindow window;

  int x = 230;
  int y = 260;
  uint width = 640;
  uint height = 480;
  ulong bg = 0x333336;

  void createWindow()
  {
    window.rect(x, y, width, height)
      .backgroundColor(bg)
      .create();
  }

  void createSurface()
  {
    int screen = DefaultScreen(display);
    Visual* visual = DefaultVisual(display, screen);
    sfc = cairo_xlib_surface_create(
      display, window.xWindow(), visual, width, height
    );
    cr = cairo_create(sfc);
  }

  void drawContent()
  {
    Color bg(0x333334);
    Color color("#fff");

    cairo_set_source_rgb(cr, bg.red(), bg.green(), bg.blue());
    cairo_paint(cr);

    cairo_set_source_rgb(cr, color.red(), color.green(), color.blue());
    cairo_rectangle(cr, 16, 16, width - 32, height - 32);
    cairo_fill(cr);

    color.setColor("#fa3");
    cairo_set_source_rgb(cr, color.red(), color.green(), color.blue());
    cairo_arc(cr, 100, 100, 10, 0, PI2);
    cairo_fill(cr);
  }
};

}  // namespace App