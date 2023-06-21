#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <functional>

typedef std::function<void()> ButtonCb;


class Button {
 public:
  Window window;

  Button(
    Display *d,
    Window parent,
    int x, int y,
    int w, int h
  );
};