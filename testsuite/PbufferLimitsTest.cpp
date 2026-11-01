// Regression coverage for the private offscreen size-limit calculation.
// No OpenGL context, display server, or installed Coin library is needed.
#include "glue/pbuffer_limits.h"
#include <cstdio>

static bool
check(unsigned int width, unsigned int height,
      unsigned int maxwidth, unsigned int maxheight, unsigned int maxpixels,
      unsigned int expectedwidth, unsigned int expectedheight)
{
  const unsigned int limits[3] = { maxwidth, maxheight, maxpixels };
  coin_glglue_clamp_pbuffer_dimensions(width, height, limits);
  if (width == expectedwidth && height == expectedheight) return true;
  std::fprintf(stderr, "limits %u x %u, %u pixels: got %u x %u, expected %u x %u\n",
               maxwidth, maxheight, maxpixels, width, height,
               expectedwidth, expectedheight);
  return false;
}

int main()
{
  bool ok = true;
  // The SSH/XQuartz failure: zero pixels must not collapse usable dimensions.
  ok = check(16384, 16384, 4096, 4096, 0, 4096, 4096) && ok;
  // An absent pixel-count hint must not discard tighter per-axis limits.
  ok = check(4096, 4096, 1024, 512, 0, 1024, 512) && ok;
  // Viewport limits remain effective even when pbuffer limits are larger.
  ok = check(800, 600, 4096, 4096, 0, 800, 600) && ok;
  // Positive pixel-count limits must still be honored with the same clamping.
  ok = check(4096, 4096, 4096, 4096, 4096 * 4096, 4096, 4096) && ok;
  ok = check(128, 64, 128, 64, 2048, 64, 32) && ok;
  ok = check(128, 128, 128, 128, 1, 1, 1) && ok;
  // Zero dimension limits remain zero; this workaround only concerns pixels.
  ok = check(800, 600, 0, 4096, 0, 0, 600) && ok;
  ok = check(800, 600, 4096, 0, 0, 800, 0) && ok;
  return ok ? 0 : 1;
}
