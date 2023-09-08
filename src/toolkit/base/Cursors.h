#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include <map>

enum CursorKey {
  DEFAULT,
  POINTER,
  RESIZE_LEFT,
  RESIZE_RIGHT,
  RESIZE_UP,
  RESIZE_DOWN,
  RESIZE_HORIZONTAL,
  RESIZE_VERTICAL,
  RESIZE_UP_LEFT,
  RESIZE_UP_RIGHT,
  RESIZE_DOWN_LEFT,
  RESIZE_DOWN_RIGHT,
};

struct Cursors {
  std::map<CursorKey, Cursor> cursorMap;
  Display* display;

  Cursors(Display* dis);

  void createCursors();

  void set(Window win, CursorKey key);
};
