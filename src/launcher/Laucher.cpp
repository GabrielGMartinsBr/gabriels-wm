#include <X11/Xlib.h>

#include <iostream>

#include "Launcher.h"
#include "cairo-xlib.h"
#include "cairo.h"

Launcher::Launcher(const Central* ct) :
    bgColor("#333"),
    startButton("Iniciar")
{
  central = ct;
  display = ct->display;
  rootWindow = ct->rootWindow;

  XWindowAttributes winAttrs;
  XGetWindowAttributes(display, rootWindow, &winAttrs);

  width = winAttrs.width;
  height = 40;
  x = 0;
  y = winAttrs.height - height;

  createWindow();
  setupCairo();

  startButton.height = height;

  XMapWindow(display, window);
}

void Launcher::createWindow()
{
  window = XCreateSimpleWindow(
    display, rootWindow,
    x, y, width, height,
    0, 0, bgColor.hex
  );

  XSetWindowAttributes attrs;
  attrs.override_redirect = true;
  XChangeWindowAttributes(display, window, CWOverrideRedirect, &attrs);

  long evtMasks = SubstructureRedirectMask
                  | SubstructureNotifyMask
                  | ExposureMask
                  | ButtonPressMask
                  | ButtonReleaseMask
                  | PointerMotionMask;

  XSelectInput(display, window, evtMasks);
}

void Launcher::setupCairo()
{
  int screen = DefaultScreen(display);
  Visual* visual = DefaultVisual(display, screen);
  sfc = cairo_xlib_surface_create(
    display, window, visual, width, height
  );
  cr = cairo_create(sfc);
  tr = new Tracer(cr);
}

void Launcher::draw()
{
  startButton.updateMetrics(cr);

  startButton.draw(tr);
}

void Launcher::handleXEvent(const XEvent evt)
{
  if (evt.xany.window != window) {
    return;
  }
  switch (evt.type) {
    case Expose:
      draw();
      break;
  }
  std::cout << evt.type << '\n';
}
