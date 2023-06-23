#include "Elementor.h"

#include <X11/Xlib.h>

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

  int diameter = 16;
  int resolution = 720;
  int pixMapWidth = (diameter * resolution) / 72;
  int pixMapHeight = pixMapWidth;

  // XClearWindow(display, rootWindow);

  Pixmap pixMap = XCreatePixmap(display, window, pixMapWidth, pixMapHeight, DefaultDepth(display, screen));
  GC gc = XCreateGC(display, pixMap, 0, nullptr);

  XSetForeground(display, gc, 0xdddddd);
  XFillRectangle(display, pixMap, gc, 0, 0, pixMapWidth, pixMapHeight);
  XSetForeground(display, gc, 0x333333);

  int cx = (pixMapWidth - diameter) / 2;
  int cy = (pixMapHeight - diameter) / 2;
  XFillArc(display, pixMap, gc, cx, cy, diameter, diameter, 0, 360 * 64);

  XCopyArea(display, pixMap, window, gc, 0, 0, pixMapWidth, pixMapHeight, 10, 10);
  XFlush(display);

  return window;
}