#pragma once

#include <X11/Xlib.h>
#include <sstream>

using ::std::string;

string getEventName(const XEvent &e);