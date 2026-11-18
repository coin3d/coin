#include <Inventor/SoDB.h>
#include <Inventor/SbVec4f.h>

#include <cstdio>

// Test-only declarations of historical ELF symbols. This independent consumer
// deliberately includes no private headers and instantiates none of these
// implementation classes. Do not install these declarations as a public API.
class SoTextureCoordinateNormalMapP {
public:
  static SbVec4f * dummy_texcoords;
  static void cleanup_func(void);
};

class SoTextureCoordinateEnvironmentP {
public:
  static SbVec4f * dummy_texcoords;
  static void cleanup_func(void);
};

class SoTextureCoordinateReflectionMapP {
public:
  static SbVec4f * dummy_texcoords;
  static void cleanup_func(void);
};

// Volatile reads keep all three function-address relocations observable even
// with optimization or LTO. Calling cleanup here would double-delete at finish.
typedef void (*CleanupFunction)(void);
static CleanupFunction volatile cleanup_functions[] = {
  &SoTextureCoordinateNormalMapP::cleanup_func,
  &SoTextureCoordinateEnvironmentP::cleanup_func,
  &SoTextureCoordinateReflectionMapP::cleanup_func
};

struct LegacyCoordinate {
  const char * name;
  SbVec4f ** storage;
};

int
main()
{
  const LegacyCoordinate coordinates[] = {
    { "NormalMap", &SoTextureCoordinateNormalMapP::dummy_texcoords },
    { "Environment", &SoTextureCoordinateEnvironmentP::dummy_texcoords },
    { "ReflectionMap", &SoTextureCoordinateReflectionMapP::dummy_texcoords }
  };
  const unsigned count = sizeof(coordinates) / sizeof(coordinates[0]);
  for (unsigned i = 0; i < count; ++i) {
    if (cleanup_functions[i] == NULL) {
      std::fprintf(stderr, "%s: missing legacy cleanup address\n", coordinates[i].name);
      return 1;
    }
    if (*coordinates[i].storage != NULL) {
      std::fprintf(stderr, "%s: legacy storage is not NULL before SoDB::init\n",
                   coordinates[i].name);
      return 1;
    }
  }

  SoDB::init();
  int result = 0;
  for (unsigned i = 0; i < count; ++i) {
    const SbVec4f * value = *coordinates[i].storage;
    if (value == NULL) {
      std::fprintf(stderr, "%s: legacy storage was not allocated by SoDB::init\n",
                   coordinates[i].name);
      result = 1;
      continue;
    }
    for (unsigned component = 0; component < 4; ++component) {
      const float expected = component == 3 ? 1.0f : 0.0f;
      if ((*value)[component] != expected) {
        std::fprintf(stderr, "%s: legacy component %u is %g, expected %g\n",
                     coordinates[i].name, component,
                     double((*value)[component]), double(expected));
        result = 1;
      }
    }
  }

  // Finish owns the registered cleanups. Historical storage may remain a
  // dangling pointer afterward, so neither read it nor invoke cleanup manually.
  SoDB::finish();
  return result;
}
