#pragma once

#include <X11/Xlib.h>

#include "base/SurfaceContext.hpp"
#include "cairo-xlib.h"
#include "cairo.h"

namespace App {

class Surface {
 public:
  Surface(Display *display, Drawable drawable, uint width, uint height)
  {
    create(display, drawable, width, height);
  }

  SurfaceContext createContext()
  {
    return SurfaceContext(sfc);
  }

  Pointer<SurfaceContext> createContextPtr()
  {
    return std::make_unique<SurfaceContext>(sfc);
  }

  cairo_surface_t *cairoSurface()
  {
    return sfc;
  };

 private:
  cairo_surface_t *sfc;

  void create(Display *dpy, Drawable drawable, uint width, uint height)
  {
    int screen = DefaultScreen(dpy);
    Visual *visual = DefaultVisual(dpy, screen);
    sfc = cairo_xlib_surface_create(
      dpy, drawable, visual, width, height
    );
  }
};

}  // namespace App