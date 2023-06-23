#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>
#include <stdexcept>
#include <unordered_map>

class WindowManager {
 public:
  void run();

 private:
  std::unordered_map<Window, Window> frames;
  Window rootWindow;

  void handleMapRequest(const XMapRequestEvent &evt);

  void unFrame(const XUnmapEvent &evt);
};