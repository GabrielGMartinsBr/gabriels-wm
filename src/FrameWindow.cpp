#include "./FrameWindow.h"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>

FrameWindow::FrameWindow(
  Display* d, Window contentWindow
)
{
  const unsigned int BORDER_WIDTH = 0;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x3f7faf;

  Window rootWindow = DefaultRootWindow(d);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(d, contentWindow, &winAttrs);

  const int headerHeight = 24;
  const int borderThick = 3;
  int topMargin = headerHeight + borderThick;

  int width = winAttrs.width + borderThick * 2;
  int height = winAttrs.height + topMargin;

  frameWindow = XCreateSimpleWindow(
    d, rootWindow,
    winAttrs.x, winAttrs.y,
    width, height,
    BORDER_WIDTH, BORDER_COLOR, BG_COLOR
  );

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(d, frameWindow, evtMasks);

  XReparentWindow(d, contentWindow, frameWindow, borderThick, headerHeight);

  XMapWindow(d, frameWindow);

  Button closeButton = Elementor::button(
    frameWindow, width - 15, 6, 10, 10,
    0xff0000
  );
  closeButton.onClick([=]() {
    XKillClient(d, contentWindow);
  });


  Button maximizeBtn = Elementor::button(
    frameWindow, width - 30, 6, 10, 10,
    0x00ff00
  );
  Window frameWindowId = frameWindow;
  maximizeBtn.onClick([=]() {
    Elementor::central.maximizeWindow(frameWindowId, contentWindow);
  });

  Button minimizeBtn = Elementor::button(
    frameWindow, width - 45, 6, 10, 10,
    0xffff00
  );
}
