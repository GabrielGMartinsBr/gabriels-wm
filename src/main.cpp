#include "WindowManager.h"
#include "toolkit/Elementor.h"
// #include "./composite/CompositeTest.h"


int main(int, char **)
{
  Elementor::init();

  WindowManager wm(&Elementor::central);
  wm.run();
  return 0;

  // Compositor compositor;
  // return compositor.run();
}
