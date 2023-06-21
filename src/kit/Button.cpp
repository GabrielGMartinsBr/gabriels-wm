#include "./Button.h"

Button::Button(
  Display *d,
  Window parent,
  int x, int y,
  int w, int h
)
{
  window = XCreateSimpleWindow(
    d, parent, x, y, w, h, 1,
    BlackPixel(d, DefaultScreen(d)), WhitePixel(d, DefaultScreen(d))
  );
  XSelectInput(d, window, ButtonPressMask | ButtonReleaseMask);
  XMapWindow(d, window);
}