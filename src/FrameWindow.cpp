#include "./FrameWindow.h"

#include "toolkit/Elementor.h"

FrameWindow::FrameWindow(
  Central* ct,
  Window cWin
) :
    maximized(false)
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
    0, 0, bgColor
  );

  long evtMasks = SubstructureRedirectMask | SubstructureNotifyMask;
  XSelectInput(display, frameWindow, evtMasks);

  XReparentWindow(display, cWin, frameWindow, borderWidth, topHeight);

  XMapWindow(display, frameWindow);

  closeButton = Elementor::button(
    frameWindow, width - 15, 6, 12, 12,
    0xff4030
  );
  closeButton->onClick([=]() {
    XKillClient(display, cWin);
  });

  maximizeButton = Elementor::button(
    frameWindow, width - 31, 6, 12, 12,
    0x00df30
  );
  maximizeButton->onClick([this]() {
    handleMaximizeClick();
  });
  minimizeButton = Elementor::button(
    frameWindow, width - 47, 6, 12, 12,
    0xefdf00
  );
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
  XMoveWindow(display, closeButton->win, _width - 15, 6);
  XMoveWindow(display, maximizeButton->win, _width - 30, 6);
  XMoveWindow(display, minimizeButton->win, _width - 45, 6);
}