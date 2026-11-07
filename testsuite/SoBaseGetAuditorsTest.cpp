#include <Inventor/SoDB.h>
#include <Inventor/lists/SoAuditorList.h>
#include <Inventor/nodes/SoGroup.h>

#include <cstdio>
#include <cstring>

namespace {
struct Auditor {
  void * pointer;
  SoNotRec::Type type;
};

int failures = 0;

void check(const bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    ++failures;
  }
}

bool matches(const SoAuditorList & actual, const Auditor * expected,
             const int length)
{
  if (actual.getLength() != length) return false;
  for (int i = 0; i < length; ++i) {
    int actualcount = 0, expectedcount = 0;
    for (int j = 0; j < length; ++j) {
      if (actual.getObject(j) == expected[i].pointer &&
          actual.getType(j) == expected[i].type) ++actualcount;
      if (expected[j].pointer == expected[i].pointer &&
          expected[j].type == expected[i].type) ++expectedcount;
    }
    if (actualcount != expectedcount) return false;
  }
  return true;
}

void checkEmpty()
{
  SoGroup * node = new SoGroup;
  node->ref();
  const SoAuditorList & cached = node->getAuditors();
  check(cached.getLength() == 0, "initial empty cache is not empty");
  check(&node->getAuditors() == &cached && cached.getLength() == 0,
        "repeated empty query changed the cache");

  int token = 0;
  const Auditor expected[] = { { &token, SoNotRec::PARENT } };
  node->addAuditor(&token, SoNotRec::PARENT);
  check(matches(node->getAuditors(), expected, 1),
        "empty cache did not acquire the added auditor");
  node->removeAuditor(&token, SoNotRec::PARENT);
  check(node->getAuditors().getLength() == 0,
        "cache did not become empty after removing the last auditor");
  node->unref();
}

void checkRefresh()
{
  SoGroup * node = new SoGroup;
  node->ref();
  int tokens[4] = {};
  const Auditor initial[] = {
    { &tokens[0], SoNotRec::PARENT },
    { &tokens[1], SoNotRec::FIELD },
    { &tokens[2], SoNotRec::PARENT }
  };
  for (int i = 0; i < 3; ++i) node->addAuditor(initial[i].pointer, initial[i].type);
  const SoAuditorList & cached = node->getAuditors();
  check(matches(cached, initial, 3), "first populated query lost auditor pairs");
  for (int i = 0; i < 3; ++i) {
    const SoAuditorList & refreshed = node->getAuditors();
    check(&refreshed == &cached && matches(refreshed, initial, 3),
          "unchanged tree accumulated stale cache entries");
  }

  node->removeAuditor(initial[1].pointer, initial[1].type);
  const Auditor remaining[] = { initial[0], initial[2] };
  check(matches(node->getAuditors(), remaining, 2),
        "removed auditor remained in the refreshed cache");
  node->addAuditor(&tokens[3], SoNotRec::FIELD);
  node->addAuditor(initial[0].pointer, initial[0].type);
  const Auditor expanded[] = {
    initial[0], initial[2], { &tokens[3], SoNotRec::FIELD }, initial[0]
  };
  check(matches(node->getAuditors(), expanded, 4),
        "cache lost auditor types or duplicate registrations");
  node->removeAuditor(initial[0].pointer, initial[0].type);
  node->removeAuditor(initial[0].pointer, initial[0].type);
  node->removeAuditor(initial[2].pointer, initial[2].type);
  node->removeAuditor(&tokens[3], SoNotRec::FIELD);
  check(node->getAuditors().getLength() == 0,
        "clearing the tree left cached auditor pairs");
  check(node->getAuditors().getLength() == 0,
        "repeated query of cleared tree changed the cache");
  node->unref();
}

void checkLifetime()
{
  SoGroup * other = new SoGroup;
  other->ref();
  const SoAuditorList & othercache = other->getAuditors();
  for (int i = 0; i < 8; ++i) {
    SoGroup * node = new SoGroup;
    node->ref();
    int token = 0;
    const Auditor expected[] = { { &token, SoNotRec::PARENT } };
    node->addAuditor(&token, SoNotRec::PARENT);
    check(matches(node->getAuditors(), expected, 1),
          "new object inherited another object's cache");
    node->removeAuditor(&token, SoNotRec::PARENT);
    node->unref();
    check(&other->getAuditors() == &othercache && othercache.getLength() == 0,
          "destroying an object changed another object's cache");
  }
  other->unref();
}

int runTest(const char * mode)
{
  if (std::strcmp(mode, "empty") == 0) checkEmpty();
  else if (std::strcmp(mode, "refresh") == 0) checkRefresh();
  else if (std::strcmp(mode, "lifetime") == 0) checkLifetime();
  else return 2;
  return failures == 0 ? 0 : 1;
}
} // namespace

int main(int argc, char ** argv)
{
  SoDB::init();
  const int result = runTest(argc > 1 ? argv[1] : "refresh");
  SoDB::finish();
  return result;
}
