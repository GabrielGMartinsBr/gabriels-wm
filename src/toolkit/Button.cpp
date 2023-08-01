#include "./Button.h"

#include "events/Events.h"

Button::Button(
  Central* c,
  const Window parent,
  int x, int y,
  int w, int h,
  unsigned long bgColor,
  unsigned long borderColor
)
{
  central = c;
  Display* dpy = central->dpy;
  win = XCreateSimpleWindow(
    dpy, parent, x, y, w, h, 0,
    borderColor, bgColor
  );

  XSelectInput(dpy, win, ButtonPressMask | ButtonReleaseMask);
  XMapWindow(dpy, win);

  int diameter = w;
  float r = diameter / 2.0;

  Pixmap mask = XCreatePixmap(dpy, win, diameter, diameter, 1);

  auto screen = DefaultScreenOfDisplay(dpy);
  cairo_surface_t* sfc = cairo_xlib_surface_create_for_bitmap(
    dpy, mask, screen, diameter, diameter
  );
  cairo_t* cr = cairo_create(sfc);

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_arc(cr, r, r, r, 0.0, 2 * M_PI);
  cairo_fill(cr);

  XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, mask, ShapeSet);
  XShapeCombineMask(dpy, win, ShapeInput, 0, 0, mask, ShapeSet);

  XFreePixmap(dpy, mask);
}

void Button::setBackground(unsigned long color)
{
  XSetWindowAttributes attributes;
  attributes.background_pixel = color;
  XChangeWindowAttributes(central->dpy, win, CWBackPixel, &attributes);
  XClearWindow(central->dpy, win);
}

void Button::onClick(const EventCallback& cb)
{
  central->eventsHandler->addEventCb(
    EventType::BUTTON_PRESS, win, cb
  );
}
