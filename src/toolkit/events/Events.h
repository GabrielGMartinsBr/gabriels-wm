#pragma once

#include <X11/X.h>

enum EventType {
  BUTTON_PRESS,
  BUTTON_RELEASE,
  MOTION,
};

struct Event {
  EventType type;
  int x;
  int y;
  int x_root;
  int y_root;
  Window window;
  Window root;
};

struct ButtonEvent : Event {
  bool leftButton;
  bool rightButton;
  unsigned int button;
};
