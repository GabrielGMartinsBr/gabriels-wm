#include "./Central.h"

#include "events/EventHandler.h"


void Central::init()
{
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    throw std::runtime_error("failed to open x display");
  }
  rootWindow = DefaultRootWindow(display);

  cursors = new Cursors(display);

  eventsHandler = new EventHandler();
}
