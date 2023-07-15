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
  cursorMap[CursorKey::POINTER] = XCreateFontCursor(display, XC_hand2);
}

void Cursors::set(Window win, CursorKey key)
{
  Cursor cursor = cursorMap[key];
  XDefineCursor(display, win, cursor);
}