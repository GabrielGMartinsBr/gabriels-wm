#include "./CompositeTest.h"

#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/composite.h>

#include "base/Log.hpp"
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

  srcWindow = XCreateSimpleWindow(
    display, root, 10, 10, 635, 620, 0, 0, 0x44ffaa
  );
  XSelectInput(display, srcWindow, ButtonPressMask);

  dstWindow = XCreateSimpleWindow(
    display, root, 655, 10, 635, 620, 0, 0, 0x44aaff
  );
  XSelectInput(display, dstWindow, ButtonPressMask);

  XSync(display, false);

  XMapWindow(display, srcWindow);
  XMapWindow(display, dstWindow);

  XCompositeRedirectSubwindows(
    display,
    srcWindow,
    CompositeRedirectManual
  );

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

  //   render();
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
  if (drawn) {
    // XRaiseWindow(display, srcWindow);
    // XRaiseWindow(display, child);
    return;
  }
  cairo_set_source_rgb(cr, .1, .4, .7);
  cairo_rectangle(cr, 100, 100, 50, 50);
  cairo_fill(cr);

  child = XCreateSimpleWindow(
    display, srcWindow, 200, 20, 200, 200, 0, 0, 0xffaa44
  );
  XSelectInput(display, child, ButtonPressMask);
  XMapWindow(display, child);
  Log::out() << "child: " << child;
  drawn = true;
}

void Compositor::render()
{
  int screen = DefaultScreen(display);
  Visual* visual = DefaultVisual(display, screen);
  XRenderPictFormat* format = XRenderFindVisualFormat(display, visual);
  srcPicture = XRenderCreatePicture(
    display,
    child,
    format,
    0,
    nullptr
  );
  //   XRectangle rect;
  //   rect.x = 0;
  //   rect.y = 0;
  //   rect.width = 100;
  //   rect.height = 100;
  //   XRectangle rectangles[] = { rect };
  //   XRenderSetPictureClipRectangles(
  //     display,
  //     srcPicture,
  //     0, 0,
  //     rectangles,
  //     1
  //   );
  XRenderComposite(
    display,
    PictOpAtop,
    srcPicture,
    None,
    dstPicture,
    10, 10,
    0, 0,
    10, 10,
    w, h
  );
}