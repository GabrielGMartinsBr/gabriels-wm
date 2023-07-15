#include "LauncherEntry.h"

#include "cairo.h"

LauncherEntry::LauncherEntry(const char* txt) :
    bgColor("#444"),
    textColor("#eee")
{
  text = txt;
}

void LauncherEntry::setText(std::string value)
{
  text = value;
}

void LauncherEntry::updateMetrics(cairo_t* cr)
{
  cairo_select_font_face(
    cr,
    "Arial",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_BOLD
  );
  cairo_set_font_size(cr, 16);
  cairo_text_extents(cr, text.c_str(), &extents);

  width = extents.width + px * 2;
}

void LauncherEntry::draw(Tracer* tr)
{
  cairo_t* cr = tr->cr;
  updateMetrics(cr);
  tr->setColor(bgColor);
  cairo_rectangle(cr, x, y, width, height);
  cairo_fill(cr);

  tr->setColor(textColor);
  int tx = x + px;
  int ty = y + (height + extents.height) / 2.;
  cairo_move_to(cr, tx, ty);
  cairo_show_text(cr, text.c_str());
}

bool LauncherEntry::isHover(int _x, int _y)
{
  return _x > x
         && _x < x + width;
}
