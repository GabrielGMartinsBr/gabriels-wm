#pragma once

#include <X11/Xlib.h>

#include <functional>
#include <stdexcept>
#include <unordered_map>

typedef std::function<void()> ButtonCallback;

struct Central {
  Display *display;
  Window rootWindow;

  void init();

  void addButtonClickCallback(const Window windowId, ButtonCallback cb);

  void handleButtonClickEvent(XButtonEvent event);

 private:
  std::unordered_map<Window, ButtonCallback> buttonCallbacks;
};