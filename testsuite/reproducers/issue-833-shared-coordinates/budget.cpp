#include "shapenodes/SoVertexArrayTexCoordGuard.h"

#include <climits>
#include <cstdio>

struct BudgetCase {
  int coordinates;
  std::int64_t referenced;
  SbBool expected;
};

int
main()
{
  const int maxCoordinates = static_cast<int>(
    (std::size_t(32) * 1024 * 1024) / sizeof(SbVec4f));
  const BudgetCase cases[] = {
    { 410881, 4096, FALSE },
    { 4096, 4096, TRUE },
    { 8256, 4096, TRUE },
    { 8257, 4096, FALSE },
    { maxCoordinates, INT_MAX, TRUE },
    { maxCoordinates + 1, INT_MAX, FALSE },
    { 1000, LLONG_MAX, TRUE },
    { -1, 1, FALSE },
    { 1, -1, FALSE },
    { 64, 0, TRUE },
    { 65, 0, FALSE }
  };

  for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    const SbBool actual = SoVertexArrayTexCoordGuard::isAffordable(
      cases[i].coordinates, cases[i].referenced);
    if (actual != cases[i].expected) {
      std::fprintf(stderr,
        "case %u failed: coordinates=%d referenced=%lld expected=%d got=%d\n",
        i, cases[i].coordinates,
        static_cast<long long>(cases[i].referenced),
        static_cast<int>(cases[i].expected), static_cast<int>(actual));
      return 1;
    }
  }
  return 0;
}
