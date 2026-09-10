// Matrix and bounding-box cache checks shared with CoinTests.
// See run.sh for invocation. This driver does not exercise GL render caches.
#include <cstdio>
#include <Inventor/SoDB.h>
#include "../../IdentityTransformCacheTest.h"

static int failures = 0;
static void check(bool result, const char * message)
{
  if (!result) {
    fprintf(stderr, "FAIL: %s\n", message);
    ++failures;
  }
}

int main()
{
  SoDB::init();
  for (int kind = 0; kind < 3; ++kind)
    IdentityTransformCacheTest::run(kind, check);
  // All action and scene objects have already been destroyed.
  SoDB::finish();
  fprintf(stderr, "%s: identity transform cache checks\n",
          failures ? "FAIL" : "PASS");
  return failures ? 1 : 0;
}
