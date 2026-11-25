#ifndef COIN_TEST_GLX_DISPLAY_CHECK_H
#define COIN_TEST_GLX_DISPLAY_CHECK_H

#if defined(COIN_TEST_HAS_X11)
#include <X11/Xlib.h>
#endif

#include <cstdio>
#include <cstdlib>

static bool
coinTestGLXDisplayAvailable(void)
{
#if defined(COIN_TEST_HAS_X11)
  Display * display = XOpenDisplay(NULL);
  if (display) {
    XCloseDisplay(display);
    return true;
  }
  std::fprintf(stderr, "SKIP: no accessible X display for GLX rendering\n");
  return false;
#else
  return true;
#endif
}

#endif
