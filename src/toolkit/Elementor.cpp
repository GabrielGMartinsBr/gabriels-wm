#include "Elementor.h"

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#include <math.h>

#include "Button.h"
#include "Central.h"

Central Elementor::central;

void Elementor::init()
{
  central.init();
}

Button *Elementor::button(
  Window parent,
  int x, int y,
  int w, int h,
  unsigned long bgColor,
  unsigned long borderColor
)
{
  Button *button = new Button(
    &central,
    parent, x, y, w, h,
    bgColor, borderColor
  );
  return button;
}

Window Elementor::testWindow()
{
  XWindowAttributes winAttrs;

  Display *display = central.display;
  Window rootWindow = central.rootWindow;
  int screen = DefaultScreen(display);

  // XGetWindowAttributes(display, rootWindow, &winAttrs);
  // int h = 18;
  // int width = winAttrs.width;
  // int y = winAttrs.height - h;
  // Window w = XCreateSimpleWindow(
  //   central.display, central.rootWindow, 0, y, width, h, 0, 0, 0x333333
  // );
  Window window = XCreateSimpleWindow(
    display, rootWindow, 200, 100, 500, 350, 0, 0, 0xffffff
  );
  XMapWindow(display, window);

  int diameter = 12;
  int resolution = 720;
  int pixMapWidth = (diameter * resolution) / 72;
  int pixMapHeight = pixMapWidth;

  Pixmap pixMap = XCreatePixmap(display, window, diameter + 2, diameter + 2, DefaultDepth(display, screen));
  GC gc = XCreateGC(display, pixMap, 0, nullptr);

  // ==========================

  // Drawable da = pixMap;
  // cairo_surface_t *sfc;

  // auto visual = DefaultVisual(display, screen);
  // sfc = cairo_xlib_surface_create(
  //   display, da, visual, 60, 60
  // );
  // // cairo_xlib_surface_set_size(sfc, 100, 100);
  // cairo_t *cr = cairo_create(sfc);

  // int d = 12;

  // cairo_set_source_rgb(cr, .9, .9, .9);
  // // cairo_set_line_width(cr, 2);

  // cairo_arc(cr, 30, 30, d / 2.0, 0.0, 2 * M_PI);
  // // cairo_stroke(cr);
  // cairo_fill(cr);

  // cairo_rectangle(cr, 20, 20, 50, 50);
  // cairo_stroke(cr);

  // ==========================
  // XClearWindow(display, rootWindow);

  // XSetForeground(display, gc, 0xdddddd);
  // XFillRectangle(display, pixMap, gc, 0, 0, pixMapWidth, pixMapHeight);
  // XSetForeground(display, gc, 0x333333);

  // int cx = (pixMapWidth - diameter) / 2;
  // int cy = (pixMapHeight - diameter) / 2;
  // XFillArc(display, pixMap, gc, cx, cy, diameter, diameter, 0, 360 * 64);

  // ===========
  auto visual = DefaultVisual(display, screen);
  cairo_surface_t *sfc = cairo_xlib_surface_create(
    display, pixMap, visual, 60, 60
  );
  cairo_t *cr = cairo_create(sfc);
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  float r = diameter / 2.0;
  cairo_arc(cr, r + 1, r + 1, r, 0.0, 2 * M_PI);
  cairo_fill(cr);
  // ===========

  XCopyArea(display, pixMap, window, gc, 0, 0, diameter + 2, diameter + 2, 1, 1);
  XFlush(display);

  return window;
}