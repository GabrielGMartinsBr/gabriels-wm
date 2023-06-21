#include "./FrameWindow.h"

#include <X11/X.h>
#include <X11/Xlib.h>

FrameWindow::FrameWindow(Display* d, Window w)
{
  const unsigned int BORDER_WIDTH = 3;
  const unsigned long BORDER_COLOR = 0xff0000;
  const unsigned long BG_COLOR = 0x0000ff;

  Window rootWindow = DefaultRootWindow(d);

  XWindowAttributes winAttrs;
  XGetWindowAttributes(d, w, &winAttrs);

  const Window frame = XCreateSimpleWindow(
    d, rootWindow,
    300, 100,
    winAttrs.width, winAttrs.height,
    BORDER_WIDTH, BORDER_COLOR, BG_COLOR
  );

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(d, frame, evtMasks);

  XReparentWindow(d, w, frame, 0, 0);

  XMapWindow(d, frame);
}