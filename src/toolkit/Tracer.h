#pragma once

#include "base/Color.h"
#include "cairo.h"

struct Tracer {
  cairo_t* cr;

  Tracer(cairo_t* cairo);

  void setColor(Color& color);
};