#include <X11/Xlib.h>
#include <sys/types.h>

#include <array>

#include "../toolkit/Central.h"
#include "cairo-xlib.h"
#include "cairo.h"

class Desktop {
  bool initialized = false;
  Central *ct;
  Display *dpy;
  Window rootWin;
  Window win;

  int width = 0;
  int height = 0;

 public:
  void init(Central *central)
  {
    if (initialized) {
      return;
    }
    ct = central;
    dpy = central->dpy;
    rootWin = central->rootWindow;
    initialized = true;
    createWindow();
    mapWindow();
  }

 private:
  struct WinVec {
    int x;
    int y;
    int w;
    int h;
  };

  void createWindow()
  {
    int x = 0;
    int y = 0;
    width = ct->availWidth;
    height = ct->availHeight;
    u_int bg = 0x030309;
    win = XCreateSimpleWindow(dpy, rootWin, x, y, width, height, 0, 0, bg);
  }

  void mapWindow()
  {
    XMapWindow(dpy, win);
  }

  void setupCairo()
  {
    int screen = DefaultScreen(dpy);
    Visual *visual = DefaultVisual(dpy, screen);
    cairo_surface_t *sfc = cairo_xlib_surface_create(
      dpy, win, visual, width, height
    );
    cairo_t *cr = cairo_create(sfc);
  }
};