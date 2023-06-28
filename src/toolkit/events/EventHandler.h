#pragma once

#include <X11/Xlib.h>

#include <functional>
#include <unordered_map>

#include "Events.h"

typedef std::function<void(Event)> EventCallback;
typedef std::function<void(ButtonEvent)> ButtonEventCallback;

typedef std::unordered_map<u_int, EventCallback> EventMap;
typedef std::unordered_map<u_int, ButtonEventCallback> ButtonEventMap;

struct EventHandler {
  EventMap motionMap;
  ButtonEventMap pressMap;
  ButtonEventMap releaseMap;
  std::unordered_map<EventType, EventMap> handlersMap;

  void handleButtonPress(const XButtonEvent event);
  void handleButtonRelease(const XButtonEvent event);
  void handleMotionNotify(const XMotionEvent event);

  void addEventCb(EventType type, Window id, const EventCallback& cb);
};