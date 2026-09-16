// Independent contract oracle for SoTempPath's non-owning/non-auditing policy.
//
// Authored from the frozen Coin 4 intent and baseline
// 40c6372d2984a94c6ac17f059caee7b8d6374654 without access to an
// implementation candidate. See README.md for scope and Unknown policy.

#include <cstdio>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>

struct Fixture {
  Fixture()
    : root(new SoGroup), child(new SoSeparator), source(new SoPath(root))
  {
    root->ref();
    child->ref();
    root->addChild(child);
    source->ref();
    source->append(child);
  }

  ~Fixture()
  {
    source->unref();
    root->unref();
    child->unref();
  }

  SoGroup * root;
  SoSeparator * child;
  SoPath * source;
};

static bool
same_route(const SoTempPath & path, const Fixture & fixture)
{
  return path.getLength() == 2 &&
         path.getNode(0) == fixture.root && path.getIndex(0) == 0 &&
         path.getNode(1) == fixture.child && path.getIndex(1) == 0;
}

static int
nonempty_destination()
{
  Fixture fixture;
  SoTempPath path(2);
  path.setHead(fixture.root);

  const int rootrefs = fixture.root->getRefCount();
  const int childrefs = fixture.child->getRefCount();
  path.append(fixture.source);

  if (!same_route(path, fixture)) {
    std::fprintf(stderr, "FAIL: non-empty append did not copy the route\n");
    return 1;
  }
  if (fixture.root->getRefCount() != rootrefs ||
      fixture.child->getRefCount() != childrefs) {
    std::fprintf(stderr, "FAIL: non-empty append changed node ownership\n");
    return 1;
  }
  return 0;
}

static int
empty_destination()
{
  Fixture fixture;
  SoTempPath path(2);

  const int rootrefs = fixture.root->getRefCount();
  const int childrefs = fixture.child->getRefCount();
  path.append(fixture.source);

  if (!same_route(path, fixture)) {
    std::fprintf(stderr, "FAIL: empty-destination append did not copy route\n");
    return 1;
  }
  if (fixture.root->getRefCount() != rootrefs ||
      fixture.child->getRefCount() != childrefs) {
    std::fprintf(stderr,
                 "FAIL: empty-destination append acquired node ownership\n");
    return 1;
  }

  // Both paths currently describe root -> child. Removing that child must
  // update the ordinary auditing SoPath, but not the temporary path. The
  // fixture's explicit child reference keeps the stale raw pointer alive for
  // this observation.
  fixture.root->removeChild(0);
  if (fixture.source->getLength() != 1) {
    std::fprintf(stderr, "FAIL: control SoPath did not audit child removal\n");
    return 1;
  }
  if (!same_route(path, fixture)) {
    std::fprintf(stderr,
                 "FAIL: SoTempPath audited child removal after route copy\n");
    return 1;
  }
  return 0;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr,
                 "usage: %s nonempty-destination|empty-destination\n",
                 argv[0]);
    return 2;
  }
  SoDB::init();
  int result = 2;
  if (std::strcmp(argv[1], "nonempty-destination") == 0)
    result = nonempty_destination();
  else if (std::strcmp(argv[1], "empty-destination") == 0)
    result = empty_destination();
  else
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
  SoDB::finish();
  return result;
}
