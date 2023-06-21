#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>
#include <stdexcept>

#include "./utils/codes.h"
#include "./FrameWindow.h"

// Error Handler
int onWindowManagerDetected(Display *display, XErrorEvent *e)
{
  if (e->error_code == BadAccess) {
    // WM events already handled by another program
    throw std::runtime_error("window manager detected");
  }
  return 0;
}

void onMapRequest(const XMapRequestEvent &evt) { }

void handleMap(Display *d, Window w)
{
  FrameWindow f(d, w);
}


// Run
int runWindowManager()
{
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

  std::cout << "Window Manager opened!\n";

  XEvent evt;

  while (true) {
    XNextEvent(display, &evt);

    switch (evt.type) {
      case CreateNotify:
        std::cout << "Received: CreateNotify" << '\n';
        break;
      case ReparentNotify:
        std::cout << "Received: ReparentNotify" << '\n';
        break;
      case DestroyNotify:
        std::cout << "Received: DestroyNotify" << '\n';
        break;

      case ConfigureRequest: {
        std::cout << "Received: ConfigureRequest" << '\n';
        XConfigureRequestEvent e = evt.xconfigurerequest;
        XWindowChanges changes;
        changes.x = e.x;
        changes.y = e.y;
        changes.width = e.width;
        changes.height = e.height;
        changes.border_width = 0;
        changes.sibling = e.above;
        changes.stack_mode = e.detail;
        std::cout << e.value_mask << '\n';
        XConfigureWindow(display, e.window, e.value_mask | CWX | CWY | CWBorderWidth, &changes);
        break;
      }

      case MapRequest: {
        std::cout << "Received: MapRequest" << '\n';
        XWindowChanges changes;
        changes.x = 300;
        changes.y = 60;
        changes.border_width = 0;
        
        XConfigureWindow(display, evt.xmaprequest.window, CWX | CWY | CWBorderWidth, &changes);

        handleMap(evt.xmaprequest.display, evt.xmaprequest.window);

        XMapWindow(display, evt.xmaprequest.window);

        break;
      }

      default:
        std::cout << "Ignored Event: ";
        std::cout << "[" << evt.type << "] ";
        std::cout << getEventName(evt) << '\n';
    }
  }

  XCloseDisplay(display);
  std::cout << "Closing Window Manager...\n";

  return EXIT_SUCCESS;
}