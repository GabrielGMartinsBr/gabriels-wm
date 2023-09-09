#include <sys/types.h>

#include "base/Log.hpp"
#include "cairo-xlib.h"
#include "cairo.h"

class DesktopBase {
 protected:
  int width;
  int height;
  bool flagInit = false;
  bool flagCairo = false;

  Display *dpy;
  Window rootWin;
  Window win;

  cairo_surface_t *sfc;
  cairo_t *cr;

 protected:
  void
  setupCairo()
  {
    Log::out() << "setupCairo";
    int screen = DefaultScreen(dpy);
    Visual *visual = DefaultVisual(dpy, screen);
    sfc = cairo_xlib_surface_create(
      dpy, win, visual, width, height
    );
    cr = cairo_create(sfc);
    flagCairo = true;
  }
};