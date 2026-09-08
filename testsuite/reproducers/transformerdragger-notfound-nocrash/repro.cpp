// Reproducer for Coin issue #174 ("Removal of 'found' Assertion in
// SoTransformerDragger::dragStart()").
//
// SoTransformerDragger::dragStart() tries to figure out which of its 20
// named interactive parts (translator1-6, rotator1-6, scale1-8) was
// actually picked, by searching the pick path and by checking
// getSurrogatePartPickedName() (used when this dragger's geometry is
// substituted in as a surrogate for a named part of some other node/kit).
// If none of those 20 names match -- which the reporter hit in a
// multi-SoSelection/multi-dragger setup, and which can equally happen
// whenever this dragger is wired up as a surrogate under a name other
// than one of those 20 -- `found` stayed FALSE and the old code hit
// `assert(found)`, aborting the whole application in a debug build.
//
// This left PRIVATE(this)->whatkind at its constructor-initialized value
// of WHATKIND_NONE (dragStart() only ever sets it inside a `found`
// branch). drag() and dragFinish() switch on that same whatkind, and
// their `default:` branches (also `assert(0 && ...)` before this fix)
// are exactly what runs afterward for a fully "not found" drag: dragStart
// used to abort via its own assert before ever reaching drag()/
// dragFinish(), but a debug build with COIN_DEBUG-only asserts stripped
// in some other way (or any code path that leaves whatkind at
// WHATKIND_NONE) would go on to hit drag()'s and dragFinish()'s own
// identical assert(0) sites the moment the user moved the mouse or
// released the button -- the exact same crash, one call later.
//
// This repro exercises the part of the fix that is deterministically
// reachable without wiring up a full multi-SoSelection GUI scenario or
// the nodekit surrogate-part-substitution machinery: it drives drag()
// and dragFinish() directly on a freshly constructed dragger, which is
// guaranteed by the constructor to have whatkind == WHATKIND_NONE (the
// exact state dragStart() leaves behind when it can't find a match) --
// proving the "unknown/none" case in both functions no longer aborts.
// (dragStart()'s own new early-return was verified by source inspection:
// see the commit message for why driving it live would require either a
// real multi-selection pick-routing scenario or the surrogate-part
// mechanism, neither of which is exercised here.)
//
// drag()/dragStart()/dragFinish() are `protected` on SoTransformerDragger
// -- ExposedTransformerDragger below is plain, un-registered C++
// subclassing (no new SO_KIT type) purely to call them from this repro.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/draggers/SoTransformerDragger.h>

class ExposedTransformerDragger : public SoTransformerDragger {
public:
  void testDrag(void) { this->drag(); }
  void testDragFinish(void) { this->dragFinish(); }
};

int
main()
{
  SoDB::init();
  SoInteraction::init();

  ExposedTransformerDragger * dragger = new ExposedTransformerDragger;
  dragger->ref();

  fprintf(stderr, "[repro] calling drag() on a fresh dragger (whatkind == WHATKIND_NONE)...\n");
  dragger->testDrag();
  fprintf(stderr, "[repro] drag() returned without aborting\n");

  fprintf(stderr, "[repro] calling dragFinish() on the same dragger...\n");
  dragger->testDragFinish();
  fprintf(stderr, "[repro] dragFinish() returned without aborting\n");

  dragger->unref();

  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
