#pragma once

#include <string>

#include "../toolkit/Tracer.h"
#include "../toolkit/base/Color.h"
#include "cairo.h"

struct LauncherEntry {
  std::string text;
  Color bgColor;
  Color textColor;

  int width;
  int height;

  int px = 9;

  cairo_text_extents_t extents;

  LauncherEntry(const char* txt);

  void setText(std::string value);

  void updateMetrics(cairo_t* cr);

  void draw(Tracer* tr);
};