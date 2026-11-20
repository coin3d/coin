#include <Inventor/SbImage.h>
#include <Inventor/engines/SoHeightMapToNormalMap.h>

#include <cstring>

static bool empty(const SbImage & image)
{
  SbVec2s size;
  int nc;
  const unsigned char * bytes = image.getValue(size, nc);
  return !bytes && size == SbVec2s(0, 0) && nc == 0;
}

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
  }
  else if (std::strcmp(test, "null") == 0) {
    SoHeightMapToNormalMap::convert(NULL, SbVec2s(1, 1), 1, result);
  }
  else if (std::strcmp(test, "output_limit") == 0) {
    SoHeightMapToNormalMap::convert(&pixel, SbVec2s(32767, 21846), 1, result);
  }
  else if (std::strcmp(test, "source_limit") == 0) {
    SoHeightMapToNormalMap::convert(&pixel, SbVec2s(32767, 16385), 4, result);
  }
  else if (std::strcmp(test, "valid") == 0) {
    const unsigned char source[] = {0, 77, 255, 12};
    SoHeightMapToNormalMap::convert(source, SbVec2s(1, 1), 4, result);
    SbVec2s size;
    int nc;
    const unsigned char * bytes = result.getValue(size, nc);
    return bytes && size == SbVec2s(1, 1) && nc == 3 &&
           bytes[0] == 128 && bytes[1] == 128 && bytes[2] == 255 ? 0 : 1;
  }
  else return 2;
  return empty(result) ? 0 : 1;
}
