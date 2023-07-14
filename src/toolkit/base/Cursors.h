#include <X11/Xlib.h>
#include <X11/cursorfont.h>

struct Cursors {
  Cursor Default;
  Cursor Pointer;

  Cursors(Display * d);
};