// Correctness check for SoPath::getFull{Tail,NodeFromTail,IndexFromTail,
// Length}() -- Phase 1 of fixing the SoPath/SoFullPath
// reinterpret_cast UB pattern (see the SoDragger callback-list fix on
// the neighboring branch for the same UB category applied to function
// pointers instead of downcasts).
//
// SoPath::getTail()/getLength()/etc. only consider nodes down to the
// first one with hidden children (nodekit-internal implementation
// nodes); SoFullPath's identically-named methods -- historically
// reached only via reinterpret_cast<SoFullPath*>(path), which is UB
// whenever the pointee was never actually constructed as a SoFullPath
// -- expose the raw, complete node list including those hidden nodes.
// The new SoPath::getFullXxx() methods expose the exact same "raw"
// data directly on SoPath, without any cast.
//
// This builds a real SoNodeKitPath into an SoTransformerDragger's
// hidden internal geometry (a nodekit "part"), where the
// hidden-vs-full distinction is actually exercised (a plain
// SoSeparator-only path has no hidden nodes and wouldn't tell
// getLength() and getFullLength() apart), and checks:
//   - SoPath::getLength()/getTail() still stop at the first hidden
//     node (unchanged, pre-existing behavior).
//   - SoPath::getFullLength()/getFullTail()/getFullNodeFromTail()/
//     getFullIndexFromTail() return the same values the *old*
//     reinterpret_cast<SoFullPath*>(path)->getXxx() pattern would --
//     compared directly against that still-present, still-legal (if
//     UB) pattern here, since no call site has been migrated to the
//     new methods yet.
//   - SoFullPath::getTail()/getLength()/etc, now implemented by
//     delegating to the new SoPath methods instead of touching
//     this->nodes/indices directly, still return the same values too.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/nodes/SoSeparator.h>

int
main()
{
  SoDB::init();
  SoInteraction::init();

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoTransformerDragger * dragger = new SoTransformerDragger;
  root->addChild(dragger);

  SoNodeKitPath * kp = dragger->createPathToPart("translator1", TRUE);
  if (!kp) {
    fprintf(stderr, "[repro] FAIL: createPathToPart(\"translator1\") returned NULL\n");
    root->unref();
    return 1;
  }
  kp->ref();
  // createPathToPart() only returns a path to the dragger itself, not
  // into geom -- append the rest by hand isn't needed: what matters
  // for this check is just that the path runs through the nodekit's
  // hidden catalog machinery at all, which createPathToPart() already
  // guarantees (SoNodeKitPath paths always carry hidden nodes for the
  // kit's internal structure down to the named part).
  SoPath * path = kp;

  int plainlength = path->getLength();
  int fulllength_new = path->getFullLength();
  int fulllength_old = reinterpret_cast<SoFullPath *>(path)->getLength();

  SoNode * plaintail = path->getTail();
  SoNode * fulltail_new = path->getFullTail();
  SoNode * fulltail_old = reinterpret_cast<SoFullPath *>(path)->getTail();

  SoNode * fullnodefromtail_new = path->getFullNodeFromTail(0);
  SoNode * fullnodefromtail_old = reinterpret_cast<SoFullPath *>(path)->getNodeFromTail(0);

  int fullindexfromtail_new = path->getFullIndexFromTail(0);
  int fullindexfromtail_old = reinterpret_cast<SoFullPath *>(path)->getIndexFromTail(0);

  fprintf(stderr, "[repro] plain getLength()=%d  getFullLength() new=%d old=%d\n",
          plainlength, fulllength_new, fulllength_old);
  fprintf(stderr, "[repro] plain getTail()=%p  getFullTail() new=%p old=%p\n",
          (void *)plaintail, (void *)fulltail_new, (void *)fulltail_old);
  fprintf(stderr, "[repro] getFullNodeFromTail(0) new=%p old=%p\n",
          (void *)fullnodefromtail_new, (void *)fullnodefromtail_old);
  fprintf(stderr, "[repro] getFullIndexFromTail(0) new=%d old=%d\n",
          fullindexfromtail_new, fullindexfromtail_old);

  int failures = 0;

  // The whole point of the split: full length must exceed the plain,
  // hidden-node-truncated length for a real nodekit path, otherwise
  // this test isn't actually exercising the distinction it claims to.
  if (fulllength_new <= plainlength) {
    fprintf(stderr, "[repro] FAIL: expected getFullLength() > getLength() for a nodekit path\n");
    failures++;
  }
  if (fulllength_new != fulllength_old) {
    fprintf(stderr, "[repro] FAIL: getFullLength() disagrees with the old reinterpret_cast pattern\n");
    failures++;
  }
  if (fulltail_new != fulltail_old) {
    fprintf(stderr, "[repro] FAIL: getFullTail() disagrees with the old reinterpret_cast pattern\n");
    failures++;
  }
  if (fullnodefromtail_new != fullnodefromtail_old) {
    fprintf(stderr, "[repro] FAIL: getFullNodeFromTail(0) disagrees with the old reinterpret_cast pattern\n");
    failures++;
  }
  if (fullindexfromtail_new != fullindexfromtail_old) {
    fprintf(stderr, "[repro] FAIL: getFullIndexFromTail(0) disagrees with the old reinterpret_cast pattern\n");
    failures++;
  }
  // getFullTail() is, by definition, the very last node -- must match
  // getFullNodeFromTail(0).
  if (fulltail_new != fullnodefromtail_new) {
    fprintf(stderr, "[repro] FAIL: getFullTail() != getFullNodeFromTail(0)\n");
    failures++;
  }

  // SoFullPath, now delegating to the new SoPath methods instead of
  // touching this->nodes/indices directly, must still match too.
  SoFullPath * fp = reinterpret_cast<SoFullPath *>(path);
  if (fp->getLength() != fulllength_new) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getLength() no longer matches getFullLength()\n");
    failures++;
  }
  if (fp->getTail() != fulltail_new) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getTail() no longer matches getFullTail()\n");
    failures++;
  }
  if (fp->getNodeFromTail(0) != fullnodefromtail_new) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getNodeFromTail(0) no longer matches getFullNodeFromTail(0)\n");
    failures++;
  }
  if (fp->getIndexFromTail(0) != fullindexfromtail_new) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getIndexFromTail(0) no longer matches getFullIndexFromTail(0)\n");
    failures++;
  }

  kp->unref();
  root->unref();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
