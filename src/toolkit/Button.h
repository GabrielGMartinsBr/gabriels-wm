#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include "Central.h"

class Button {
 public:
  Window window;

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