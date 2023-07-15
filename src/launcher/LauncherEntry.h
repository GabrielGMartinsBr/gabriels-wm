#pragma once

#include <string>

#include "../toolkit/Tracer.h"
#include "../toolkit/base/Color.h"
#include "cairo.h"

struct LauncherEntry {
  std::string text;
  std::string command;
  Color bgColor;
  Color textColor;

  int x;
  int y;
  int width;
  int height;

  int px = 9;

  cairo_text_extents_t extents;

  LauncherEntry(const char* txt, const char* cmd);

  void setText(std::string value);

  void updateMetrics(cairo_t* cr);

  void draw(Tracer* tr);

  bool isHover(int x, int y);
};