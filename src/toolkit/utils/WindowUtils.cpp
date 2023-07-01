#include "WindowUtils.h"

char *WindowUtils::getNameProp(Display *d, const Window &w)
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