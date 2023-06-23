#pragma once

#include <X11/Xlib.h>

#include <unordered_map>

#include "toolkit/Elementor.h"

class FrameWindow {
 public:
  Window window;

  FrameWindow(
    Display* d, Window w
  );

 private:
  void close();
};