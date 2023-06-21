#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>
#include <stdexcept>

class WindowManager {
 public:
  void run();

 private:
  void handleMapRequest(const XMapRequestEvent &evt);
};