#pragma once

#include <X11/Xlib.h>

class FrameWindow {
 public:
  Window frame;

  FrameWindow(Display* d, Window w);
};