#include "./FrameWindow.h"

#include <X11/Xlib.h>

#include <iostream>

#include "cairo.h"
#include "toolkit/Elementor.h"
#include "toolkit/events/Events.h"
#include "toolkit/utils/WindowUtils.h"

FrameWindow::FrameWindow(
  Central* ct,
  Window cWin
) :
    maximized(false),
    bgColor(BG_COLOR_HEX)
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

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask
                  | ButtonPressMask | ButtonReleaseMask
                  | Button1MotionMask
                  | ExposureMask
                  | VisibilityChangeMask
                  | EnterWindowMask;
  XSelectInput(display, frameWindow, evtMasks);

  XReparentWindow(display, cWin, frameWindow, borderWidth, topHeight);

  XMapWindow(display, frameWindow);

  closeButton = Elementor::button(
    frameWindow, width - 19, 4, 15, 15,
    0xff4030
  );
  closeButton->onClick([=](Event e) {
    XKillClient(display, cWin);
  });

  maximizeButton = Elementor::button(
    frameWindow, width - 37, 4, 15, 15,
    0x00df30
  );

  auto closeCb = [this](Event e) {
    handleMaximizeClick();
  };

  maximizeButton->onClick(closeCb);
  minimizeButton = Elementor::button(
    frameWindow, width - 55, 4, 15, 15,
    0xefdf00
  );

  central->eventsHandler->addEventCb(
    EventType::BUTTON_PRESS,
    minimizeButton->win,
    [this](Event e) {
      handleMaximizeClick();
    }
  );

  setupCairo();
  getWinAttrs();
  drawElements();
  registerEvents();
}

void FrameWindow::setupCairo()
{
  int screen = DefaultScreen(display);
  Visual* visual = DefaultVisual(display, screen);
  sfc = cairo_xlib_surface_create(
    display, frameWindow, visual, width, topHeight
  );

  cr = cairo_create(sfc);

  titleSfc = cairo_image_surface_create(
    CAIRO_FORMAT_ARGB32, width, topHeight
  );

  cairo_save(cr);
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

void FrameWindow::drawElements()
{
  drawTitle();

  cairo_set_source_surface(cr, titleSfc, 0, 0);
  cairo_paint(cr);

  cairo_surface_flush(sfc);
  XFlush(display);
}

void FrameWindow::drawTitle()
{
  cairo_t* titleCr = cairo_create(titleSfc);
  cairo_set_source_rgb(titleCr, 1.0, 1.0, 1.0);
  cairo_select_font_face(titleCr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(titleCr, 12);
  cairo_move_to(titleCr, 6, 16);
  cairo_show_text(titleCr, winName);
}

void FrameWindow::registerEvents()
{
  central->eventsHandler->addEventCb(
    EventType::BUTTON_PRESS,
    frameWindow,
    [this](Event e) {
      handleButtonEvent(true, e.x, e.y);
    }
  );
  central->eventsHandler->addEventCb(
    EventType::BUTTON_RELEASE,
    frameWindow,
    [this](Event e) {
      handleButtonEvent(false, e.x, e.y);
    }
  );
  central->eventsHandler->addEventCb(
    EventType::MOTION,
    frameWindow,
    [this](Event e) {
      handleMotionEvent(e.x, e.y);
    }
  );
}

void FrameWindow::handleXEvent(const XEvent evt)
{
  if (evt.type != Expose) {
    return;
  }

  cairo_set_source_rgb(cr, bgColor.r, bgColor.g, bgColor.b);
  cairo_paint(cr);

  cairo_set_source_surface(cr, titleSfc, 0, 0);
  cairo_paint(cr);

  cairo_surface_flush(sfc);
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
  XWindowAttributes winAttrs;
  XGetWindowAttributes(display, central->rootWindow, &winAttrs);
  int contentWidth = winAttrs.width - borderWidth * 2;
  int contentHeight = winAttrs.height - topHeight;
  XSetWindowBorderWidth(display, frameWindow, 0);
  XMoveResizeWindow(display, frameWindow, 0, 0, winAttrs.width, winAttrs.height);
  XResizeWindow(display, contentWindow, contentWidth, contentHeight);
  maximized = true;
  updateButtonsPosition();
}

void FrameWindow::restoreSize()
{
  int cWidth = width - borderWidth * 2;
  int cHeight = height - topHeight - borderWidth;
  XMoveResizeWindow(display, frameWindow, x, y, width, height);
  XResizeWindow(display, contentWindow, cWidth, cHeight);
  XSetWindowBorderWidth(display, frameWindow, 1);
  maximized = false;
  updateButtonsPosition();
}

void FrameWindow::updateButtonsPosition()
{
  int _width = width;
  if (maximized) {
    // TODO: add root window dimensions on central
    XWindowAttributes winAttrs;
    XGetWindowAttributes(display, central->rootWindow, &winAttrs);
    _width = winAttrs.width;
  }
  XMoveWindow(display, closeButton->win, _width - 19, 6);
  XMoveWindow(display, maximizeButton->win, _width - 37, 6);
  XMoveWindow(display, minimizeButton->win, _width - 55, 6);
}

void FrameWindow::handleButtonEvent(bool status, int _x, int _y)
{
  if (status) {
    XRaiseWindow(display, frameWindow);
    startDrag(_x, _y);
  } else if (!status && dragging) {
    stopDrag(_x, _y);
  }
}

void FrameWindow::handleMotionEvent(int _x, int _y)
{
  updateDrag(_x, _y);
}

void FrameWindow::startDrag(int _x, int _y)
{
  dragging = true;
  dragInitX = _x;
  dragInitY = _y;
}

void FrameWindow::updateDrag(int _x, int _y)
{
  if (!dragging) {
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
  dragging = false;
}
