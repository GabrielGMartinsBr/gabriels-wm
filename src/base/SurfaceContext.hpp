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

  SurfaceContext& setSourceColor(uint r, uint g, uint b)
  {
    color.setColor(r, g, b);
    updateColor();
    return *this;
  }

  SurfaceContext& setSourceColor(float red, float green, float blue)
  {
    color.setColor(red, green, blue);
    updateColor();
    return *this;
  }

  SurfaceContext& setSourceColor(ulong hex)
  {
    color.setColor(hex);
    updateColor();
    return *this;
  }

  SurfaceContext& setSourceColor(const char* str)
  {
    color.setColor(str);
    updateColor();
    return *this;
  }

  SurfaceContext& paint()
  {
    cairo_paint(cr);
    return *this;
  }
  SurfaceContext& paintWithAlpha(double alpha)
  {
    cairo_paint_with_alpha(cr, alpha);
    return *this;
  }
  SurfaceContext& stroke()
  {
    cairo_stroke(cr);
    return *this;
  }
  SurfaceContext& fill()
  {
    cairo_fill(cr);
    return *this;
  }

  SurfaceContext& rectangle(double x, double y, double width, double height)
  {
    cairo_rectangle(cr, x, y, width, height);
    return *this;
  }

  SurfaceContext& arc(
    double xc, double yc,
    double radius,
    double angle1, double angle2
  )
  {
    cairo_arc(cr, xc, yc, radius, angle1, angle2);
    return *this;
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