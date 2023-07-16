#include "./FrameWindow.h"

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>

#include "cairo-xlib.h"
#include "cairo.h"
#include "toolkit/base/Color.h"
#include "toolkit/utils/WindowUtils.h"

FrameWindow::FrameWindow(
  Central* ct,
  Window cWin
) :
    maximized(false),
    bgColor(BG_COLOR_HEX),
    closeButton(0xff4030),
    maximizeButton(0x00df30),
    minimizeButton(0xefdf00)
{
  central = ct;
  display = ct->display;
  contentWindow = cWin;

  XWindowAttributes winAttrs;
  XGetWindowAttributes(display, cWin, &winAttrs);

  x = winAttrs.x;
  y = winAttrs.y;
  width = winAttrs.width + borderWidth * 2;
  height = winAttrs.height + topHeight + borderWidth;

  frameWindow = XCreateSimpleWindow(
    display, central->rootWindow,
    x, y, width, height,
    1, 0x333333, bgColor.hex
  );

  long frameEvents = SubstructureRedirectMask | SubstructureNotifyMask
                     | ButtonPressMask | ButtonReleaseMask
                     | Button1MotionMask
                     | PointerMotionMask
                     | ExposureMask
                     | VisibilityChangeMask
                     | EnterWindowMask;
  XSelectInput(display, frameWindow, frameEvents);

  XReparentWindow(display, cWin, frameWindow, borderWidth, topHeight);

  XMapWindow(display, frameWindow);

  setupCairo();
  getWinAttrs();
}

void FrameWindow::setupCairo()
{
  int screen = DefaultScreen(display);
  Visual* visual = DefaultVisual(display, screen);
  sfc = cairo_xlib_surface_create(
    display, frameWindow, visual, width, topHeight
  );
  cr = cairo_create(sfc);
  tr = new Tracer(cr);
}

void FrameWindow::handleXEvent(const XEvent evt)
{
  if (
    evt.xany.window != frameWindow
    && evt.xany.window != contentWindow
  ) {
    return;
  }

  switch (evt.type) {
    case Expose:
      if (evt.xexpose.window == frameWindow) {
        onExpose();
      }
      break;
    case FocusIn:
      std::cout << "FocusIn!" << '\n';
      break;
    case ButtonPress:
      handleButtonPress(evt.xbutton);
      break;
    case ButtonRelease:
      handleButtonRelease(evt.xbutton);
      break;
    case MotionNotify:
      handleMotion(evt.xmotion);
      break;
  }
}

void FrameWindow::onExpose()
{
  if (maximized && !isCairoMaximized) {
    cairo_xlib_surface_set_size(
      sfc,
      central->availWidth,
      topHeight
    );
    isCairoMaximized = true;
  }
  if (!maximized && isCairoMaximized) {
    cairo_xlib_surface_set_size(
      sfc,
      width,
      topHeight
    );
    isCairoMaximized = false;
  }

  cairo_set_source_rgb(
    cr,
    bgColor.r,
    bgColor.g,
    bgColor.b
  );
  cairo_paint(cr);
  drawTitle();
  drawFrameButtons();
}

void FrameWindow::handleButtonPress(const XButtonPressedEvent evt)
{
  if (evt.window != frameWindow) {
    return;
  }

  if (minimizeButton.isHover(evt.x, evt.y)) {
    return;
  }
  if (maximizeButton.isHover(evt.x, evt.y)) {
    handleMaximizeClick();
    return;
  }
  if (closeButton.isHover(evt.x, evt.y)) {
    closeWindow();
    return;
  }

  XRaiseWindow(display, frameWindow);

  if (evt.window == frameWindow) {
    startDrag(evt.x, evt.y);
  }
}

void FrameWindow::handleButtonRelease(const XButtonReleasedEvent evt)
{
  if (evt.window == frameWindow) {
    stopDrag(evt.x, evt.y);
  }
}

void FrameWindow::handleMotion(const XMotionEvent evt)
{
  if (evt.window != frameWindow) {
    return;
  }
  if (isDragging) {
    updateDrag(evt.x, evt.y);
    return;
  }
  if (
    minimizeButton.isHover(evt.x, evt.y)
    || maximizeButton.isHover(evt.x, evt.y)
    || closeButton.isHover(evt.x, evt.y)
  ) {
    if (!cursorChanged) {
      central->cursors->set(frameWindow, CursorKey::POINTER);
      cursorChanged = true;
    }
    return;
  }
  if (cursorChanged) {
    central->cursors->set(frameWindow, CursorKey::DEFAULT);
    cursorChanged = false;
  }
}

void FrameWindow::getWinAttrs()
{
  if (!contentWindow) {
    return;
  }
  winName = WindowUtils::getNameProp(
    display,
    contentWindow
  );
  if (!winName) {
    winName = "Dev Window";
  }
}

void FrameWindow::drawTitle()
{
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_select_font_face(
    cr,
    "Arial",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_BOLD
  );
  cairo_set_font_size(cr, 12);
  cairo_move_to(cr, 6, 16);
  cairo_show_text(cr, winName);
  cairo_fill(cr);
}

void FrameWindow::drawFrameButtons()
{
  int w = width;
  if (maximized) {
    w = central->availWidth;
  }

  int size = 13;
  int space = 3;
  int fSize = size + space;
  int len = 3;

  int radius = size / 2;
  int x = w - (len * fSize) + radius;
  int y = topHeight / 2;
  minimizeButton.x = x;
  minimizeButton.y = y;
  minimizeButton.radius = radius;
  x += fSize;
  maximizeButton.x = x;
  maximizeButton.y = y;
  maximizeButton.radius = radius;
  x += fSize;
  closeButton.x = x;
  closeButton.y = y;
  closeButton.radius = radius;

  minimizeButton.draw(tr);
  maximizeButton.draw(tr);
  closeButton.draw(tr);
}

void FrameWindow::handleMaximizeClick()
{
  if (maximized) {
    restoreSize();
  } else {
    maximize();
  }
}

void FrameWindow::maximize()
{
  int fullWidth = central->availWidth;
  int fullHeight = central->availHeight;

  int contW = fullWidth - 2;
  int contH = fullHeight - topHeight - 1;

  XSetWindowBorderWidth(display, frameWindow, 0);
  XMoveResizeWindow(display, frameWindow, 0, 0, fullWidth, fullHeight);
  XMoveResizeWindow(display, contentWindow, 1, topHeight, contW, contH);
  maximized = true;
}

void FrameWindow::restoreSize()
{
  int cWidth = width - borderWidth * 2;
  int cHeight = height - topHeight - borderWidth;
  XMoveResizeWindow(display, frameWindow, x, y, width, height);
  XMoveResizeWindow(
    display,
    contentWindow,
    borderWidth,
    topHeight,
    cWidth,
    cHeight
  );
  XSetWindowBorderWidth(display, frameWindow, 1);
  maximized = false;
}

void FrameWindow::closeWindow()
{
  Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
  XEvent event;
  event.type = ClientMessage;
  event.xclient.window = contentWindow;
  event.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", false);
  event.xclient.format = 32;
  event.xclient.data.l[0] = wmDeleteWindow;
  event.xclient.data.l[1] = CurrentTime;
  XSendEvent(display, contentWindow, false, NoEventMask, &event);
}

void FrameWindow::startDrag(int _x, int _y)
{
  isDragging = true;
  dragInitX = _x;
  dragInitY = _y;
}

void FrameWindow::updateDrag(int _x, int _y)
{
  if (!isDragging) {
    return;
  }
  int dx = _x - dragInitX;
  int dy = _y - dragInitY;
  x += dx;
  y += dy;
  XMoveWindow(display, frameWindow, x, y);
}

void FrameWindow::stopDrag(int _x, int _y)
{
  isDragging = false;
}

// CircleButton

FrameWindow::CircleButton::CircleButton(int hex) :
    color(hex)
{
  radius = 6;
}

void FrameWindow::CircleButton::draw(Tracer* tr)
{
  tr->setColor(color);
  cairo_arc(tr->cr, x, y, radius, 0, PI2);
  cairo_fill(tr->cr);
}

bool FrameWindow::CircleButton::isHover(int _x, int _y)
{
  double distance = std::sqrt(
    std::pow(_x - x, 2) + std::pow(_y - y, 2)
  );
  return distance <= radius;
}