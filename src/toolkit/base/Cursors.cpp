#include "Cursors.h"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

Cursors::Cursors(Display* d)
{
  Default = XCreateFontCursor(d, XC_arrow);
  Pointer = XCreateFontCursor(d, XC_hand2);
}
