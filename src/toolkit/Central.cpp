#include "./Central.h"

#include "events/EventHandler.h"

void Central::init()
{
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    throw std::runtime_error("failed to open x display");
  }

  rootWindow = DefaultRootWindow(display);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(display, rootWindow, &winAttrs);
  fullWidth = winAttrs.width;
  fullHeight = winAttrs.height;

  panelHeight = 40;
  availWidth = fullWidth;
  availHeight = fullHeight - panelHeight;

  cursors = new Cursors(display);

  eventsHandler = new EventHandler();
}
