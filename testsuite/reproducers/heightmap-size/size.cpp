#include <Inventor/SbImage.h>
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoHeightMapToNormalMap.h>
#include "engines/SoHeightMapToNormalMapP.h"

#include <cstring>

namespace {

const SbVec2s extent(3, 2);
const unsigned char heightmap[] = {
  0, 77, 255, 12,  255, 0, 42, 11,  0, 181, 46, 0,
  255, 15, 0, 25,  0, 111, 63, 42,  0, 71, 66, 255
};
const unsigned char normals[] = {
  13, 128, 185,  242, 128, 185,  128, 128, 255,
  13, 128, 185,  242, 128, 185,  128, 128, 255
};

template <typename Image>
bool empty(const Image & image)
{
  SbVec2s size;
  int nc;
  const unsigned char * bytes = image.getValue(size, nc);
  return !bytes && size == SbVec2s(0, 0) && nc == 0;
}

template <typename Image>
bool matchesNormals(const Image & image)
{
  SbVec2s size;
  int nc;
  const unsigned char * bytes = image.getValue(size, nc);
  return bytes && size == extent && nc == 3 &&
         std::memcmp(bytes, normals, sizeof normals) == 0;
}

bool enginePublishesEmptyImage()
{
  SoDB::init();
  SoHeightMapToNormalMap * engine = new SoHeightMapToNormalMap;
  engine->ref();
  SoSFImage output;
  const bool connected = output.connectFrom(&engine->image);
  engine->sourceImage.setValue(extent, 4, heightmap);
  const bool valid = connected && matchesNormals(output);
  engine->sourceImage.setValue(SbVec2s(0, 0), 0, NULL);
  const bool cleared = empty(output);
  output.disconnect();
  engine->unref();
  return valid && cleared;
}

} // namespace

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  const unsigned char pixel = 0;
  SbImage result;
  // Check that rejection also clears a previously populated destination.
  result.setValue(SbVec2s(1, 1), 1, &pixel);
  const char * test = argv[1];
  if (std::strcmp(test, "negative") == 0) {
    SoHeightMapToNormalMap::convert(&pixel, SbVec2s(-1, 0), 1, result);
  }
  else if (std::strcmp(test, "zero") == 0) {
    SoHeightMapToNormalMap::convert(&pixel, SbVec2s(0, 1), 1, result);
  }
  else if (std::strcmp(test, "components") == 0) {
    SoHeightMapToNormalMap::convert(&pixel, SbVec2s(1, 1), 0, result);
    if (!empty(result)) return 1;
    result.setValue(SbVec2s(1, 1), 1, &pixel);
    const unsigned char fiveComponents[] = {0, 1, 2, 3, 4};
    SoHeightMapToNormalMap::convert(fiveComponents, SbVec2s(1, 1), 5, result);
  }
  else if (std::strcmp(test, "null") == 0) {
    SoHeightMapToNormalMap::convert(NULL, SbVec2s(1, 1), 1, result);
  }
  else if (std::strcmp(test, "output_limit") == 0) {
    // These RGB byte counts straddle INT_MAX - 3: 2147483640 and
    // 2147483646. Check the arithmetic without allocating either image.
    return coin_heightmap::pixelCount(24295, 29464, 1) == 715827880u &&
           coin_heightmap::pixelCount(32767, 21846, 1) == 0 ? 0 : 1;
  }
  else if (std::strcmp(test, "source_limit") == 0) {
    // Same RGBA width on both sides of the limit. The rejected extent
    // still fits as RGB, so the source stride determines the limit.
    return coin_heightmap::pixelCount(16385, 32766, 4) == 536870910u &&
           coin_heightmap::pixelCount(16385, 32767, 4) == 0 &&
           coin_heightmap::pixelCount(16385, 32767, 3) == 536887295u ? 0 : 1;
  }
  else if (std::strcmp(test, "valid") == 0) {
    SoHeightMapToNormalMap::convert(heightmap, extent, 4, result);
    return matchesNormals(result) ? 0 : 1;
  }
  else if (std::strcmp(test, "engine") == 0) {
    return enginePublishesEmptyImage() ? 0 : 1;
  }
  else return 2;
  return empty(result) ? 0 : 1;
}
