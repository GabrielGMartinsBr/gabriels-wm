#pragma once

#include <X11/Xlib.h>

#include <unordered_map>

#include "kit/Button.h"

class FrameWindow {
 public:
  Window window;

  FrameWindow(
    Display* d, Window w,
    std::unordered_map<Window, ButtonCb>* buttonsCb
  );

 private:
  void close();
};