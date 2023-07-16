#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct WindowUtils {
  static char *getNameProp(Display *d, const Window w);

  static void setAlwaysOnTop(Display *d, const Window w);
};