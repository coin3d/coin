#include <cstddef>
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
  int n;
  Value(int n = 0) : n(n) { ++live; }
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
int Value::live; int Value::remaining = -1;
struct Key {
  static bool fail;
  unsigned int n;
  Key(unsigned int n) : n(n) { }
  Key(const Key & k) : n(k.n) { if (fail) throw std::runtime_error("key"); }
  bool operator==(const Key & k) const { return n == k.n; }
};
bool Key::fail;
inline unsigned int SbHashFunc(const Key & k) noexcept { return k.n; }
#include "misc/SbHash.h"
struct Probe : SbHash<unsigned int, int> {
  Probe(unsigned int n = 5, float f = 0) : SbHash<unsigned int, int>(n,f) { }
  unsigned int count() const { return getNumBuckets(); }
  unsigned int limit() const { return getResizeThreshold(); }
};
#define CHECK(c) do { if (!(c)) return __LINE__; } while (0)
int main() {
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
