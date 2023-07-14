#include "Tracer.h"

Tracer::Tracer(cairo_t* cairo)
{
  cr = cairo;
}

void Tracer::setColor(Color& color)
{
  cairo_set_source_rgb(cr, color.r, color.g, color.b);
}
