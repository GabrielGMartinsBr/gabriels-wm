#pragma once

#include <X11/Xlib.h>

#include <memory>

#include "base/BasicWindow.hpp"
#include "base/Color.hpp"
#include "base/Surface.hpp"
#include "base/SurfaceContext.hpp"
#include "base/consts.h"
#include "cairo.h"

namespace App {

class FrameComponent {
 public:
  FrameComponent(Display* dpy, Window parent) :
      window(dpy, parent)
  {
    this->display = dpy;
    this->parent = parent;
    createWindow();
    setupXInputs();
    createSurface();
  }

  void show()
  {
    window.show();
    drawContent();
  }

  void anime()
  {
    doAnime();
  }

  void handleXEvent(const XEvent& evt)
  {
    if (evt.xany.window != window.xWindow()) {
      return;
    }

    if (evt.type == MotionNotify) {
      xm = evt.xmotion.x;
      ym = evt.xmotion.y;
    }
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

  int i = 0;
  int xm = 0;
  int ym = 0;

  void createWindow()
  {
    window.rect(x, y, width, height)
      .backgroundColor(bg)
      .create();
  }

  void setupXInputs()
  {
    long evtMasks = SubstructureRedirectMask
                    | SubstructureNotifyMask
                    | ExposureMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | EnterWindowMask
                    | LeaveWindowMask
                    | PointerMotionMask
                    | VisibilityChangeMask;

    XSelectInput(display, window.xWindow(), evtMasks);
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
      .fill()
      .setSourceColor("#37a")
      .rectangle(200, 200, 200, 200)
      .lineWidth(9.0)
      .stroke()
      .setSourceColor("#333")
      .lineWidth(6.0)
      .setLineCap(CAIRO_LINE_CAP_BUTT)
      .moveTo(100, 100)
      .lineTo(200, 100)
      .stroke()
      .lineWidth(12.0)
      .setLineCap(CAIRO_LINE_CAP_SQUARE)
      .setLineJoin(CAIRO_LINE_JOIN_BEVEL)
      .moveTo(100, 110)
      .lineTo(200, 110)
      .lineTo(200, 200)
      .stroke()
      .moveTo(200, 200)
      .setLineJoin(CAIRO_LINE_JOIN_MITER)
      .lineTo(100, 200)
      .stroke()
      .moveTo(100, 200)
      .setLineJoin(CAIRO_LINE_JOIN_ROUND)
      .lineTo(100, 110)
      .stroke();
  }

  void doAnime()
  {
    sfx->setSourceColor(0x333334).paint();
    i = (i + 3) % 512;

    int j = i;
    if (i > 255) {
      j = 512 - i;
    }

    sfx->setSourceColor("#fc7")
      .circle(10 + j, 100, 9)
      .fill();

    sfx->setSourceColor(j % 255, ((3 * i) / 2) % 255, (2 * i) % 255)
      .circle(96, 200, 32)
      .fill();

    sfx->setSourceColor("#444")
      .circle(xm, ym, 32)
      .fill();
  }
};

}  // namespace App