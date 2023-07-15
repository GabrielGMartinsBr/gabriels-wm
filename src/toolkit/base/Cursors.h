#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include <map>

enum CursorKey {
  DEFAULT,
  POINTER
};

struct Cursors {
  std::map<CursorKey, Cursor> cursorMap;
  Display* display;

  Cursors(Display* dis);

  void createCursors();

  void set(Window win, CursorKey key);
};
