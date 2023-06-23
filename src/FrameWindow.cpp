#include "./FrameWindow.h"

#include <X11/X.h>
#include <X11/Xlib.h>

FrameWindow::FrameWindow(
  Display* d, Window w
)
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

  window = XCreateSimpleWindow(
    d, rootWindow,
    300, 100,
    width, height,
    BORDER_WIDTH, BORDER_COLOR, BG_COLOR
  );

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(d, window, evtMasks);

  XReparentWindow(d, w, window, borderThick, headerHeight);

  XMapWindow(d, window);

  Button closeButton = Elementor::button(
    window, width - 15, 6, 10, 10,
    0xff0000
  );
  closeButton.onClick([=]() {
    XKillClient(d, w);
  });

  Button maximizeBtn = Elementor::button(
    window, width - 30, 6, 10, 10,
    0x00ff00
  );
  Button minimizeBtn = Elementor::button(
    window, width - 45, 6, 10, 10,
    0xffff00
  );

}
