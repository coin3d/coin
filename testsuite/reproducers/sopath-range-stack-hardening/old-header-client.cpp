#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>

#include <cstdio>
#include <cstring>

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s sopath_pop_empty|sofullpath_pop_empty\n", argv[0]);
    return 2;
  }

  SoDB::init();
  SoTempPath * path = new SoTempPath(1);
  path->ref();

  if (std::strcmp(argv[1], "sopath_pop_empty") == 0) {
    static_cast<SoPath *>(path)->pop();
  }
  else if (std::strcmp(argv[1], "sofullpath_pop_empty") == 0) {
    static_cast<SoFullPath *>(path)->pop();
  }
  else {
    path->unref();
    SoDB::finish();
    return 2;
  }

  const bool unchanged = static_cast<SoFullPath *>(path)->getLength() == 0;
  path->unref();
  SoDB::finish();
  std::fprintf(stderr, "[old-header] %s %s\n", argv[1],
               unchanged ? "PRESERVED" : "VIOLATED");
  return unchanged ? 0 : 1;
}
