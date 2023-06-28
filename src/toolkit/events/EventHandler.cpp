#include "EventHandler.h"

#include <X11/X.h>

#include "Events.h"

void EventHandler::handleButtonPress(const XButtonEvent event)
{
  if (pressMap.count(event.window)) {
    ButtonEvent buttonEvent;
    buttonEvent.type = EventType::BUTTON_PRESS;
    buttonEvent.x = event.x;
    buttonEvent.y = event.y;
    buttonEvent.x_root = event.x_root;
    buttonEvent.y_root = event.y_root;
    buttonEvent.window = event.window;
    buttonEvent.root = event.root;
    buttonEvent.leftButton = event.button == Button1;
    buttonEvent.rightButton = event.button == Button3;
    buttonEvent.button = event.button;
    pressMap[event.window](buttonEvent);
  }
}

void EventHandler::handleButtonRelease(const XButtonEvent event)
{
  if (releaseMap.count(event.window)) {
    ButtonEvent buttonEvent;
    buttonEvent.type = EventType::BUTTON_RELEASE;
    buttonEvent.x = event.x;
    buttonEvent.y = event.y;
    buttonEvent.x_root = event.x_root;
    buttonEvent.y_root = event.y_root;
    buttonEvent.window = event.window;
    buttonEvent.root = event.root;
    buttonEvent.leftButton = event.button == Button1;
    buttonEvent.rightButton = event.button == Button3;
    buttonEvent.button = event.button;
    releaseMap[event.window](buttonEvent);
  }
}

void EventHandler::handleMotionNotify(const XMotionEvent event)
{
  if (motionMap.count(event.window)) {
    Event motionEvent;
    motionEvent.type = EventType::MOTION;
    motionEvent.x = event.x;
    motionEvent.y = event.y;
    motionEvent.x_root = event.x_root;
    motionEvent.y_root = event.y_root;
    motionEvent.window = event.window;
    motionEvent.root = event.root;
    motionMap[event.window](motionEvent);
  }
}

void EventHandler::addEventCb(EventType type, Window id, const EventCallback& cb)
{
  switch (type) {
    case EventType::MOTION: {
      motionMap[id] = cb;
      break;
    }
    case EventType::BUTTON_PRESS: {
      pressMap[id] = cb;
      break;
    }
    case EventType::BUTTON_RELEASE: {
      releaseMap[id] = cb;
      break;
    }
  }
}
