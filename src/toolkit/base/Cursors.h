#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include <map>

enum CursorKey {
  DEFAULT,
  POINTER,
  RESIZE_L,
  RESIZE_R,
  RESIZE_U,
  RESIZE_D,
  RESIZE_H,
  RESIZE_V,
  RESIZE_TL,
  RESIZE_TR,
  RESIZE_BL,
  RESIZE_BR,
};

struct Cursors {
  std::map<CursorKey, Cursor> cursorMap;
  Display* display;

  Cursors(Display* dis);

  void createCursors();

  void set(Window win, CursorKey key);
};
