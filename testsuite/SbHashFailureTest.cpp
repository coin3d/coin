#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <climits>
#include <limits>
#include <map>
#include <stdexcept>
static bool failarray;
static int arrays;
void * operator new[](std::size_t n) {
  if (failarray) { failarray = false; throw std::bad_alloc(); }
  void * p = std::malloc(n ? n : 1);
  if (!p) throw std::bad_alloc();
  ++arrays; return p;
}
void operator delete[](void * p) noexcept { if (p) { --arrays; std::free(p); } }
void operator delete[](void * p, std::size_t) noexcept { operator delete[](p); }
void * operator new[](std::size_t n, const std::nothrow_t &) noexcept {
  try { return operator new[](n); } catch (...) { return NULL; }
}
void operator delete[](void * p, const std::nothrow_t &) noexcept { operator delete[](p); }
struct Value {
  static int live, remaining;
  static bool faildefault;
  int n;
  Value(int n = 0) : n(n) {
    if (faildefault) throw std::runtime_error("default");
    ++live;
  }
  Value(const Value & v) : n(v.n) {
    if (remaining == 0) throw std::runtime_error("copy");
    if (remaining > 0) --remaining;
    ++live;
  }
  Value & operator=(const Value & v) {
    if (remaining == 0) throw std::runtime_error("assign");
    n = v.n; return *this;
  }
  ~Value() { --live; }
};
int Value::live; int Value::remaining = -1; bool Value::faildefault;
struct Key {
  static bool fail, failcompare;
  static int remaining, live;
  unsigned int n;
  Key(unsigned int n) : n(n) { ++live; }
  Key(const Key & k) : n(k.n) {
    if (fail || remaining == 0) throw std::runtime_error("key");
    if (remaining > 0) --remaining;
    ++live;
  }
  ~Key() { --live; }
  bool operator==(const Key & k) const {
    if (failcompare) throw std::runtime_error("compare");
    return n == k.n;
  }
};
bool Key::fail; bool Key::failcompare; int Key::remaining = -1; int Key::live;
inline unsigned int SbHashFunc(const Key & k) noexcept { return k.n % 5; }
#include <Inventor/C/base/memalloc.h>
static bool failpool;
static cc_memalloc * injected_construct(unsigned int size, unsigned int alignment)
{
  if (failpool) { failpool = false; throw std::bad_alloc(); }
  return cc_memalloc_construct_aligned(size, alignment);
}
#define cc_memalloc_construct_aligned(size, alignment) injected_construct(size, alignment)
#include "misc/SbHash.h"
#undef cc_memalloc_construct_aligned
struct Probe : SbHash<unsigned int, int> {
  Probe(unsigned int n = 5, float f = 0) : SbHash<unsigned int, int>(n,f) { }
  unsigned int count() const { return getNumBuckets(); }
  unsigned int limit() const { return getResizeThreshold(); }
};
#define CHECK(c) do { if (!(c)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #c); return 1; \
} } while (0)
static int additional_exception_contracts()
{
  // A failed key copy at every prefix must release the incomplete dictionary.
  {
    SbHash<Key, int> source(5);
    for (unsigned int i = 0; i < 8; ++i) CHECK(source.put(Key(i), int(i + 10)));
    const int baselinearrays = arrays, baselinekeys = Key::live;
    for (int prefix = 0; prefix < 8; ++prefix) {
      Key::remaining = prefix;
      bool caught = false;
      try { SbHash<Key, int> copy(source); }
      catch (const std::runtime_error &) { caught = true; }
      Key::remaining = -1;
      CHECK(caught && arrays == baselinearrays && Key::live == baselinekeys &&
            source.getNumElements() == 8);
      for (unsigned int i = 0; i < 8; ++i) {
        int value = 0;
        CHECK(source.get(Key(i), value) && value == int(i + 10));
      }
    }
    SbHash<Key, int> copy(source);
    CHECK(copy.erase(Key(0)) == 1 && source.find(Key(0))->obj == 10);
  }
  // Equality may throw even though hashing is noexcept. No operation may
  // unlink a mapping or publish a new entry before equality has succeeded.
  for (int missing = 0; missing < 2; ++missing) {
    SbHash<Key, int> hash(5);
    CHECK(hash.put(Key(1), 10) && hash.put(Key(2), 20));
    const int * address = &hash.find(Key(1))->obj;
    const Key query(missing ? 6 : 1); // Key 6 collides with Key 1.
    for (int operation = 0; operation < 5; ++operation) {
      int output = -1;
      Key::failcompare = true;
      bool caught = false;
      try {
        switch (operation) {
        case 0: hash.put(query, 99); break;
        case 1: hash.get(query, output); break;
        case 2: hash.find(query); break;
        case 3: hash.erase(query); break;
        case 4: hash[query] = 99; break;
        }
      }
      catch (const std::runtime_error &) { caught = true; }
      Key::failcompare = false;
      CHECK(caught && output == -1 && hash.getNumElements() == 2);
      CHECK(&hash.find(Key(1))->obj == address && *address == 10);
      CHECK(hash.find(Key(2))->obj == 20 && hash.find(Key(6)) == hash.const_end());
    }
    CHECK(hash.put(Key(6), 60) && hash.erase(Key(1)) == 1);
  }
  {
    SbHash<unsigned int, Value> hash(5);
    Value original(10), replacement(99), output(-1);
    CHECK(hash.put(1, original) && hash.put(2, original));
    const Value * address = &hash.find(1)->obj;
    const int baseline = Value::live;
    // Existing operator[] must return the stored object without constructing
    // a default value or copying it, even when either operation would throw.
    Value::faildefault = true;
    Value::remaining = 0;
    CHECK(&hash[1] == address && hash[1].n == 10);
    bool caught = false;
    try { hash[3]; }
    catch (const std::runtime_error &) { caught = true; }
    Value::faildefault = false;
    CHECK(caught && hash.getNumElements() == 2 && Value::live == baseline);
    for (int operation = 0; operation < 2; ++operation) {
      caught = false;
      try {
        if (operation == 0) hash.put(1, replacement);
        else hash.get(1, output);
      }
      catch (const std::runtime_error &) { caught = true; }
      CHECK(caught && hash.getNumElements() == 2 && Value::live == baseline);
      CHECK(&hash.find(1)->obj == address && address->n == 10);
      CHECK(hash.find(2)->obj.n == 10 && output.n == -1);
    }
    Value::remaining = -1;
    CHECK(!hash.put(1, replacement) && hash.get(1, output) && output.n == 99);
    CHECK(hash[3].n == 0 && hash.getNumElements() == 3);
    hash.releaseStorage();
    CHECK(hash.begin() == hash.end() && Value::live == baseline - 2);
    CHECK(hash.put(4, original));
  }
  CHECK(Value::live == 0 && Key::live == 0);
  return 0;
}

int main() {
  const int additional = additional_exception_contracts();
  if (additional) return additional;
  // Pool construction must not leak staged buckets or publish partial storage.
  {
    SbHash<unsigned int, int> hash;
    const int baselinearrays = arrays;
    failpool = true;
    bool caught = false;
    try { hash.put(1, 7); }
    catch (const std::bad_alloc &) { caught = true; }
    CHECK(caught && !failpool && arrays == baselinearrays);
    CHECK(hash.getNumElements() == 0);
    CHECK(hash.put(1, 7) && hash.find(1)->obj == 7);
  }
  CHECK(Probe(0).count() == 2 && Probe(3).count() == 3);
  CHECK(Probe(4294967291U).count() == 4294967291U);
  CHECK(Probe(5, std::numeric_limits<float>::infinity()).limit() == 3);
  CHECK(Probe(5, std::numeric_limits<float>::quiet_NaN()).limit() == 3);
  CHECK(Probe(5, -1).limit() == 3 && Probe(5, 1e30f).limit() == UINT_MAX);
  Probe h;
  h.put(1,10); h.put(6,60); h.put(11,110);
  const int * address = &h.find(1)->obj;
  CHECK(!h.put(6,61) && h.count() == 5);
  failarray = true;
  CHECK(h.put(16,160) && !failarray && h.count() == 5 && h.getNumElements() == 4);
  CHECK(&h.find(1)->obj == address && h.find(16)->obj == 160);
  CHECK(h.put(21,210) && h.count() == 11 && &h.find(1)->obj == address);
  h.clear(); h.put(1,10); h.put(2,20); h.put(3,30); h.put(4,40);
  h.put(5,50); h.put(6,60); h.put(7,70); h.put(8,80);
  failarray = true; h[9] = 90;
  CHECK(!failarray && h.count() == 11 && h.find(9)->obj == 90);
  // Partial constructor failure must destroy all completed entries and storage.
  {
    SbHash<unsigned int, Value> source(5); Value v(7);
    for (unsigned int i=0; i<8; ++i) source.put(i,v);
    const int baseline = Value::live, baselinearrays = arrays;
    for (int copies=0; copies<8; ++copies) {
      Value::remaining=copies; bool caught=false;
      try { SbHash<unsigned int, Value> copy(source); }
      catch (const std::runtime_error &) { caught=true; }
      CHECK(caught && Value::live == baseline && arrays == baselinearrays);
    }
    Value::remaining=-1;
    SbHash<unsigned int, Value> target(source);
    Value::remaining=2;
    try { target=source; CHECK(false); } catch (const std::runtime_error &) { }
    CHECK(target.getNumElements()==2 && source.getNumElements()==8);
    target=target; CHECK(target.getNumElements()==2);
    Value::remaining=0;
    const unsigned int before=target.getNumElements();
    try { target.put(100,v); CHECK(false); } catch (const std::runtime_error &) { }
    CHECK(target.getNumElements()==before && target.find(100)==target.const_end());
    Value::remaining=-1; target.put(100,v); target=source;
    CHECK(target.getNumElements()==8);
  }
  CHECK(Value::live==0);
  {
    SbHash<Key,int> keys(5); Key k(1);
    Key::fail=true;
    try { keys.put(k,1); CHECK(false); } catch (const std::runtime_error &) { }
    CHECK(keys.getNumElements()==0);
    Key::fail=false; CHECK(keys.put(k,2));
  }
  {
    char a[]="same", b[]="same";
    SbHash<const char *,int> content; CHECK(content.put(a,1));
    // Hashes bytes, but equality uses pointer identity (interned strings).
    CHECK(SbHashFunc(static_cast<const char *>(a))==SbHashFunc(static_cast<const char *>(b)));
    CHECK(!content.put(a,2) && content.put(b,3) && content.getNumElements()==2);
    SbHash<char *,int> identity; CHECK(identity.put(a,1) && identity.put(b,2));
    CHECK(identity.getNumElements()==2);
    SbList<const char *> list; list.append("prefix"); content.makeKeyList(list);
    CHECK(list.getLength()==3 && list[0][0]=='p');
    SbHash<size_t,int> wide; const size_t key=static_cast<size_t>(1) << (sizeof(size_t)*8-1);
    CHECK(wide.put(key,1) && wide.put(1,2) && wide.find(key)->obj==1);
  }
  // Deterministic mixed operations compared with an independent model.
  SbHash<unsigned int,int> hash(3); std::map<unsigned int,int> model;
  unsigned int rng=71;
  for (int i=0;i<20000;++i) {
    rng=rng*1664525U+1013904223U; unsigned int key=(rng>>8)%401;
    if (rng%3) { bool fresh=model.count(key)==0; model[key]=i; CHECK(bool(hash.put(key,i))==fresh); }
    else { CHECK(hash.erase(key)==model.erase(key)); }
    CHECK(hash.getNumElements()==model.size());
    for (auto const & item:model) { int value; CHECK(hash.get(item.first,value) && value==item.second); }
  }
  SbHash<unsigned int,int> copy(hash); hash.releaseStorage();
  CHECK(copy.getNumElements()==model.size() && hash.begin()==hash.end());
  return 0;
}
