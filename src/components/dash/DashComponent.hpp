#pragma once

#include <X11/Xlib.h>

class DashComponent {
 public:
  DashComponent(
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
    int x = 300;
    int y = 900;
    int width = 500;
    int height = 80;
    int border = 0;
    u_int borderColor = 0;
    u_int bg = 0x333336;
    win = XCreateSimpleWindow(dpy, root, x, y, width, height, border, borderColor, bg);
  }

  void showWindow()
  {
    XMapWindow(dpy, win);
  }
};