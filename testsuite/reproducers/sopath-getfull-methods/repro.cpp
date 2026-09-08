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
// P2 fix (code review): this reproducer originally cross-checked the
// new getFullXxx() methods against reinterpret_cast<SoFullPath *>
// (path)->getXxx() on a plain SoPath -- exactly the UB this whole
// branch exists to eliminate, confirmed by the review with
// -fsanitize=vptr. Replaced that with two independent checks that
// introduce no cast at all:
//
//   - Check 1 (below) cross-checks the new methods against values
//     obtained through entirely different, already-legitimate
//     routes: SoBaseKit::getPart() (asks the nodekit catalog for
//     "translator1" directly, with no path involved at all) for the
//     expected tail node, and SoPath::getIndex()/getNode() (the
//     plain, absolute-index accessors, which SoFullPath doesn't
//     override and which already operate correctly across the full
//     range -- see SoPath::getNode()'s own bounds check, which uses
//     getFullLength()) for the expected index/node-at-position.
//
//   - Check 2 (further below) tests SoFullPath's own implementation
//     (which, after this branch's Phase 1, delegates to the new
//     SoPath methods instead of touching this->nodes/indices
//     directly) using a real SoTempPath instead of a cast:
//     SoTempPath genuinely inherits from SoFullPath (see
//     include/Inventor/misc/SoTempPath.h), so calling
//     SoFullPath::getTail()/getLength()/etc. on one is completely
//     legitimate -- no cast, no UB -- and its length/tail/etc are
//     fully known here since the path is built by hand from three
//     plain nodes.
//
// This builds a real SoNodeKitPath into an SoTransformerDragger's
// hidden internal geometry (a nodekit "part"), where the
// hidden-vs-full distinction is actually exercised (a plain
// SoSeparator-only path has no hidden nodes and wouldn't tell
// getLength() and getFullLength() apart).

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>

int
main()
{
  SoDB::init();
  SoInteraction::init();

  int failures = 0;

  // *** Check 1: real nodekit path, hidden children actually exercised ***

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
  // into geom -- what matters for this check is just that the path
  // runs through the nodekit's hidden catalog machinery at all, which
  // createPathToPart() already guarantees (SoNodeKitPath paths always
  // carry hidden nodes for the kit's internal structure down to the
  // named part).
  SoPath * path = kp;

  // Independently-obtained expected tail: ask the nodekit catalog
  // directly, no path involved.
  SoNode * expectedtail = dragger->getPart("translator1", TRUE);
  if (!expectedtail) {
    fprintf(stderr, "[repro] FAIL: getPart(\"translator1\") returned NULL\n");
    failures++;
  }

  int plainlength = path->getLength();
  int fulllength = path->getFullLength();
  SoNode * plaintail = path->getTail();
  SoNode * fulltail = path->getFullTail();
  SoNode * fullnodefromtail = path->getFullNodeFromTail(0);
  int fullindexfromtail = path->getFullIndexFromTail(0);

  fprintf(stderr, "[repro] plain getLength()=%d  getFullLength()=%d\n",
          plainlength, fulllength);
  fprintf(stderr, "[repro] plain getTail()=%p  getFullTail()=%p  expected=%p\n",
          (void *)plaintail, (void *)fulltail, (void *)expectedtail);
  fprintf(stderr, "[repro] getFullNodeFromTail(0)=%p  getFullIndexFromTail(0)=%d\n",
          (void *)fullnodefromtail, fullindexfromtail);

  // The whole point of the split: full length must exceed the plain,
  // hidden-node-truncated length for a real nodekit path, otherwise
  // this test isn't actually exercising the distinction it claims to.
  if (fulllength <= plainlength) {
    fprintf(stderr, "[repro] FAIL: expected getFullLength() > getLength() for a nodekit path\n");
    failures++;
  }
  if (fulltail != expectedtail) {
    fprintf(stderr, "[repro] FAIL: getFullTail() doesn't match getPart(\"translator1\")\n");
    failures++;
  }
  if (fullnodefromtail != expectedtail) {
    fprintf(stderr, "[repro] FAIL: getFullNodeFromTail(0) doesn't match getPart(\"translator1\")\n");
    failures++;
  }
  // Cross-check against the plain, absolute-index accessors (which
  // SoFullPath doesn't override, and which already range over the
  // full path -- see SoPath::getNode()'s own bounds check).
  if (path->getNode(fulllength - 1) != expectedtail) {
    fprintf(stderr, "[repro] FAIL: getNode(getFullLength()-1) doesn't match getPart(\"translator1\")\n");
    failures++;
  }
  if (fullindexfromtail != path->getIndex(fulllength - 1)) {
    fprintf(stderr, "[repro] FAIL: getFullIndexFromTail(0) doesn't match getIndex(getFullLength()-1)\n");
    failures++;
  }

  kp->unref();
  root->unref();

  // *** Check 2: SoFullPath's own implementation, via a real SoTempPath ***

  SoSeparator * broot = new SoSeparator;
  broot->ref();
  SoSeparator * bbranch = new SoSeparator;
  broot->addChild(bbranch);
  SoCube * bleaf = new SoCube;
  bbranch->addChild(bleaf);

  // SoTempPath genuinely *is* an SoFullPath (real inheritance, see
  // include/Inventor/misc/SoTempPath.h) -- built by hand here, so its
  // length/tail/etc are fully known values, not derived from anything
  // being tested.
  SoTempPath temppath(3);
  temppath.ref();
  temppath.append(broot);
  temppath.append(bbranch);
  temppath.append(bleaf);

  fprintf(stderr, "[repro] SoTempPath: SoFullPath::getLength()=%d getTail()=%p\n",
          temppath.getLength(), (void *)temppath.getTail());

  if (temppath.getLength() != 3) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getLength() on SoTempPath expected 3\n");
    failures++;
  }
  if (temppath.getTail() != bleaf) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getTail() on SoTempPath expected bleaf\n");
    failures++;
  }
  if (temppath.getNodeFromTail(0) != bleaf) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getNodeFromTail(0) on SoTempPath expected bleaf\n");
    failures++;
  }
  if (temppath.getIndexFromTail(0) != 0) {
    fprintf(stderr, "[repro] FAIL: SoFullPath::getIndexFromTail(0) on SoTempPath expected 0 "
                    "(bleaf is bbranch's only child)\n");
    failures++;
  }
  // A path built with no hidden children at all: plain and full must
  // agree exactly.
  if (temppath.getFullLength() != 3) {
    fprintf(stderr, "[repro] FAIL: SoPath::getFullLength() on SoTempPath expected 3\n");
    failures++;
  }
  if (temppath.getFullTail() != bleaf) {
    fprintf(stderr, "[repro] FAIL: SoPath::getFullTail() on SoTempPath expected bleaf\n");
    failures++;
  }
  if (temppath.getFullNodeFromTail(0) != bleaf) {
    fprintf(stderr, "[repro] FAIL: SoPath::getFullNodeFromTail(0) on SoTempPath expected bleaf\n");
    failures++;
  }
  if (temppath.getFullIndexFromTail(0) != 0) {
    fprintf(stderr, "[repro] FAIL: SoPath::getFullIndexFromTail(0) on SoTempPath expected 0\n");
    failures++;
  }

  // temppath is stack-allocated: only ref() it (as an established
  // idiom elsewhere in Coin, e.g. SoPickedPoint::applyMatrixAction(),
  // does -- to silence an internal Coin warning), never unref() it --
  // SoBase::unref() calls delete this once the count reaches zero,
  // which would be fatal for a non-heap object. Its destructor runs
  // normally via the stack unwind at the end of main().
  broot->unref();

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
