#include "WindowManager.h"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>

#include "FrameWindow.h"
#include "toolkit/Elementor.h"

// TODO: Get win attrs is duplicate on window frame
// TODO: Refactor event switch statement
// TODO:

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

  getPreExistingWindows();

  XEvent evt;

  while (true) {
    XNextEvent(display, &evt);

    switch (evt.type) {
      case CreateNotify:
        std::cout << "CreateNotify:" << evt.xcreatewindow.window << '\n';
        break;
      case ReparentNotify:
        // std::cout << "Received: ReparentNotify" << '\n';
        break;

      case ConfigureRequest: {
        std::cout << "ConfigureRequest:" << evt.xconfigurerequest.window << '\n';
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
        std::cout << "MapRequest:" << evt.xmaprequest.window << '\n';
        XWindowChanges changes;
        changes.x = 300;
        changes.y = 60;
        changes.border_width = 0;

        XConfigureWindow(display, evt.xmaprequest.window, CWX | CWY | CWBorderWidth, &changes);

        addWindowFrame(evt.xmaprequest.window);

        XMapWindow(display, evt.xmaprequest.window);

        break;
      }
      case UnmapNotify: {
        std::cout << "UnmapNotify: " << evt.xunmap.window << '\n';
        unFrame(evt.xunmap);
        break;
      }
      case DestroyNotify: {
        std::cout << "DestroyNotify:" << evt.xdestroywindow.window << '\n';
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

void WindowManager::addWindowFrame(Window window, Bool isPreExisting)
{
  Display *d = Elementor::central.display;

  if (isPreExisting) {
    XWindowAttributes winAttrs;
    XGetWindowAttributes(d, window, &winAttrs);
    if (
      winAttrs.override_redirect
      || winAttrs.map_state != IsViewable
    ) {
      return;
    }

    XSetWindowBorderWidth(d, window, 0);
  }

  FrameWindow *frame = new FrameWindow(d, window);
  frames[window] = frame->frameWindow;
  framesMap[window] = frame;

  std::cout << "FrameWindow: " << frame->frameWindow << " | "
            << "FramedWindow: " << window << '\n';
}

void WindowManager::unFrame(const XUnmapEvent &evt)
{
  if (!frames.count(evt.window)) {
    return;
  }

  if (evt.event == rootWindow) {
    return;
  }

  Window frame = frames[evt.window];

  std::cout << "unFrame window: " << evt.window << '\n';

  XUnmapWindow(evt.display, frame);

  XReparentWindow(evt.display, evt.window, rootWindow, 0, 0);

  XDestroyWindow(evt.display, frame);

  frames.erase(evt.window);
}

void WindowManager::getPreExistingWindows()
{
  Display *d = Elementor::central.display;
  Window root = Elementor::central.rootWindow;
  XGrabServer(d);

  Window returned_root, returned_parent;
  Window *top_level_windows;
  unsigned int num_top_level_windows;
  XQueryTree(
    d,
    root,
    &returned_root,
    &returned_parent,
    &top_level_windows,
    &num_top_level_windows
  );

  for (unsigned int i = 0; i < num_top_level_windows; ++i) {
    addWindowFrame(top_level_windows[i], true);
  }

  XFree(top_level_windows);
  XUngrabServer(d);
}