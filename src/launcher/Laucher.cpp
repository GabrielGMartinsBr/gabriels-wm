#include <X11/X.h>
#include <X11/Xlib.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "Launcher.h"
#include "cairo-xlib.h"
#include "cairo.h"

Launcher::Launcher(Central* ct) :
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
                  | EnterWindowMask
                  | LeaveWindowMask
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
    case EnterNotify:
      central->setCursor(central->cursors->Pointer);
      break;
    case LeaveNotify:
      central->setCursor(central->cursors->Default);
      break;
    case ButtonPress:
      handleClick();
      break;
    default:
      std::cout << evt.type << '\n';
  }
}

void Launcher::handleClick()
{
  pid_t pid = fork();
  if (pid == 0) {
    system("xterm");
  }
}