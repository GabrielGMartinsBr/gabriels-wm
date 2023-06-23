#include "./Central.h"

#include <X11/Xlib.h>

#include <iostream>

void Central::init()
{
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    throw std::runtime_error("failed to open x display");
  }
  rootWindow = DefaultRootWindow(display);
}

void Central::addButtonClickCallback(const Window windowId, const ButtonCallback cb)
{
  buttonCallbacks[windowId] = cb;
}

void Central::handleButtonClickEvent(XButtonEvent event)
{
  const auto windowId = event.window;
  auto hasCallback = buttonCallbacks.count(windowId);
  if (hasCallback) {
    buttonCallbacks[windowId]();
  }
}

void Central::maximizeWindow(Window frameWindow, Window contentWindow)
{
  const int headerHeight = 24;
  const int borderThick = 3;
  int topMargin = headerHeight + borderThick;

  XWindowAttributes winAttrs;
  XGetWindowAttributes(display, rootWindow, &winAttrs);

  int width = winAttrs.width;
  int height = winAttrs.height;
  int contentWidth = width - borderThick * 2;
  int contentHeight = height - topMargin;

  XMoveResizeWindow(display, frameWindow, 0, 0, width, height);
  XResizeWindow(display, contentWindow, contentWidth, contentHeight);
}
