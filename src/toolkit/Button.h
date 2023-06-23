#pragma once

#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <cairo/cairo-xlib.h>

#include <cmath>

#include "Central.h"

class Button {
 public:
  Window win;

  Button(
    Central* central,
    const Window parent,
    int x, int y,
    int w, int h,
    unsigned long bgColor = 0xffffff,
    unsigned long borderColor = 0x000000
  );

  void onClick(const ButtonCallback& cb);

  void setBackground(unsigned long color);

 private:
  Central* central;
};