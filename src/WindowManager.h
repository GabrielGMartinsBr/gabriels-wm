#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include "FrameWindow.h"

class WindowManager {
 public:
  void run();

 private:
  std::unordered_map<Window, Window> frames;
  std::unordered_map<Window, FrameWindow*> framesMap;
  Window rootWindow;

  void addWindowFrame(Window window, Bool isPreExisting = false);

  void unFrame(const XUnmapEvent &evt);

  void getPreExistingWindows();
};