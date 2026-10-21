#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <memory>
#include <map>
#include <climits>
static int failafter=-1, arrays=0, allocations=0;
void * operator new[](std::size_t n) {
  if (failafter==0) { failafter=-1; throw std::bad_alloc(); }
  if (failafter>0) --failafter;
  void * p=std::malloc(n ? n : 1);
  if (!p) throw std::bad_alloc();
  ++arrays; ++allocations; return p;
}
void operator delete[](void * p) noexcept { if (p) { --arrays; std::free(p); } }
void operator delete[](void * p,std::size_t) noexcept { operator delete[](p); }
#include "misc/SbSmallMap.h"
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#c); return 1; } } while (0)
typedef SbSmallMap<unsigned int,int> Map;
static bool matches(const Map & m,unsigned int n) {
  if (m.getNumElements()!=n) return false;
  for (unsigned int i=0;i<n;++i) { int v=-1; if (!m.get(i,v) || v!=int(i*10)) return false; }
  return true;
}
struct Resource {
  static int live;
  Resource() { ++live; }
  ~Resource() { --live; }
};
int Resource::live;
struct BoolProxy { explicit operator bool() const noexcept { return true; } };
struct ProxyKey { unsigned int n; };
inline BoolProxy operator==(const ProxyKey &,const ProxyKey &) noexcept { return BoolProxy(); }
int main() {
  const int baseline=arrays;
  {
    Map m; const int initial=allocations;
    for (unsigned int i=0;i<4;++i) CHECK(m.put(i,int(i*10)));
    CHECK(allocations==initial); const int * old=&m.find(0)->obj;
    for (unsigned int boundary=4;boundary<=32;boundary*=2) {
      while (m.getNumElements()<boundary) { unsigned int n=m.getNumElements(); CHECK(m.put(n,int(n*10))); }
      failafter=0;
      CHECK(!m.put(0,0) && failafter==0); // Replacement never allocates.
      bool caught=false;
      try { m.put(boundary,int(boundary*10)); } catch (const std::bad_alloc &) { caught=true; }
      CHECK(caught && matches(m,boundary) && &m.find(0)->obj==old);
      failafter=0; caught=false;
      try { m[boundary]=int(boundary*10); } catch (const std::bad_alloc &) { caught=true; }
      CHECK(caught && matches(m,boundary) && &m.find(0)->obj==old);
      // The object value comes from storage that is freed during the spill.
      CHECK(m.put(boundary,m.find(0)->obj));
      CHECK(m.find(boundary)->obj==0);
      m[boundary]=int(boundary*10); old=&m.find(0)->obj;
      CHECK(matches(m,boundary+1));
    }
    const int beforearrays=arrays;
    failafter=0; bool caught=false;
    try { Map copy(m); } catch (const std::bad_alloc &) { caught=true; }
    CHECK(caught && arrays==beforearrays && matches(m,33));
    Map target; target.put(99,99); const int * prior=&target.find(99)->obj;
    failafter=0; caught=false;
    try { target=m; } catch (const std::bad_alloc &) { caught=true; }
    CHECK(caught && target.getNumElements()==1 && target.find(99)->obj==99 && &target.find(99)->obj==prior);
    CHECK(matches(m,33));
    target=m; target=target; CHECK(matches(target,33));
    Map copy(m); m[0]=999; CHECK(matches(copy,33) && matches(target,33));
    Map empty; copy=empty; CHECK(copy.getNumElements()==0);
    const int retained=allocations; copy.put(1,1); CHECK(allocations==retained);
    m.clear(); const int cleared=allocations;
    for (unsigned int i=0;i<33;++i) CHECK(m.put(i,int(i*10)));
    CHECK(allocations==cleared && matches(m,33));
    SbList<unsigned int> list; for (unsigned int i=0;i<4;++i) list.append(UINT_MAX-i);
    failafter=1; caught=false; // First list growth succeeds, second fails.
    try { m.makeKeyList(list); } catch (const std::bad_alloc &) { caught=true; }
    CHECK(caught && list.getLength()==8 && matches(m,33));
    for (unsigned int i=0;i<4;++i) CHECK(list[int(i)]==UINT_MAX-i);
    for (int i=4;i<8;++i) CHECK(m.find(list[i])!=m.const_end());
    list.truncate(4); m.makeKeyList(list); CHECK(list.getLength()==37);
  }
  CHECK(arrays==baseline);
  {
    std::unique_ptr<Resource> owner(new Resource);
    SbSmallMap<int,Resource *> m; m.put(1,owner.get());
    SbSmallMap<int,Resource *> copy(m); m.erase(1); copy.clear();
    CHECK(Resource::live==1);
    m.put(1,owner.get()); m.put(1,NULL); CHECK(Resource::live==1);
  }
  CHECK(Resource::live==0 && arrays==baseline);
  {
    Map empty; const int before=allocations; Map copied(empty);
    CHECK(copied.getNumElements()==0 && allocations==before);
    SbSmallMap<unsigned long long,int> wide;
    CHECK(wide.put(1,1) && wide.put(1ULL<<63,2));
    CHECK(wide.find(1ULL<<63)->obj==2 && wide.find(1)->obj==1);
    SbSmallMap<ProxyKey,int> proxy; ProxyKey key={1}; CHECK(proxy.put(key,7));
    CHECK(proxy.find(key)->obj==7);
  }
  Map actual; std::map<unsigned int,int> expected;
  unsigned int rng=77;
  for (int i=0;i<20000;++i) {
    rng=rng*1664525U+1013904223U; unsigned int key=(rng>>8)%257;
    switch (rng%4) {
    case 0: CHECK(actual.erase(key)==expected.erase(key)); break;
    case 1: actual[key]++; expected[key]++; break;
    default: { bool fresh=expected.count(key)==0; expected[key]=i; CHECK(bool(actual.put(key,i))==fresh); }
    }
    CHECK(actual.getNumElements()==expected.size());
    unsigned int visited=0;
    for (auto it=actual.const_begin();it!=actual.const_end();++it) {
      CHECK(expected.count(it->key)==1 && expected[it->key]==it->obj); ++visited;
    }
    CHECK(visited==expected.size());
    for (auto const & item:expected) { int v; CHECK(actual.get(item.first,v) && v==item.second); }
  }
  return 0;
}
