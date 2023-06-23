#include "./FrameWindow.h"
#include <X11/Xlib.h>

#include "toolkit/Elementor.h"

FrameWindow::FrameWindow(
  Display* d, Window cWindow
) :
    maximized(false)
{
  const unsigned int BORDER_WIDTH = 0;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x3f7faf;

  Window rootWindow = DefaultRootWindow(d);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(d, cWindow, &winAttrs);

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

  contentWindow = cWindow;

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(d, frameWindow, evtMasks);

  XReparentWindow(d, cWindow, frameWindow, borderThick, headerHeight);

  XMapWindow(d, frameWindow);

  closeButton = Elementor::button(
    frameWindow, width - 15, 6, 12, 12,
    0xff0000
  );
  closeButton->onClick([=]() {
    XKillClient(d, cWindow);
  });

  maximizeButton = Elementor::button(
    frameWindow, width - 30, 6, 12, 12,
    0x00ff00
  );
  maximizeButton->onClick([this]() {
    maximize();
  });
  minimizeButton = Elementor::button(
    frameWindow, width - 45, 6, 12, 12,
    0xffff00
  );
}

void FrameWindow::maximize()
{
  Elementor::central.maximizeWindow(frameWindow, contentWindow);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(Elementor::central.display, Elementor::central.rootWindow, &winAttrs);
  int width = winAttrs.width;
  XMoveWindow(Elementor::central.display, closeButton->win, width - 15, 6 );
  XMoveWindow(Elementor::central.display, maximizeButton->win, width - 30, 6 );
  XMoveWindow(Elementor::central.display, minimizeButton->win, width - 45, 6 );

  maximized = true;
}