#include <X11/Xlib.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include "./utils/log.h"

int onWindowManagerDetected(Display *display, XErrorEvent *e) {
  if (e->error_code == BadAccess) {
    // WM events already handled by another program
    throw std::runtime_error("window manager detected");
  }
  return 0;
}

int runWindowManager() {
  Display *display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    // Could not establish a connection with x server
    throw std::runtime_error("failed to open x display");
  }

  Window rootWindow = DefaultRootWindow(display);

  // Try to get window manager events
  long events = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(display, rootWindow, events);
  XSetErrorHandler(&onWindowManagerDetected);
  XSync(display, false);

  Log("Window Manager opened!");

  while (true) {
  }

  XCloseDisplay(display);
  Log("Closing Window Manager...");

  return EXIT_SUCCESS;
}