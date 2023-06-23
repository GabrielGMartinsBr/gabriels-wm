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
    int w, int h
  );

  void onClick(const ButtonCallback& cb);

 private:
  Central* central;
};