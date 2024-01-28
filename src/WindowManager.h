#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "components/panel/PanelComponent.hpp"
#include "desktop/Desktop.hpp"
#include "frameWindow/FrameWindow.h"
#include "launcher/Launcher.h"
#include "toolkit/Central.h"

class WindowManager {
 public:
  WindowManager(Central *ct);
  ~WindowManager();

  void run();

 private:
  Central *central;
  Display *display;
  Window rootWindow;
  Desktop desktop;

  std::unique_ptr<PanelComponent> panel = nullptr;

  Launcher launcher;

  std::unordered_map<Window, Window> frames;
  std::unordered_map<Window, FrameWindow *> framesMap;

  void createPanel();

  void handleCreateNotify(const XCreateWindowEvent event);
  void handleConfigureRequest(const XConfigureRequestEvent event);
  void handleMapRequest(const XMapRequestEvent event);
  void handleMapNotify(const XMapEvent event);
  void handleUnmapNotify(const XUnmapEvent event);
  void handleReparentNotify(const XReparentEvent event);
  void handleButtonPress(const XButtonEvent event);
  void handleButtonRelease(const XButtonEvent event);
  void handleMotionNotify(const XMotionEvent event);
  void handleDestroyNotify(const XDestroyWindowEvent event);
  void handleIgnoredEvent(const XEvent &event);

  void addWindowFrame(Window window, Bool isPreExisting = false);

  void unFrame(const XUnmapEvent &evt);

  void getPreExistingWindows();
};