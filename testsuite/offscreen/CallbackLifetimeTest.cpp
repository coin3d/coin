// Linux shared-library interposition observes the public registration API without
// reusing an action or GL context after an exception. The independent action
// exception tests cover recovery; this test observes temporary callback lifetime.
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace {
struct Registration {
  SoGLRenderAction * action;
  SoGLPreRenderCB * callback;
  void * data;
};
std::vector<Registration> registrations;
int additions=0,removals=0,unmatched=0;
bool observing=false;
using Method=void (*)(SoGLRenderAction *,SoGLPreRenderCB *,void *);
Method resolve(const char * symbol) {
  Method method=reinterpret_cast<Method>(dlsym(RTLD_NEXT,symbol));
  if(!method) {std::fprintf(stderr,"cannot resolve %s: %s\n",symbol,dlerror());std::exit(1);}
  return method;
}
}

void SoGLRenderAction::addPreRenderCallback(SoGLPreRenderCB * callback,void * data) {
  static Method real=resolve("_ZN16SoGLRenderAction20addPreRenderCallbackEPFvPvPS_ES0_");
  real(this,callback,data);
  if(observing) {registrations.push_back({this,callback,data});++additions;}
}
void SoGLRenderAction::removePreRenderCallback(SoGLPreRenderCB * callback,void * data) {
  static Method real=resolve("_ZN16SoGLRenderAction23removePreRenderCallbackEPFvPvPS_ES0_");
  if(observing) {
    bool found=false;
    for(auto i=registrations.end();i!=registrations.begin();) {
      --i;
      if(i->action==this && i->callback==callback && i->data==data) {
        registrations.erase(i);found=true;break;
      }
    }
    if(!found) ++unmatched;
    ++removals;
  }
  real(this,callback,data);
}
namespace {
struct Marker {};
struct ThrowAction : SoGLRenderAction {
  explicit ThrowAction(const SbViewportRegion & vp):SoGLRenderAction(vp) {}
  bool beforeBase=false;
  void apply(SoNode * node) override {
    if(beforeBase) throw Marker();
    SoGLRenderAction::apply(node);
  }
  void apply(SoPath * path) override {
    if(beforeBase) throw Marker();
    SoGLRenderAction::apply(path);
  }
};
struct UserData {
  int calls=0;
  bool throws=false;
  SoOffscreenRenderer * renderer=nullptr;
  SoGLRenderAction * replacement=nullptr;
};
void user(void * data,SoGLRenderAction *) {
  UserData * state=static_cast<UserData *>(data);
  ++state->calls;
  if(state->replacement) state->renderer->setGLRenderAction(state->replacement);
  if(state->throws) throw Marker();
}
}
int main(int argc,char ** argv) {
  if(argc!=2) return 2;
  const bool path=std::strstr(argv[1],"path")!=nullptr;
  const bool before=std::strstr(argv[1],"before")!=nullptr;
  const bool switching=std::strstr(argv[1],"switch")!=nullptr;
  const bool during=std::strstr(argv[1],"callback")!=nullptr ||
                    std::strstr(argv[1],"throw")!=nullptr;
  if(!before && !during && !switching && std::strcmp(argv[1],"success")) return 2;
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING","1",0);
  SoDB::init();
  const SbViewportRegion vp(19,17);
  // Each invocation is a fresh process. Keep these alive through the observation
  // and exit without invoking unrelated cleanup after an interrupted traversal.
  auto * action=new ThrowAction(vp);
  action->beforeBase=before;
  UserData userdata;
  userdata.throws=during;
  observing=true;
  action->addPreRenderCallback(user,&userdata);
  auto * root=new SoSeparator;root->ref();
  auto * scenePath=new SoPath(root);scenePath->ref();
  auto * renderer=new SoOffscreenRenderer(action);
  userdata.renderer=renderer;
  if(switching) userdata.replacement=new SoGLRenderAction(vp);
  bool threw=false,rendered=false;
  try {rendered=path?renderer->render(scenePath):renderer->render(root);}
  catch(const Marker &) {threw=true;}
  catch(...) {std::fprintf(stderr,"unexpected exception\n");std::_Exit(1);}
  if(!rendered && !threw && additions==1) {
    std::fprintf(stderr,"offscreen GL context unavailable\n");std::_Exit(77);
  }
  bool ok=threw==(before||during) && additions>1 && additions==removals+1 &&
    registrations.size()==1 && registrations[0].action==action &&
    registrations[0].callback==user && registrations[0].data==&userdata && unmatched==0 && userdata.calls==(before?0:1);
  // Removing the user callback after observing the temporary registration also
  // checks that the renderer did not remove this unrelated registration.
  action->removePreRenderCallback(user,&userdata);
  ok &= registrations.empty() && additions==removals && unmatched==0;
  observing=false;
  std::fprintf(stderr,"lifetime %s: threw=%d added=%d removed=%d active=%zu unmatched=%d user=%d\n",
    argv[1],threw,additions,removals,registrations.size(),unmatched,userdata.calls);
  std::_Exit(ok?0:1);
}
