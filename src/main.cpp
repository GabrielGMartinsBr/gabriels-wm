#include "WindowManager.h"
#include "toolkit/Elementor.h"

int main(int, char **)
{
  Elementor::init();

  WindowManager wm(&Elementor::central);
  wm.run();

  return 0;
}
