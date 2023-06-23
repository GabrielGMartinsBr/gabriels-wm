#pragma once

#include "Button.h"
#include "Central.h"

// Element Creator
struct Elementor {
  static Central central;

  static void init();

  static Button* button(
    Window parent,
    int x, int y,
    int w, int h,
    unsigned long bgColor = 0xffffff,
    unsigned long borderColor = 0x000000
  );
};