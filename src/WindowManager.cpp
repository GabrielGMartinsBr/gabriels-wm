#include "./WindowManager.h"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>

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

void WindowManager::run()
{
  Display *display = Elementor::central.display;
  rootWindow = Elementor::central.rootWindow;

  // Try to get window manager events
  long events = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(display, rootWindow, events);
  XSetErrorHandler(&onWindowManagerDetected);
  XSync(display, false);

  std::cout << "Window Manager has been init!\n";

  XEvent evt;

  while (true) {
    XNextEvent(display, &evt);

    switch (evt.type) {
      case CreateNotify:
        // std::cout << "Received: CreateNotify" << '\n';
        break;
      case ReparentNotify:
        // std::cout << "Received: ReparentNotify" << '\n';
        break;

      case ConfigureRequest: {
        // std::cout << "Received: ConfigureRequest" << '\n';
        XConfigureRequestEvent e = evt.xconfigurerequest;
        XWindowChanges changes;
        changes.x = e.x;
        changes.y = e.y;
        changes.width = e.width;
        changes.height = e.height;
        changes.border_width = 0;
        changes.sibling = e.above;
        changes.stack_mode = e.detail;
        XConfigureWindow(display, e.window, e.value_mask | CWX | CWY | CWBorderWidth, &changes);
        break;
      }
      case MapRequest: {
        // std::cout << "Received: MapRequest" << '\n';
        XWindowChanges changes;
        changes.x = 300;
        changes.y = 60;
        changes.border_width = 0;

        XConfigureWindow(display, evt.xmaprequest.window, CWX | CWY | CWBorderWidth, &changes);

        handleMapRequest(evt.xmaprequest);

        XMapWindow(display, evt.xmaprequest.window);

        break;
      }
      case UnmapNotify: {
        unFrame(evt.xunmap);
        break;
      }
      case DestroyNotify: {
        // std::cout << "Received: DestroyNotify" << '\n';
        break;
      }
      case ButtonPress: {
        Elementor::central.handleButtonClickEvent(
          evt.xbutton
        );
        break;
      }

        // default:
        // std::cout << "Ignored Event: ";
        // std::cout << "[" << evt.type << "] ";
        // std::cout << getEventName(evt) << '\n';
    }
  }

  XCloseDisplay(display);
  std::cout << "Closing Window Manager...\n";
}

void WindowManager::handleMapRequest(const XMapRequestEvent &evt)
{
  FrameWindow frame(evt.display, evt.window);
  std::cout << "FrameWindow: " << frame.window << '\n';
  frames[evt.window] = frame.window;
}

void WindowManager::unFrame(const XUnmapEvent &evt)
{
  std::cout << "unFrame window: " << evt.window << '\n';
  if (!frames.count(evt.window)) {
    return;
  }
  Window frame = frames[evt.window];

  XUnmapWindow(evt.display, frame);

  XReparentWindow(evt.display, evt.window, rootWindow, 0, 0);

  XDestroyWindow(evt.display, frame);

  frames.erase(evt.window);
}