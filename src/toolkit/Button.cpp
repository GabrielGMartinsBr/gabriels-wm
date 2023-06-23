#include "./Button.h"

#include <X11/Xlib.h>

#include <iostream>

Button::Button(
  Central* c,
  const Window parent,
  int x, int y,
  int w, int h
)
{
  central = c;
  Display* display = central->display;
  window = XCreateSimpleWindow(
    display, parent, x, y, w, h, 1,
    0x000000, 0xffffff
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
