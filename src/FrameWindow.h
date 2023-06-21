#pragma once

#include <X11/Xlib.h>

class FrameWindow {
 public:
  FrameWindow(Display* d, Window w);
};