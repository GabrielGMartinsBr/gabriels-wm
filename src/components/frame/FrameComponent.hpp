#pragma once

#include <X11/Xlib.h>

#include <memory>

#include "base/BasicWindow.hpp"
#include "base/Color.hpp"
#include "base/Surface.hpp"
#include "base/SurfaceContext.hpp"
#include "base/consts.h"

namespace App {

class FrameComponent {
 public:
  FrameComponent(Display* dpy, Window parent) :
      window(dpy, parent)
  {
    this->display = dpy;
    this->parent = parent;
    createWindow();
    createSurface();
  }

  void show()
  {
    window.show();
    drawContent();
  }

 private:
  Display* display;
  Window parent;
  Pointer<Surface> sfc;
  Pointer<SurfaceContext> sfx;

  BasicWindow window;

  int x = 230;
  int y = 260;
  uint width = 640;
  uint height = 480;
  ulong bg = 0x333336;

  void createWindow()
  {
    window.rect(x, y, width, height)
      .backgroundColor(bg)
      .create();
  }

  void createSurface()
  {
    sfc = std::make_unique<Surface>(display, window.xWindow(), width, height);
    sfx = sfc->createContextPtr();
  }

  void drawContent()
  {
    sfx->setSourceColor(0x333334)
      .paint()
      .setSourceColor("#fff")
      .rectangle(16, 16, width - 32, height - 32)
      .fill()
      .setSourceColor("#fa3")
      .arc(100, 100, 10, 0, PI2)
      .fill();
  }
};

}  // namespace App