// Regression test for a P1 code-review finding on an earlier version
// of this fix: SoSelection's five callback lists (selCBList,
// deselCBList, startCBList, finishCBList, changeCBList) must keep
// their historical type, SoCallbackList*, because they are
// `protected: // unfortunately only protected in OIV` members of the
// public header, and Coin's own SoExtSelection subclass reaches into
// startCBList/finishCBList directly. An earlier version of this fix
// retyped them to eliminate the function-pointer UB (see
// SoSelectionCBTrampolines' class comment in SoSelection.cpp for that
// UB's full explanation) -- code review confirmed this broke both:
//
//   - source compatibility: a third-party subclass following the
//     same, Coin-documented direct-access pattern hit an
//     incomplete-type compile error against the new header.
//   - binary compatibility: an *already-compiled* such subclass,
//     linked against the new library, called through the *old*
//     SoCallbackList ABI at an address that was now a different class
//     entirely -- caught with -fsanitize=function at
//     SoSelectionP.h:129.
//
// Fixed by keeping the field type completely unchanged and instead
// registering a correctly-typed trampoline function with the
// SoCallbackList -- see SoSelectionCBTrampolines.
//
// This test:
//
//  1. Defines TestExtSelection, a subclass reaching into
//     startCBList/finishCBList directly and calling
//     ->invokeCallbacks(this) on them -- mirroring SoExtSelection.cpp
//     line for line -- proving this still compiles (source
//     compatibility) and behaves correctly (calls the right
//     callbacks) against the fixed header/library.
//
//  2. Drives a real pick sequence through the public API (same as
//     socallbacklist-second-wave's SoSelection check) to confirm
//     ordinary application-level usage still works end to end.
//
//  3. Exercises removing a callback *during* another callback's
//     execution in the same invocation batch -- SoCallbackList::
//     invokeCallbacks()'s own documented contract is that every
//     callback registered *when the method was invoked* still fires
//     in that same batch even if another callback removes it
//     mid-invocation (only future invocations are affected) -- and
//     confirms that still holds (i.e. no crash/use-after-free from
//     the entry being freed too early, and the removed callback still
//     fires once more in the in-flight batch, then never again).

#include <cstdio>
#ifndef __has_feature
#define __has_feature(x) 0
#endif
#if __has_feature(address_sanitizer)
#include <sanitizer/allocator_interface.h>
#define CHECK_CALLBACK_ALLOCATIONS 1
#endif

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/SbViewportRegion.h>

// *** Check 1: direct protected-member access, mirroring SoExtSelection.cpp ***

static int extstartcount = 0;
static int extfinishcount = 0;

class TestExtSelection : public SoSelection {
public:
  void triggerStart(void) { this->startCBList->invokeCallbacks(this); }
  void triggerFinish(void) { this->finishCBList->invokeCallbacks(this); }
};

static void
extStartCB(void *, SoSelection *)
{
  extstartcount++;
}

static void
extFinishCB(void *, SoSelection *)
{
  extfinishcount++;
}

static SbBool
test_direct_protected_access(void)
{
  TestExtSelection * sel = new TestExtSelection;
  sel->ref();

  sel->addStartCallback(extStartCB);
  sel->addFinishCallback(extFinishCB);

  sel->triggerStart();
  sel->triggerFinish();
  sel->triggerStart();

  fprintf(stderr, "[repro] direct protected-member access: extstartcount=%d extfinishcount=%d\n",
          extstartcount, extfinishcount);

  sel->removeStartCallback(extStartCB);
  sel->removeFinishCallback(extFinishCB);

  sel->unref();

  return extstartcount == 2 && extfinishcount == 1;
}

// *** Check 2: ordinary end-to-end pick through the public API ***

static int selectcount = 0;
static int deselectcount = 0;

static void selectCB(void *, SoPath *) { selectcount++; }
static void deselectCB(void *, SoPath *) { deselectcount++; }

static void
click(SoHandleEventAction & action, SoNode * root, const SbVec2s & pos)
{
  SoMouseButtonEvent press;
  press.setButton(SoMouseButtonEvent::BUTTON1);
  press.setState(SoButtonEvent::DOWN);
  press.setPosition(pos);
  action.setEvent(&press);
  action.apply(root);

  SoMouseButtonEvent release;
  release.setButton(SoMouseButtonEvent::BUTTON1);
  release.setState(SoButtonEvent::UP);
  release.setPosition(pos);
  action.setEvent(&release);
  action.apply(root);
}

static SbBool
test_public_api_pick(void)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  root->addChild(camera);
  SoSelection * selection = new SoSelection;
  selection->policy = SoSelection::SINGLE;
  root->addChild(selection);
  selection->addChild(new SoCube);

  selection->addSelectionCallback(selectCB);
  selection->addDeselectionCallback(deselectCB);

  SbViewportRegion vp(400, 400);
  camera->viewAll(root, vp);
  SoHandleEventAction action(vp);

  click(action, root, SbVec2s(200, 200)); // hits the cube: select
  click(action, root, SbVec2s(10, 10));   // misses: SINGLE policy deselects

  fprintf(stderr, "[repro] public API pick: selectcount=%d deselectcount=%d\n",
          selectcount, deselectcount);

  selection->removeSelectionCallback(selectCB);
  selection->removeDeselectionCallback(deselectCB);
  root->unref();

  return selectcount == 1 && deselectcount == 1;
}

// *** Check 3: removing a callback from within another, same-batch ***

static int firstcount = 0;
static int secondcount = 0;
static SoSelection * removalvictim = NULL;

static void secondCB(void *, SoSelection *) { secondcount++; }

static void
firstCB(void *, SoSelection * sel)
{
  firstcount++;
  // Only remove secondCB the first time -- otherwise this fires again
  // (harmlessly) in batch 2 and posts an expected-but-noisy "tried to
  // remove non-existent callback" debug warning.
  if (firstcount != 1) return;
  // SoCallbackList::invokeCallbacks() documents that every callback
  // registered *when the method was invoked* still fires in this same
  // batch, even if another callback removes it mid-invocation -- only
  // later invocations are affected. So secondCB must still fire once
  // more after this removal, in this very call.
  sel->removeChangeCallback(secondCB, removalvictim);
}

static SbBool
test_remove_during_execution(void)
{
  // A local class that can call changeCBList->invokeCallbacks()
  // directly, same as addPath()/removePath() do internally -- the
  // object is genuinely constructed as this type from the start (no
  // downcast of a plain SoSelection*, which would itself be exactly
  // the kind of UB this whole branch exists to eliminate).
  class Trigger : public SoSelection {
  public:
    void fire(void) { this->changeCBList->invokeCallbacks(this); }
  };

  Trigger * sel = new Trigger;
  sel->ref();
  removalvictim = sel;

  sel->addChangeCallback(firstCB);
  sel->addChangeCallback(secondCB, removalvictim);

  sel->fire(); // batch 1: both fire; firstCB removes secondCB mid-batch
  fprintf(stderr, "[repro] after batch 1: firstcount=%d secondcount=%d\n",
          firstcount, secondcount);
  SbBool batch1ok = (firstcount == 1 && secondcount == 1);

  sel->fire(); // batch 2: only firstCB should fire now
  fprintf(stderr, "[repro] after batch 2: firstcount=%d secondcount=%d\n",
          firstcount, secondcount);
  SbBool batch2ok = (firstcount == 2 && secondcount == 1);

  sel->removeChangeCallback(firstCB);
  sel->unref();

  return batch1ok && batch2ok;
}

static int copiedcalls = 0;
static void copiedCB(void *, SoSelection *) { copiedcalls++; }

static SbBool
test_copied_list_lifetime(void)
{
  class Trigger : public SoSelection {
  public:
    const SoCallbackList & callbacks() const { return *this->changeCBList; }
    void clear() { this->changeCBList->clearCallbacks(); }
  };
  Trigger * sel = new Trigger;
  sel->ref();
  sel->addChangeCallback(copiedCB);
  SoCallbackList copied(sel->callbacks());
  SoCallbackList assigned;
  assigned = copied;
  sel->clear();
  sel->unref();
  copied.invokeCallbacks(NULL);
  copied.clearCallbacks();
  assigned.invokeCallbacks(NULL);
  assigned.clearCallbacks();
  fprintf(stderr, "[repro] copied/assigned lists after owner destruction: %d calls\n", copiedcalls);
  return copiedcalls == 2;
}

static int nestedcalls = 0;
static void nestedVictim(void *, SoSelection *) { nestedcalls++; }
class NestedSelection : public SoSelection {
public:
  void fire() { this->changeCBList->invokeCallbacks(this); }
  void clear() { this->changeCBList->clearCallbacks(); }
};
static void clearAndReenter(void *, SoSelection * selection)
{
  NestedSelection * sel = static_cast<NestedSelection *>(selection);
  sel->clear();
  sel->fire(); // empty nested invocation must not destroy the outer snapshot
}
static SbBool
test_clear_and_reenter(void)
{
  NestedSelection * sel = new NestedSelection;
  sel->ref();
  sel->addChangeCallback(clearAndReenter);
  sel->addChangeCallback(nestedVictim);
  sel->fire();
  sel->fire();
  sel->unref();
  return nestedcalls == 1;
}

// Protected and public APIs must retain the same callback/userdata identity.
static int mixedcalls = 0;
static void mixedTyped(void *, SoSelection *) { mixedcalls += 1; }
static void mixedGeneric(void *, void *) { mixedcalls += 10; }
static SbBool
test_mixed_registration_removal(void)
{
  class Trigger : public SoSelection {
  public:
    SoCallbackList & callbacks() { return *this->changeCBList; }
    void fire() { this->changeCBList->invokeCallbacks(this); }
  };
  Trigger * sel = new Trigger;
  sel->ref();
  sel->addChangeCallback(mixedTyped);
  sel->callbacks().removeCallback(reinterpret_cast<SoCallbackListCB *>(mixedTyped), NULL);
  sel->fire();
  sel->callbacks().addCallback(mixedGeneric);
  sel->addChangeCallback(mixedTyped);
  sel->callbacks().addCallback(mixedGeneric, sel);
  sel->addChangeCallback(mixedTyped, sel);
  sel->removeChangeCallback(reinterpret_cast<SoSelectionClassCB *>(mixedGeneric));
  sel->fire(); // remaining typed, generic, typed callbacks: 12
  sel->removeChangeCallback(mixedTyped);
  sel->fire(); // remaining generic, typed callbacks: 11
  sel->callbacks().clearCallbacks();
  sel->fire();
  sel->unref();
  fprintf(stderr, "[repro] mixed registration/removal: %d (expected 23)\n", mixedcalls);
  return mixedcalls == 23;
}

static SbBool
test_registration_churn(void)
{
#ifdef CHECK_CALLBACK_ALLOCATIONS
  SoSelection * sel = new SoSelection;
  sel->ref();
  sel->addChangeCallback(copiedCB);
  sel->removeChangeCallback(copiedCB); // warm up the ownership registry
  const size_t before = __sanitizer_get_current_allocated_bytes();
  for (int i = 0; i < 100000; ++i) {
    sel->addChangeCallback(copiedCB);
    sel->removeChangeCallback(copiedCB);
  }
  const size_t after = __sanitizer_get_current_allocated_bytes();
  fprintf(stderr, "[repro] live allocation bytes before/after 100000 registrations: %zu/%zu\n", before, after);
  sel->unref();
  return after <= before + 4096;
#else
  fprintf(stderr, "[repro] INFO: compile with Clang ASan to check registration memory retention\n");
  return TRUE;
#endif
}

int
main()
{
  SoDB::init();
  SoInteraction::init();

  int failures = 0;
  if (!test_registration_churn()) failures++;
  if (!test_mixed_registration_removal()) failures++;
  if (!test_copied_list_lifetime()) failures++;
  if (!test_clear_and_reenter()) failures++;
  if (!test_direct_protected_access()) { fprintf(stderr, "[repro] FAIL: direct protected-member access\n"); failures++; }
  if (!test_public_api_pick()) { fprintf(stderr, "[repro] FAIL: public API pick\n"); failures++; }
  if (!test_remove_during_execution()) { fprintf(stderr, "[repro] FAIL: remove during execution\n"); failures++; }

  if (failures > 0) {
    fprintf(stderr, "[repro] FAIL: %d check(s) failed\n", failures);
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
