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

  // Get Cairo Context
  cairo_t* context()
  {
    return cr;
  }

  // Set Source Color By RGB
  SurfaceContext& setSourceColor(uint r, uint g, uint b)
  {
    color.setColor(r, g, b);
    updateColor();
    return *this;
  }

  // Set Source Color By RGB Float
  SurfaceContext& setSourceColor(float red, float green, float blue)
  {
    color.setColor(red, green, blue);
    updateColor();
    return *this;
  }

  // Set Source Color By Color Hex
  SurfaceContext& setSourceColor(ulong hex)
  {
    color.setColor(hex);
    updateColor();
    return *this;
  }

  // Set Source Color By Color String
  SurfaceContext& setSourceColor(const char* str)
  {
    color.setColor(str);
    updateColor();
    return *this;
  }

  // Paint
  SurfaceContext& paint()
  {
    cairo_paint(cr);
    return *this;
  }

  // Paint With Alpha
  SurfaceContext& paintWithAlpha(double alpha)
  {
    cairo_paint_with_alpha(cr, alpha);
    return *this;
  }

  // Stroke
  SurfaceContext& stroke()
  {
    cairo_stroke(cr);
    return *this;
  }

  // Fill
  SurfaceContext& fill()
  {
    cairo_fill(cr);
    return *this;
  }

  // Rectangle
  SurfaceContext& rectangle(double x, double y, double width, double height)
  {
    cairo_rectangle(cr, x, y, width, height);
    return *this;
  }

  // Arc
  SurfaceContext& arc(
    double xc, double yc,
    double radius,
    double angle1, double angle2
  )
  {
    cairo_arc(cr, xc, yc, radius, angle1, angle2);
    return *this;
  }

  // Move To
  SurfaceContext& moveTo(double x, double y)
  {
    cairo_move_to(cr, x, y);
    return *this;
  }

  // Line To
  SurfaceContext& lineTo(double x, double y)
  {
    cairo_line_to(cr, x, y);
    return *this;
  }

  // Line Width
  SurfaceContext& lineWidth(double width)
  {
    cairo_set_line_width(cr, width);
    return *this;
  }

  // Line Cap
  cairo_line_cap_t getLineCap() { return cairo_get_line_cap(cr); }
  SurfaceContext& setLineCap(cairo_line_cap_t line_cap)
  {
    cairo_set_line_cap(cr, line_cap);
    return *this;
  }

  // Line Join
  cairo_line_join_t getLineJoin() { return cairo_get_line_join(cr); }
  SurfaceContext& setLineJoin(cairo_line_join_t line_join)
  {
    cairo_set_line_join(cr, line_join);
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