#pragma once

#include <X11/Xlib.h>

#include <unordered_map>

#include "toolkit/Button.h"

class FrameWindow {
 public:
  Window frameWindow;
  Window contentWindow;
  Bool maximized;

  FrameWindow(
    Display *d, Window w
  );

  void maximize();

 private:
  Button *closeButton;
  Button *maximizeButton;
  Button *minimizeButton;
};