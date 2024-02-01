#pragma once

#include <X11/Xlib.h>

#include <memory>

#include "base/BasicWindow.hpp"

namespace App {

class FrameComponent {
 public:
  FrameComponent(Display* dpy, Window parent) :
      window(dpy, parent)
  {
    create();
  }

  void show() { }

 private:
  BasicWindow window;

  void create()
  {
    window.rect(230, 260, 640, 480)
      .backgroundColor(0xff7744)
      .create()
      .show();
  }
};

}  // namespace App