#include "./Central.h"

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
