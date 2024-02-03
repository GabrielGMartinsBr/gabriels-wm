#pragma once

#include "Color.hpp"
#include "cairo.h"

namespace App {

class SurfaceContext {
 public:
  SurfaceContext(cairo_surface_t* sfc)
  {
    create(sfc);
  }

  cairo_t* context()
  {
    return cr;
  }

  void setSourceColor(uint r, uint g, uint b)
  {
    color.setColor(r, g, b);
    updateColor();
  }

  void setSourceColor(float red, float green, float blue)
  {
    color.setColor(red, green, blue);
    updateColor();
  }

  void setSourceColor(ulong hex)
  {
    color.setColor(hex);
    updateColor();
  }

  void setSourceColor(const char* str)
  {
    color.setColor(str);
    updateColor();
  }

  void paint() { cairo_paint(cr); }
  void paintWithAlpha(double alpha) { cairo_paint_with_alpha(cr, alpha); }
  void stroke() { cairo_stroke(cr); }
  void fill() { cairo_fill(cr); }

  void rectangle(double x, double y, double width, double height)
  {
    cairo_rectangle(cr, x, y, width, height);
  }

  void arc(
    double xc, double yc,
    double radius,
    double angle1, double angle2
  )
  {
    cairo_arc(cr, xc, yc, radius, angle1, angle2);
  }

 private:
  cairo_t* cr;
  Color color;

  void create(cairo_surface_t* sfc)
  {
    cr = cairo_create(sfc);
  }

  void updateColor()
  {
    cairo_set_source_rgb(cr, color.red(), color.green(), color.blue());
  }
};

}  // namespace App