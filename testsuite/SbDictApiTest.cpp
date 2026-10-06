#include <Inventor/SbDict.h>
#include <Inventor/lists/SbPList.h>
#include <climits>
#include <cstdio>
#include <limits>
#include <map>
#include <stdexcept>

static_assert(sizeof(SbDict) == sizeof(void *), "SbDict must retain its opaque-pointer layout");
static_assert(alignof(SbDict) == alignof(void *), "SbDict alignment changed");
#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return false; \
} } while (false)

static unsigned int hashcalls;
static unsigned int beforethrow;
static SbDict::Key collide(SbDict::Key) { ++hashcalls; return 1; }
static SbDict::Key mixed(SbDict::Key key) { return (key ^ (key >> 17)) * static_cast<SbDict::Key>(2654435761u); }
static SbDict::Key throwing_hash(SbDict::Key key) {
  if (beforethrow-- == 0) throw std::runtime_error("hash failed");
  return key;
}

static bool mappings_and_copy()
{
  const SbDict::Key maximum = std::numeric_limits<SbDict::Key>::max();
  const SbDict::Key keys[] = { 0, 17, 34, maximum, maximum - 1,
    static_cast<SbDict::Key>(1) << (sizeof(SbDict::Key) * CHAR_BIT / 2) };
  int values[6] = { 1, 2, 3, 4, 5, 6 };
  SbDict source(1);
  source.setHashingFunction(collide);
  void * found = &values[0];
  CHECK(!source.find(99, found) && found == &values[0]);
  for (unsigned int i = 0; i < 6; ++i)
    CHECK(source.enter(keys[i], i ? &values[i] : NULL));
  CHECK(source.find(0, found) && found == NULL);
  CHECK(!source.enter(0, &values[0]));
  hashcalls = 0;
  source = source;
  CHECK(hashcalls == 0);
  CHECK(source.find(0, found) && found == &values[0] && hashcalls == 1);

  SbDict copied(source);
  SbDict assigned(1);
  assigned.setHashingFunction(collide);
  CHECK(assigned.enter(99, &assigned));
  assigned = source;
  hashcalls = 0;
  for (unsigned int i = 0; i < 6; ++i) {
    CHECK(copied.find(keys[i], found) && found == &values[i]);
    CHECK(assigned.find(keys[i], found) && found == &values[i]);
  }
  CHECK(hashcalls == 0); // Ordinary copies use the default hash.
  found = &source;
  CHECK(!assigned.find(99, found) && found == &source);
  CHECK(copied.remove(17));
  CHECK(!copied.remove(17));
  CHECK(source.find(17, found) && found == &values[1]);
  CHECK(!assigned.enter(34, NULL));
  CHECK(source.find(34, found) && found == &values[2]);
  source.clear();
  CHECK(assigned.find(34, found) && found == NULL);
  CHECK(copied.find(maximum, found) && found == &values[3]);
  SbDict emptycopy(source);
  copied = source;
  CHECK(!copied.find(maximum, found));
  CHECK(emptycopy.enter(maximum, &values[0]));
  CHECK(!source.find(maximum, found));
  CHECK(values[0] == 1 && values[5] == 6); // Values are not owned by dictionaries.
  return true;
}

struct ApplyState {
  SbDict * dictionary;
  std::map<SbDict::Key, void *> expected;
  std::map<SbDict::Key, unsigned int> seen;
  unsigned int nested;
  bool failed;
};
static ApplyState * nodata;
static void nested_callback(SbDict::Key key, void * value) {
  ++nodata->nested;
  if (nodata->expected[key] != value) nodata->failed = true;
}
static void no_data_callback(SbDict::Key key, void * value) {
  if (nodata->seen.empty()) nodata->dictionary->applyToAll(nested_callback);
  ++nodata->seen[key];
  void * found = NULL;
  if (!nodata->dictionary->find(key, found) || found != value ||
      !nodata->dictionary->remove(key)) nodata->failed = true;
}
static void data_callback(SbDict::Key key, void * value, void * closure) {
  ApplyState * state = static_cast<ApplyState *>(closure);
  ++state->seen[key];
  if (state->expected[key] != value || !state->dictionary->remove(key)) state->failed = true;
}
static bool callbacks_and_lists()
{
  int values[32];
  SbDict dictionary(1);
  dictionary.setHashingFunction(collide);
  ApplyState state = { &dictionary, {}, {}, 0, false };
  for (unsigned int i = 0; i < 32; ++i) {
    const SbDict::Key key = static_cast<SbDict::Key>(i) * 17;
    CHECK(dictionary.enter(key, i ? &values[i] : NULL));
    state.expected[key] = i ? &values[i] : NULL;
  }
  SbPList keys, mapped;
  keys.append(&dictionary); keys.append(&state);
  mapped.append(&state);
  dictionary.makePList(keys, mapped);
  CHECK(keys.getLength() == 34 && mapped.getLength() == 33);
  CHECK(keys[0] == &dictionary && keys[1] == &state && mapped[0] == &state);
  std::map<SbDict::Key, void *> observed;
  for (int i = 0; i < 32; ++i) observed[reinterpret_cast<SbDict::Key>(keys[i + 2])] = mapped[i + 1];
  CHECK(observed == state.expected);
  nodata = &state;
  const SbDict & view = dictionary;
  view.applyToAll(no_data_callback);
  CHECK(!state.failed && state.nested == 32 && state.seen.size() == 32);
  for (const auto & entry : state.seen) CHECK(entry.second == 1);
  for (const auto & entry : state.expected) {
    void * found = &state;
    CHECK(!dictionary.find(entry.first, found) && found == &state);
    CHECK(dictionary.enter(entry.first, entry.second));
  }
  state.seen.clear();
  view.applyToAll(data_callback, &state);
  CHECK(!state.failed && state.seen.size() == 32);
  for (const auto & entry : state.seen) CHECK(entry.second == 1);
  const int keylength = keys.getLength(), valuelength = mapped.getLength();
  dictionary.makePList(keys, mapped);
  CHECK(keys.getLength() == keylength && mapped.getLength() == valuelength);
  view.applyToAll(data_callback, &state);
  CHECK(state.seen.size() == 32); // Empty traversal invokes nothing.
  nodata = NULL;
  return true;
}

static bool hash_changes_and_exceptions()
{
  int values[64];
  SbDict dictionary(1);
  for (unsigned int i = 0; i < 64; ++i) CHECK(dictionary.enter(i, &values[i]));
  SbDictHashingFunc * functions[] = { collide, mixed, NULL, collide };
  for (auto function : functions) {
    dictionary.setHashingFunction(function);
    for (unsigned int i = 0; i < 64; ++i) {
      void * found = NULL;
      CHECK(dictionary.find(i, found) && found == &values[i]);
      CHECK(!dictionary.enter(i, &values[i]));
    }
  }
  beforethrow = 1;
  bool caught = false;
  try { dictionary.setHashingFunction(throwing_hash); }
  catch (const std::runtime_error &) { caught = true; }
  CHECK(caught);
  for (unsigned int i = 0; i < 64; ++i) {
    void * found = NULL;
    CHECK(dictionary.find(i, found) && found == &values[i]);
  }
  dictionary.clear();
  dictionary.setHashingFunction(throwing_hash);
  beforethrow = 100;
  CHECK(dictionary.enter(0, NULL));
  beforethrow = 0;
  // Copy does not invoke the source's custom hash.
  SbDict copied(dictionary);
  void * found = &values[0];
  CHECK(copied.find(0, found) && found == NULL);
  caught = false;
  try { dictionary.enter(999, NULL); }
  catch (const std::runtime_error &) { caught = true; }
  CHECK(caught);
  beforethrow = 100;
  CHECK(!dictionary.find(999, found));
  dictionary.setHashingFunction(NULL);
  CHECK(dictionary.find(0, found) && found == NULL);
  return true;
}

static unsigned int random_next(unsigned int & state) {
  return state = state * 1664525u + 1013904223u;
}
static bool model()
{
  int values[256];
  std::map<SbDict::Key, void *> expected;
  SbDict dictionary(1);
  unsigned int random = 0x53424449u;
  for (unsigned int operation = 0; operation < 20000; ++operation) {
    const unsigned int key = (random_next(random) >> 16) % 256;
    const unsigned int choice = (random_next(random) >> 16) % 100;
    if (operation % 1021 == 0) { dictionary.clear(); expected.clear(); }
    if (choice < 45) {
      void * value = (choice & 1) ? &values[key] : NULL;
      CHECK(dictionary.enter(key, value) == (expected.count(key) ? FALSE : TRUE));
      expected[key] = value;
    }
    else if (choice < 70) CHECK(dictionary.remove(key) == (expected.erase(key) ? TRUE : FALSE));
    else {
      void * found = &values[0];
      const auto entry = expected.find(key);
      CHECK(dictionary.find(key, found) == (entry == expected.end() ? FALSE : TRUE));
      CHECK(found == (entry == expected.end() ? &values[0] : entry->second));
    }
    if (operation % 137 == 0) {
      dictionary.setHashingFunction((operation & 1) ? collide : NULL);
      SbPList keys, mapped;
      dictionary.makePList(keys, mapped);
      CHECK(keys.getLength() == static_cast<int>(expected.size()) && mapped.getLength() == keys.getLength());
      std::map<SbDict::Key, void *> observed;
      for (int i = 0; i < keys.getLength(); ++i) observed[reinterpret_cast<SbDict::Key>(keys[i])] = mapped[i];
      CHECK(observed == expected);
    }
  }
  return true;
}

int main()
{
  if (!mappings_and_copy() || !callbacks_and_lists() ||
      !hash_changes_and_exceptions() || !model()) return 1;
  std::puts("SbDict public API, copy, callbacks, lists, hashes and model passed.");
  return 0;
}
