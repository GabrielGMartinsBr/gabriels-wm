#include <X11/X.h>
#include <X11/Xlib.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "Launcher.h"
#include "cairo-xlib.h"
#include "cairo.h"

Launcher::Launcher(Central* ct) :
    bgColor("#333")
{
  central = ct;
  display = ct->dpy;
  rootWindow = ct->rootWindow;

  width = central->availWidth;
  height = central->panelHeight;
  x = 0;
  y = central->fullHeight - height;

  createWindow();
  setupCairo();
  createEntries();
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
                  | PointerMotionMask
                  | VisibilityChangeMask;

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

void Launcher::createEntries()
{
  entries.push_back(
    LauncherEntry("Xterm", "xterm")
  );
  entries.push_back(
    LauncherEntry("Thunar", "thunar")
  );
  int x = 0;
  for (auto& e : entries) {
    e.x = x;
    e.y = 0;
    e.height = height;
    e.updateMetrics(cr);
    x += e.width + 3;
  }
}

void Launcher::onExpose()
{
  drawEntries();
}

void Launcher::drawEntries()
{
  for (auto& e : entries) {
    e.draw(tr);
  }
}

void Launcher::handleMouseMove(const XMotionEvent evt)
{
  bool isHover = false;
  for (auto& entry : entries) {
    if (entry.isHover(evt.x, evt.y)) {
      isHover = true;
      break;
    }
  }
  if (isHover) {
    central->cursors->set(window, CursorKey::POINTER);
  } else {
    central->cursors->set(window, CursorKey::DEFAULT);
  }
}

void Launcher::handleClick(const XButtonEvent evt)
{
  for (auto& entry : entries) {
    if (entry.isHover(evt.x, evt.y)) {
      launchProgram(entry.command.c_str());
      break;
    }
  }
}

void Launcher::launchProgram(const char* command)
{
  pid_t pid = fork();
  if (pid == 0) {
    system(command);
    // execl("/path/to/your/program", "/path/to/your/program", NULL);
    exit(0);
  }
}

void Launcher::handleXEvent(const XEvent evt)
{
  if (evt.type == VisibilityNotify) {
    XVisibilityEvent e = evt.xvisibility;
    if (e.window == window && e.state) {
      XRaiseWindow(display, window);
    }
    return;
  }
  if (evt.xany.window != window) {
    return;
  }
  switch (evt.type) {
    case Expose:
      onExpose();
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