#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
class PanelComponent {
 public:
  PanelComponent(
    Display *dpy,
    Window root
  )
  {
    this->dpy = dpy;
    this->root = root;

    createWindow();
    showWindow();
  }

 private:
  Display *dpy;
  Window root;
  Window win;

  void createWindow()
  {
    int x = 0;
    int y = 0;
    int width = 1100;
    int height = 40;
    int border = 0;
    u_int borderColor = 0;
    u_int bg = 0x121215;
    win = XCreateSimpleWindow(dpy, root, x, y, width, height, border, borderColor, bg);
  }

  void showWindow()
  {
    XMapWindow(dpy, win);
  }
};