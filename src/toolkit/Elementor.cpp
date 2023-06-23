#include "Elementor.h"

#include "Button.h"
#include "Central.h"

Central Elementor::central;

void Elementor::init()
{
  central.init();
}

Button Elementor::button(
  Window parent,
  int x, int y,
  int w, int h
)
{
  Button button(
    &central,
    parent, x, y, w, h
  );
  return button;
}