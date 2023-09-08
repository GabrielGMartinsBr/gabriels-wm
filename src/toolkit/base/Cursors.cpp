#include "Cursors.h"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

Cursors::Cursors(Display* dis)
{
  display = dis;
  createCursors();
}

void Cursors::createCursors()
{
  cursorMap[CursorKey::DEFAULT] = XCreateFontCursor(display, XC_arrow);
  // cursorMap[CursorKey::POINTER] = XCreateFontCursor(display, XC_hand2);
  cursorMap[CursorKey::POINTER] = XCreateFontCursor(display, XC_top_left_arrow);
  cursorMap[CursorKey::RESIZE_L] = XCreateFontCursor(display, XC_sb_left_arrow);
  cursorMap[CursorKey::RESIZE_R] = XCreateFontCursor(display, XC_sb_right_arrow);
  cursorMap[CursorKey::RESIZE_U] = XCreateFontCursor(display, XC_sb_up_arrow);
  cursorMap[CursorKey::RESIZE_D] = XCreateFontCursor(display, XC_sb_down_arrow);
  cursorMap[CursorKey::RESIZE_H] = XCreateFontCursor(display, XC_sb_h_double_arrow);
  cursorMap[CursorKey::RESIZE_V] = XCreateFontCursor(display, XC_sb_v_double_arrow);
  cursorMap[CursorKey::RESIZE_TL] = XCreateFontCursor(display, XC_top_left_corner);
  cursorMap[CursorKey::RESIZE_TR] = XCreateFontCursor(display, XC_top_right_corner);
  cursorMap[CursorKey::RESIZE_BL] = XCreateFontCursor(display, XC_bottom_left_corner);
  cursorMap[CursorKey::RESIZE_BR] = XCreateFontCursor(display, XC_bottom_right_corner);
}

void Cursors::set(Window win, CursorKey key)
{
  Cursor cursor = cursorMap[key];
  XDefineCursor(display, win, cursor);
}