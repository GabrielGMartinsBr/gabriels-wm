#include "./Button.h"

#include <iostream>

Button::Button(
  Central* c,
  const Window parent,
  int x, int y,
  int w, int h
)
{
  central = c;
  Display* d = central->display;
  window = XCreateSimpleWindow(
    d, parent, x, y, w, h, 1,
    BlackPixel(d, DefaultScreen(d)), WhitePixel(d, DefaultScreen(d))
  );
  XSelectInput(d, window, ButtonPressMask | ButtonReleaseMask);
  XMapWindow(d, window);
}

void Button::onClick(const ButtonCallback& cb)
{
  central->addButtonClickCallback(window, cb);
}
