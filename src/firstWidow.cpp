#include "./log.h"
#include <X11/Xlib.h>
#include <unistd.h>

int createFirstWindow() {
  Display *display = XOpenDisplay(0);

  if (display == NULL) {
    Log("Failed to open display");
    return 1;
  }

  int screen = DefaultScreen(display);
  Window rootWindow = RootWindow(display, screen);

  const static unsigned short width = 320;
  const static unsigned short height = 240;

  Window window =
      XCreateWindow(display, rootWindow, 0, 0, width, height, 0, CopyFromParent,
                    InputOutput, CopyFromParent, 0, 0);

  XStoreName(display, window, "First Window");

  XMapWindow(display, window);
  XFlush(display);

  sleep(5);

  XDestroyWindow(display, window);
  XCloseDisplay(display);

  Log("First window works!");

  return 0;
}