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
  cursorMap[CursorKey::MOVE] = XCreateFontCursor(display, XC_fleur);
  cursorMap[CursorKey::GRAB] = XCreateFontCursor(display, XC_hand1);
  cursorMap[CursorKey::POINTER] = XCreateFontCursor(display, XC_hand2);
  cursorMap[CursorKey::RESIZE_LEFT] = XCreateFontCursor(display, XC_sb_left_arrow);
  cursorMap[CursorKey::RESIZE_RIGHT] = XCreateFontCursor(display, XC_sb_right_arrow);
  cursorMap[CursorKey::RESIZE_UP] = XCreateFontCursor(display, XC_sb_up_arrow);
  cursorMap[CursorKey::RESIZE_DOWN] = XCreateFontCursor(display, XC_sb_down_arrow);
  cursorMap[CursorKey::RESIZE_HORIZONTAL] = XCreateFontCursor(display, XC_sb_h_double_arrow);
  cursorMap[CursorKey::RESIZE_VERTICAL] = XCreateFontCursor(display, XC_sb_v_double_arrow);
  cursorMap[CursorKey::RESIZE_UP_LEFT] = XCreateFontCursor(display, XC_top_left_corner);
  cursorMap[CursorKey::RESIZE_UP_RIGHT] = XCreateFontCursor(display, XC_top_right_corner);
  cursorMap[CursorKey::RESIZE_DOWN_LEFT] = XCreateFontCursor(display, XC_bottom_left_corner);
  cursorMap[CursorKey::RESIZE_DOWN_RIGHT] = XCreateFontCursor(display, XC_bottom_right_corner);
}

void Cursors::set(Window win, CursorKey key)
{
  Cursor cursor = cursorMap[key];
  XDefineCursor(display, win, cursor);
}