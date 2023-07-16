#include "./CompositeTest.h"

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

#include "Logger.hpp"
#include "cairo-xlib.h"
#include "cairo.h"

Compositor::Compositor()
{
  display = XOpenDisplay(nullptr);
  root = DefaultRootWindow(display);
}

Compositor::~Compositor()
{
  XCloseDisplay(display);
}

int Compositor::run()
{
  if (display == nullptr) {
    Log::err() << "failed to open x display";
    return 1;
  }

  long events = SubstructureRedirectMask
                | SubstructureNotifyMask
                | StructureNotifyMask
                | ExposureMask
                | PropertyChangeMask
                | ButtonPressMask;
  XSelectInput(display, root, events);

  w = 600;
  h = 600;

  Window srcWindow = XCreateSimpleWindow(
    display, root, 10, 10, 635, 620, 0, 0, 0x44ffaa
  );
  XSelectInput(display, srcWindow, ButtonPressMask);

  Window dstWindow = XCreateSimpleWindow(
    display, root, 655, 10, 635, 620, 0, 0, 0x44aaff
  );
  XSelectInput(display, dstWindow, ButtonPressMask);

  XSync(display, false);

  XMapWindow(display, srcWindow);
  XMapWindow(display, dstWindow);

  //   ----------------------------------------
  //   Cairo
  int screen = DefaultScreen(display);
  Visual* visual = DefaultVisual(display, screen);

  // Create a drawable surface using Cairo
  surface = cairo_xlib_surface_create(
    display,
    srcWindow,
    visual,
    w, h
  );
  cr = cairo_create(surface);

  XRenderPictFormat* format = XRenderFindVisualFormat(display, visual);

  srcPicture = XRenderCreatePicture(
    display,
    srcWindow,
    format,
    0,
    nullptr
  );
  dstPicture = XRenderCreatePicture(
    display,
    dstWindow,
    format,
    0,
    nullptr
  );

  //   ----------------------------------------

  //   XRenderSetPictureClipRectangles(
  //     display,
  //     srcPicture,
  //     0, 0,
  //     nullptr,
  //     0
  //   );

  render();
  Log::out() << "srcWindow: " << srcWindow;
  Log::out() << "dstWindow: " << dstWindow;

  //   ----------------------------------------

  XEvent evt;
  while (true) {
    XNextEvent(display, &evt);

    switch (evt.type) {
      case ButtonPress: {
        Log::out() << evt.xbutton.window;
        Window clickedWindow = evt.xbutton.window;
        if (clickedWindow == srcWindow) {
          drawOnSrc();
        }
        if (clickedWindow == dstWindow) {
          render();
        }
        break;
      }
    }
  }

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  XRenderFreePicture(display, srcPicture);
  XRenderFreePicture(display, dstPicture);

  return 0;
}

void Compositor::drawOnSrc()
{
  cairo_set_source_rgb(cr, .1, .4, .7);
  cairo_rectangle(cr, 100, 100, 50, 50);
  cairo_fill(cr);
}

void Compositor::render()
{
  XRenderComposite(
    display,
    PictOpSrc,
    srcPicture,
    None,
    dstPicture,
    10, 10,
    0, 0,
    10, 10,
    w, h
  );
}