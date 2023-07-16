#include "WindowUtils.h"

char *WindowUtils::getNameProp(Display *d, const Window w)
{
  Atom propAtom = XInternAtom(d, "_NET_WM_NAME", False);
  XTextProperty nameProp;
  int result = XGetTextProperty(d, w, &nameProp, propAtom);
  if (result) {
    return reinterpret_cast<char *>(nameProp.value);
  }

  propAtom = XInternAtom(d, "WM_NAME", False);
  result = XGetTextProperty(d, w, &nameProp, propAtom);
  if (result) {
    return reinterpret_cast<char *>(nameProp.value);
  }

  return nullptr;
}

void WindowUtils::setAlwaysOnTop(Display *display, Window window)
{
  Atom atomNetWmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", 1);
  Atom atomNetWmState = XInternAtom(display, "_NET_WM_STATE", 1);

  XClientMessageEvent ev;
  ev.type = ClientMessage;
  ev.window = window;
  ev.message_type = atomNetWmState;
  ev.format = 32;
  ev.data.l[0] = 1;
  ev.data.l[1] = atomNetWmStateAbove;
  ev.data.l[2] = 0;
  ev.data.l[3] = 1;
  ev.data.l[4] = 0;

  Window root = DefaultRootWindow(display);

  XSendEvent(
    display,
    root,
    false,
    SubstructureRedirectMask | SubstructureNotifyMask,
    (XEvent *)&ev
  );
  XFlush(display);
}