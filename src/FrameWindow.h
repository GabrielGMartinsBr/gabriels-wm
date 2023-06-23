#pragma once

#include <X11/Xlib.h>

#include <unordered_map>

#include "toolkit/Elementor.h"

class FrameWindow {
 public:
  Window frameWindow;
  Bool maximized;

  FrameWindow(
    Display* d, Window w
  );
};