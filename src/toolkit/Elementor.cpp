#include "Elementor.h"

#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#include <math.h>

#include "Button.h"
#include "Central.h"

Central Elementor::central;

void Elementor::init()
{
  central.init();
}

Button *Elementor::button(
  Window parent,
  int x, int y,
  int w, int h,
  unsigned long bgColor,
  unsigned long borderColor
)
{
  Button *button = new Button(
    &central,
    parent, x, y, w, h,
    bgColor, borderColor
  );
  return button;
}
