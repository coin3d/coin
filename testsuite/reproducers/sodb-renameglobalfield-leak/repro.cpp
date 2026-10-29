// Reproducer for a memory leak in SoDB::renameGlobalField()
// (src/misc/SoDB.cpp), the documented way to discard a global field
// created via SoDB::createGlobalField() ("If \a to is an empty name, the
// \a from field gets deleted. If another global field already goes by
// the name \a to, that field will get deleted before the rename
// operation.").
//
// Traced by hand (src/misc/SoDB.cpp, src/fields/SoGlobalField.cpp,
// src/lists/SoBaseList.cpp): both "delete" branches of
// renameGlobalField() called only SoGlobalField::removeGlobalFieldContainer(),
// which does allcontainers->removeItem(fieldcontainer) -- and
// SoGlobalField::initClass() creates that list with addReferences(FALSE),
// so SoBaseList::remove()/removeItem() take the "unless addReferences()
// has been set to FALSE" branch and never call unref(). Nothing else in
// either code path unref()s or deletes the field container -- it is just
// silently dropped from the only list that was tracking it, at whatever
// refcount it already had. This is the exact same bug class fixed for
// SoDBP::removeRealTimeFieldCB() in 2014 (Coin issue #73 / commit
// 3695870b88) -- but renameGlobalField(), the function that fix's own
// explanatory comment and SoGlobalField::clean()'s shutdown warning both
// point applications at as *the* correct way to discard a global field,
// was never itself touched, and has zero callers anywhere in Coin's own
// source tree or test suite to have ever exercised it.
//
// SoDB::init() itself demonstrates the intended ownership protocol for
// "realTime": ref() the container right after creating it, hold that
// single reference for as long as you want to keep the field, then
// release it exactly once via the delete path. This repro follows the
// same protocol (via SoField::getContainer(), the public way to reach
// the field container without needing the unexported SoGlobalField
// type), then relies on LeakSanitizer (run this under an ASan build) to
// report any SoField/SoGlobalField allocations still reachable -- i.e.
// never freed -- when the process exits.
//
// Exercises both leak sites in renameGlobalField(): renaming to an empty
// name (delete outright), and renaming onto an existing name (which
// deletes the pre-existing field by that name).
//
// Also exercises renaming a field to its own current name: the "onto an
// existing name" lookup then finds the very same container being
// renamed, so unref()'ing it as if it were a distinct pre-existing entry
// is a use-after-free (the subsequent gf->setName(to) dereferences the
// now-deleted object) -- caught by ASan on the very first fix attempt at
// this leak (which did not special-case from == to).
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <cstdio>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoFieldContainer.h>

int main()
{
  SoDB::init();

  const int n = 100;

  // Branch 1: renameGlobalField(name, "") -- delete outright.
  for (int i = 0; i < n; i++) {
    char name[32];
    snprintf(name, sizeof(name), "leaktest_empty_%d", i);
    SoField * f = SoDB::createGlobalField(SbName(name), SoSFFloat::getClassTypeId());
    f->getContainer()->ref(); // same protocol SoDB::init() uses for "realTime"
    SoDB::renameGlobalField(SbName(name), SbName("")); // documented way to delete it
  }

  // Branch 2: renameGlobalField(newname, oldname) where oldname already
  // exists -- the pre-existing "oldname" field is documented to "get
  // deleted before the rename operation".
  for (int i = 0; i < n; i++) {
    char oldname[32], newname[32];
    snprintf(oldname, sizeof(oldname), "leaktest_old_%d", i);
    snprintf(newname, sizeof(newname), "leaktest_new_%d", i);
    SoField * of = SoDB::createGlobalField(SbName(oldname), SoSFFloat::getClassTypeId());
    of->getContainer()->ref();
    SoField * nf = SoDB::createGlobalField(SbName(newname), SoSFFloat::getClassTypeId());
    nf->getContainer()->ref();
    // "oldname"'s pre-existing field container should get deleted here;
    // "newname"'s survives, renamed to "oldname".
    SoDB::renameGlobalField(SbName(newname), SbName(oldname));
    // Clean up the survivor too via the same documented delete path, so
    // this loop doesn't itself leave anything dangling for
    // SoDB::finish() to (correctly) warn about and not free.
    SoDB::renameGlobalField(SbName(oldname), SbName(""));
  }

  // Branch 3: renameGlobalField(name, name) -- renaming a field to its
  // own current name must be a no-op, not a use-after-free.
  for (int i = 0; i < n; i++) {
    char name[32];
    snprintf(name, sizeof(name), "leaktest_self_%d", i);
    SoField * f = SoDB::createGlobalField(SbName(name), SoSFFloat::getClassTypeId());
    f->getContainer()->ref();
    SoDB::renameGlobalField(SbName(name), SbName(name));
    // Field must still be alive and findable under its own name.
    if (SoDB::getGlobalField(SbName(name)) != f) {
      fprintf(stderr, "[repro] FAIL: self-rename lost or corrupted the field\n");
      return 1;
    }
    SoDB::renameGlobalField(SbName(name), SbName("")); // clean up
  }

  fprintf(stderr, "[repro] exercised both renameGlobalField() delete paths "
                  "%d times each, plus %d self-renames\n", n, n);

  SoDB::finish(); // exercise the documented cleanup path (also frees "realTime")

  fprintf(stderr, "[repro] done -- run under LeakSanitizer/valgrind to see "
                  "whether the discarded fields were actually freed\n");
  return 0;
}
