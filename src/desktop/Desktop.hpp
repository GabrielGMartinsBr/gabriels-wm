#include <X11/Xlib.h>
#include <sys/types.h>

#include <cmath>

#include "DesktopBase.hpp"
#include "base/Log.hpp"
#include "cairo.h"
#include "toolkit/Central.h"

class Desktop : public DesktopBase {
  Central *ct;
  cairo_surface_t *img_sfc;

  bool flagBgImage = false;

 public:
  void init(Central *central)
  {
    if (flagInit) {
      return;
    }
    ct = central;
    dpy = central->dpy;
    rootWin = central->rootWindow;
    flagInit = true;
    createWindow();
    mapWindow();
  }

  void handleXEvent(const XEvent &evt)
  {
    if (evt.xany.window == win) {
      Log::out() << evt.type;
    }
    if (!flagInit || evt.xany.window != win) {
      return;
    }
    if (evt.type == Expose) {
      handleExpose(evt.xexpose);
    }
  }

 private:
  void createWindow()
  {
    int x = 0;
    int y = 0;
    width = ct->availWidth;
    height = ct->availHeight;
    u_int bg = 0x131319;
    win = XCreateSimpleWindow(dpy, rootWin, x, y, width, height, 0, 0, bg);
    long inputs = ExposureMask;
    XSelectInput(dpy, win, inputs);
  }

  void mapWindow()
  {
    XMapWindow(dpy, win);
  }

  void handleExpose(const XExposeEvent &evt)
  {
    Log::out() << "expose";
    if (!flagCairo) {
      setupCairo();
    }
    if (!flagBgImage) {
      renderBgImage();
    } else {
      // refreshImage();
    }
  }

  void renderBgImage()
  {
    Log::out() << "renderBgImage";
    flagBgImage = true;

    const char *filename = "./assets/wallpaper-001.png";
    cairo_surface_t *img_sfc = cairo_image_surface_create_from_png(filename);

    cairo_status_t status = cairo_surface_status(img_sfc);
    if (status != CAIRO_STATUS_SUCCESS) {
      Log::err() << "Failed to load the image.";
      Log::err() << "status: " << status;
      return;
    }

    double scrWidth = (double)width;
    double scrHeight = (double)height;
    double scrAspect = scrWidth / scrHeight;

    double imgWidth = cairo_image_surface_get_width(img_sfc);
    double imgHeight = cairo_image_surface_get_height(img_sfc);
    double imgAspect = imgWidth / imgHeight;

    double sx, sy;
    if (scrAspect > imgAspect) {
      sx = sy = (double)scrWidth / imgWidth;
    } else {
      sx = sy = (double)scrHeight / imgHeight;
    }
    double scale = sx;

    double tx = (scrWidth - scale * imgWidth) / 2.0;
    // double ty = (scrHeight - scale * imgHeight) / 2.0;
    double ty = (scrHeight - scale * imgHeight);

    cairo_scale(cr, scale, scale);
    cairo_translate(cr, tx, ty);

    cairo_set_source_surface(cr, img_sfc, 0, 0);
    cairo_paint(cr);
  }

  void refreshImage()
  {
    Log::out() << "Refresh";
    cairo_paint(cr);
  }
};