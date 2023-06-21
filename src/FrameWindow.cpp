#include "./FrameWindow.h"

#include <X11/X.h>
#include <X11/Xlib.h>

FrameWindow::FrameWindow(Display* d, Window w)
{
  const unsigned int BORDER_WIDTH = 0;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x3f7faf;

  Window rootWindow = DefaultRootWindow(d);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(d, w, &winAttrs);

  const int headerHeight = 24;
  const int borderThick = 3;
  int topMargin = headerHeight + borderThick;

  int width = winAttrs.width + borderThick * 2;
  int height = winAttrs.height + topMargin;

  const Window frame = XCreateSimpleWindow(
    d, rootWindow,
    300, 100,
    width, height,
    BORDER_WIDTH, BORDER_COLOR, BG_COLOR
  );

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(d, frame, evtMasks);

  XReparentWindow(d, w, frame, borderThick, headerHeight);

  XMapWindow(d, frame);
}