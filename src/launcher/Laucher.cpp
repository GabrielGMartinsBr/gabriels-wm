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
    startButton("Xterm")
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

  startButton.x = 0;
  startButton.y = 0;
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

void Launcher::handleMouseMove(const XMotionEvent e)
{
  if (startButton.isHover(e.x, e.y)) {
    central->cursors->set(window, CursorKey::POINTER);
  } else {
    central->cursors->set(window, CursorKey::DEFAULT);
  }
}

void Launcher::handleClick(const XButtonEvent e)
{
  if (startButton.isHover(e.x, e.y)) {
    launchProgram("xterm");
  }
}

void Launcher::launchProgram(const std::string& command)
{
  pid_t pid = fork();
  if (pid == 0) {
    system("xterm");
  }
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
      break;
    case LeaveNotify:
      break;
    case MotionNotify:
      handleMouseMove(evt.xmotion);
      break;
    case ButtonPress:
      handleClick(evt.xbutton);
      break;
    default:
      std::cout << evt.type << '\n';
  }
}