#pragma once

#include <X11/Xlib.h>

#include <functional>
#include <stdexcept>
#include <unordered_map>

#include "events/EventHandler.h"

struct Central {
  Display *display;
  Window rootWindow;

  EventHandler *eventsHandler;

  void init();
};