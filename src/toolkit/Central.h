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

  int fullWidth;
  int fullHeight;
  int panelHeight;
  int availWidth;
  int availHeight;

  Cursors *cursors;
  EventHandler *eventsHandler;

  void init();

};