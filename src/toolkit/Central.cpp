#include "./Central.h"

#include "events/EventHandler.h"

void Central::init()
{
  dpy = XOpenDisplay(nullptr);
  if (dpy == nullptr) {
    throw std::runtime_error("failed to open x display");
  }

  rootWindow = DefaultRootWindow(dpy);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(dpy, rootWindow, &winAttrs);
  fullWidth = winAttrs.width;
  fullHeight = winAttrs.height;

  panelHeight = 40;
  availWidth = fullWidth;
  availHeight = fullHeight - panelHeight;

  cursors = new Cursors(dpy);

  eventsHandler = new EventHandler();
}
