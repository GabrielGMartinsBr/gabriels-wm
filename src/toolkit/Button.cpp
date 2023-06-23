#include "./Button.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#include <iostream>

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
  Display* display = central->display;
  window = XCreateSimpleWindow(
    display, parent, x, y, w, h, 0,
    borderColor, bgColor
  );

  XSelectInput(display, window, ButtonPressMask | ButtonReleaseMask);
  XMapWindow(display, window);
}

void Button::setBackground(unsigned long color)
{
  XSetWindowAttributes attributes;
  attributes.background_pixel = color;
  XChangeWindowAttributes(central->display, window, CWBackPixel, &attributes);
  XClearWindow(central->display, window);
}

void Button::onClick(const ButtonCallback& cb)
{
  central->addButtonClickCallback(window, cb);
}
