#ifndef COIN_SOHEIGHTMAPTONORMALMAPP_H
#define COIN_SOHEIGHTMAPTONORMALMAPP_H

#include <cstddef>
#include <limits>

namespace coin_heightmap {

static inline std::size_t
pixelCount(int width, int height, int components)
{
  if (width <= 0 || height <= 0 || components < 1 || components > 4) {
    return 0;
  }

  // SoSFImage adds three to the byte count in int before rounding down
  // to a multiple of four. SbImage uses the same alignment in size_t.
  const std::size_t maxBytes = (std::numeric_limits<int>::max)() - 3;
  const std::size_t maxComponents = components > 3 ? components : 3;
  const std::size_t maxPixels = maxBytes / maxComponents;
  const std::size_t columns = width;
  const std::size_t rows = height;
  return rows <= maxPixels / columns ? columns * rows : 0;
}

} // namespace coin_heightmap

#endif // COIN_SOHEIGHTMAPTONORMALMAPP_H
