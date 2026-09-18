// Reproducer for a NULL-pointer dereference in SoField::disconnect(SoEngineOutput*)'s
// COIN_DEBUG_EXTRA diagnostic.
//
// The diagnostic accessed this->storage->container->getTypeId() directly,
// instead of the container==NULL-safe this->getContainer() accessor used
// everywhere else in this class (e.g. SoFieldP::getDebugIdString(), or the
// equivalent diagnostic in SoField::notify()). A field that was connected
// directly (never added to any SoFieldContainer, e.g. a standalone field
// object connected straight to an engine output) legitimately has a NULL
// container, so disconnecting it with the diagnostic enabled dereferenced
// NULL.
//
// This diagnostic is runtime-gated by coin_debug_extra() (set via the
// COIN_DEBUG_EXTRA env var, only meaningful in COIN_DEBUG/Debug builds) and
// COIN_WARNING_LEVEL>=3 -- both must be set for this to be reachable, which
// is why this needs to be run explicitly rather than via a normal ctest
// run. See run.sh in this directory.

#include "../CoinCleanup.h"
#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/engines/SoElapsedTime.h>

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  SoElapsedTime * engine = new SoElapsedTime;
  engine->ref();

  // A standalone field, never attached to any node -- container is NULL.
  SoSFFloat myfield;
  myfield.connectFrom(&engine->timeOut);

  if (myfield.getContainer() != NULL) {
    fprintf(stderr, "[repro] setup wrong: field unexpectedly has a container\n");
    engine->unref();
    return 2;
  }

  myfield.disconnect(&engine->timeOut);

  fprintf(stderr, "[repro] PASS: disconnect() of a container-less field survived\n");
  engine->unref();
  return 0;
}
