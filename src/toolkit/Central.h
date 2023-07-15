#pragma once

#include <X11/Xlib.h>

#include <functional>
#include <stdexcept>
#include <unordered_map>

#include "base/Cursors.h"
#include "events/EventHandler.h"

struct Central {
  Display *display;
  Window rootWindow;

  Cursors *cursors;
  EventHandler *eventsHandler;

  void init();

};