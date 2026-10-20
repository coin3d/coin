#include <Inventor/SoDB.h>
#include <Inventor/C/base/rbptree.h>
#include <cstdio>
#include <stdexcept>
struct State { cc_rbptree * tree; int * keys; int visits; bool change; };
static void count(void *, void *, void * closure) { ++static_cast<State *>(closure)->visits; }
static void removeCurrent(void * key, void *, void * closure) {
  State * s = static_cast<State *>(closure); ++s->visits;
  if (!cc_rbptree_remove(s->tree, key)) throw std::runtime_error("current entry disappeared");
}
static void mutate(void *, void *, void * closure) {
  State * s = static_cast<State *>(closure); ++s->visits;
  if (s->change) { s->change = false; cc_rbptree_remove(s->tree, &s->keys[1]);
    cc_rbptree_insert(s->tree, &s->keys[33], NULL); }
}
static void nested(void *, void *, void * closure) {
  State * s = static_cast<State *>(closure); ++s->visits;
  if (s->change) { s->change = false; State inner = {s->tree,s->keys,0,false};
    cc_rbptree_traverse(s->tree, count, &inner);
    if (inner.visits != 33) throw std::runtime_error("nested snapshot lost entries"); }
}
static void fail(void *, void *, void *) { throw std::runtime_error("callback failure"); }
static bool run() {
  int keys[34] = {}; cc_rbptree tree; cc_rbptree_init(&tree);
  for (int i=0;i<33;++i) cc_rbptree_insert(&tree,&keys[i],NULL);
  State s = {&tree,keys,0,false}; cc_rbptree_traverse(&tree,removeCurrent,&s);
  bool ok = s.visits==33 && cc_rbptree_size(&tree)==0;
  for (int i=0;i<33;++i) cc_rbptree_insert(&tree,&keys[i],NULL);
  s.visits=0; s.change=true; cc_rbptree_traverse(&tree,mutate,&s);
  ok &= s.visits==32 && cc_rbptree_size(&tree)==33;
  s.visits=0; s.change=true; cc_rbptree_traverse(&tree,nested,&s);
  ok &= s.visits==33;
  bool threw=false; try { cc_rbptree_traverse(&tree,fail,NULL); }
  catch (const std::runtime_error &) { threw=true; }
  s.visits=0; cc_rbptree_traverse(&tree,count,&s); ok &= threw && s.visits==33;
  cc_rbptree_clean(&tree);
  for (int i=0;i<33;++i) cc_rbptree_insert(&tree,NULL,i%2 ? &keys[i] : NULL);
  s.visits=0; cc_rbptree_traverse(&tree,count,&s); ok &= s.visits==33;
  cc_rbptree_clean(&tree); return ok;
}
int main() {
  SoDB::init(); bool ok=run(); SoDB::finish();
  if (!ok) std::fprintf(stderr,"allocated traversal snapshot contract failed\n");
  return ok ? 0 : 1;
}
