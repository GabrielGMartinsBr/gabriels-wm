#include "WindowManager.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include <iostream>

#include "FrameWindow.h"
#include "toolkit/Elementor.h"
#include "utils/codes.h"

// TODO: Get win attrs is duplicate on window frame

// Error Handler
int onWindowManagerDetected(Display *display, XErrorEvent *e)
{
  if (e->error_code == BadAccess) {
    // WM events already handled by another program
    throw std::runtime_error("window manager detected");
  }
  return 0;
}

void WindowManager::run(const Central *ct)
{
  central = ct;
  display = central->display;
  rootWindow = central->rootWindow;

  Cursor cursor = XCreateFontCursor(display, XC_arrow);
  XDefineCursor(display, rootWindow, cursor);

  // Try to get window manager events
  long events = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(display, rootWindow, events);
  XSetErrorHandler(&onWindowManagerDetected);
  XSync(display, false);

  std::cout << "Window Manager has been init!\n";

  getPreExistingWindows();

  XEvent evt;

  auto testWin = Elementor::testWindow();
  addWindowFrame(testWin);

  while (true) {
    XNextEvent(display, &evt);
    switch (evt.type) {
      case CreateNotify:
        handleCreateNotify(evt.xcreatewindow);
        break;
      case ConfigureRequest:
        handleConfigureRequest(evt.xconfigurerequest);
        break;
      case MapRequest:
        handleMapRequest(evt.xmaprequest);
        break;
      case MapNotify:
        handleMapNotify(evt.xmap);
        break;
      case UnmapNotify:
        handleUnmapNotify(evt.xunmap);
        break;
      case ReparentNotify:
        handleReparentNotify(evt.xreparent);
        break;
      case ButtonPress:
        handleButtonPress(evt.xbutton);
        break;
      case DestroyNotify:
        handleDestroyNotify(evt.xdestroywindow);
        break;
      default: {
        handleIgnoredEvent(evt);
      }
    }
  }

  XCloseDisplay(display);
  std::cout << "Closing Window Manager...\n";
}

void WindowManager::handleCreateNotify(const XCreateWindowEvent event)
{
  std::cout << "XCreateWindowEvent:" << event.window << '\n';
}

void WindowManager::handleConfigureRequest(const XConfigureRequestEvent event)
{
  std::cout << "XConfigureRequestEvent:" << event.window << '\n';
  XWindowChanges changes;
  changes.x = event.x;
  changes.y = event.y;
  changes.width = event.width;
  changes.height = event.height;
  changes.border_width = 0;
  changes.sibling = event.above;
  changes.stack_mode = event.detail;
  XConfigureWindow(display, event.window, event.value_mask | CWX | CWY | CWBorderWidth, &changes);
}

void WindowManager::handleMapRequest(const XMapRequestEvent event)
{
  std::cout << "XMapRequestEvent:" << event.window << '\n';
  XWindowChanges changes;
  changes.x = 300;
  changes.y = 60;
  changes.border_width = 0;

  XConfigureWindow(display, event.window, CWX | CWY | CWBorderWidth, &changes);
  addWindowFrame(event.window);
  XMapWindow(display, event.window);
}

void WindowManager::handleMapNotify(const XMapEvent event)
{
  std::cout << "XMapEvent: " << event.window << '\n';
}

void WindowManager::handleUnmapNotify(const XUnmapEvent event)
{
  std::cout << "XUnmapEvent: " << event.window << '\n';
  unFrame(event);
}

void WindowManager::handleReparentNotify(const XReparentEvent event)
{
  std::cout << "XReparentEvent: " << event.window << '\n';
}

void WindowManager::handleButtonPress(const XButtonEvent event)
{
  std::cout << "XButtonEvent:" << event.window << '\n';
  Elementor::central.handleButtonClickEvent(
    event
  );
}

void WindowManager::handleDestroyNotify(const XDestroyWindowEvent event)
{
  std::cout << "XButtonEvent:" << event.window << '\n';
}

void WindowManager::handleIgnoredEvent(const XEvent &event)
{
  // if (event.type)
  std::cout << "IgnoredEvent"
            << "[" << event.type << "]: ";
  std::cout << getEventName(event) << '\n';
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

  FrameWindow *frame = new FrameWindow(&Elementor::central, window);
  frames[window] = frame->frameWindow;
  framesMap[window] = frame;

  // std::cout << "FrameWindow: " << frame->frameWindow << " | "
  //           << "FramedWindow: " << window << '\n';
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