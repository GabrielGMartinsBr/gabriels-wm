#include "WindowManager.h"
#include "toolkit/Elementor.h"

int main(int, char **)
{
  Elementor::init();

  WindowManager wm;
  wm.run(&Elementor::central);

  return 0;
}
