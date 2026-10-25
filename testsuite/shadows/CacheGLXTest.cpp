// Native display-list barriers, measured in the actual GL context.
#define main nestedStyleFixtureMain
#include "NestedStyleGLXTest.cpp"
#undef main
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoChildList.h>
#ifdef SHADOW_CACHE_HAVE_NODEKITS
#include <Inventor/nodekits/SoWrapperKit.h>
#endif

namespace {
class DerivedShadow : public SoShadowGroup {
  SO_NODE_HEADER(DerivedShadow);
public:
  static void initClass() { SO_NODE_INIT_CLASS(DerivedShadow,SoShadowGroup,"ShadowGroup"); }
  DerivedShadow() { SO_NODE_CONSTRUCTOR(DerivedShadow); }
protected:
  ~DerivedShadow() override {}
};
SO_NODE_SOURCE(DerivedShadow);

// Both structural APIs can run application code. A revision notification here
// must not run under the separator's cache mutex, and must not poison a cached
// negative answer. CTest bounds a possible deadlock independently of GL errors.
class ReentrantSeparator : public SoSeparator {
  SO_NODE_HEADER(ReentrantSeparator);
public:
  mutable unsigned queries;
  mutable bool armed;
  static void initClass() { SO_NODE_INIT_CLASS(ReentrantSeparator,SoSeparator,"Separator"); }
  ReentrantSeparator() : queries(0),armed(true) { SO_NODE_CONSTRUCTOR(ReentrantSeparator); }
  SoChildList * getChildren() const override {
    ++queries;
    if(armed) { armed=false; const_cast<ReentrantSeparator *>(this)->touch(); }
    return SoSeparator::getChildren();
  }
  void search(SoSearchAction * a) override {
    ++queries;
    if(armed) { armed=false; touch(); }
    SoSeparator::search(a);
  }
protected:
  ~ReentrantSeparator() override {}
};
SO_NODE_SOURCE(ReentrantSeparator);

struct CacheProbe {
  GLint output;
  int visits,maps,receivers,compiling,linked;
  bool allowList,requireAncestor;
  CacheProbe(bool plain=false) : output(0),visits(0),maps(0),receivers(0),compiling(0),linked(0),allowList(plain),requireAncestor(true) {}
  void reset() { visits=maps=receivers=compiling=linked=0; glGetIntegerv(GL_FRAMEBUFFER_BINDING,&output); }
};
void cacheProbe(void * data,SoAction * a) {
  if(!a->isOfType(SoGLRenderAction::getClassTypeId())) return;
  CacheProbe & p=*static_cast<CacheProbe *>(data); ++p.visits;
  GLint index=0,mode=0,framebuffer=0,program=0;
  glGetIntegerv(GL_LIST_INDEX,&index); glGetIntegerv(GL_LIST_MODE,&mode);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer); glGetIntegerv(GL_CURRENT_PROGRAM,&program);
  if(index) ++p.compiling;
  if(index) check(mode==GL_COMPILE,"ordinary caches retain GL_COMPILE semantics");
  const unsigned flags=SoShapeStyleElement::get(a->getState())->getFlags();
  if(!p.allowList && (framebuffer!=p.output || (flags&(SoShapeStyleElement::SHADOWMAP|SoShapeStyleElement::SHADOWS))))
    check(index==0,"real producer/receiver traversal executes outside every GL display list");
  if(framebuffer!=p.output) {
    ++p.maps;
    check((flags&SoShapeStyleElement::SHADOWMAP)!=0,"physical producer has SHADOWMAP state");
    check(program!=0,"physical producer has a VSM program immediately enabled");
  }
  else if(flags&SoShapeStyleElement::SHADOWS) {
    ++p.receivers;
    check(program!=0,"receiver shader is enabled immediately outside lists");
  }
  if(program) { GLint ok=0; glGetProgramiv(GLuint(program),GL_LINK_STATUS,&ok);
    check(ok!=0,"actual active shadow program is linked"); if(ok) ++p.linked; }
}
void ancestorProbe(void * data,SoAction * a) {
  if(!a->isOfType(SoGLRenderAction::getClassTypeId())) return;
  cacheProbe(data,a); GLint index=0; glGetIntegerv(GL_LIST_INDEX,&index);
  if(static_cast<CacheProbe *>(data)->requireAncestor)
    check(index==0,"structural shadow ancestor never opens or inherits a display list, including inactive branches");
}
void cacheMarker(SoGroup * g,CacheProbe & p,bool ancestor=false) {
  SoCallback * c=new SoCallback; c->setCallback(ancestor?ancestorProbe:cacheProbe,&p); g->addChild(c);
}
struct CacheFixture {
  SoSeparator * root,*ancestor,*inner,*geometry;
  SoShadowGroup * shadow;
  SoSwitch * sw;
  SoTranslation * move;
  SoOrthographicCamera * camera;
  SoGLRenderAction action;
  SoPath * path;
  CacheProbe probe;
  bool active;
  CacheFixture(bool spot,int mode,int depth,const std::string & variant,bool inpath=false)
    : root(new SoSeparator),ancestor(new SoSeparator),inner(NULL),geometry(new SoSeparator),
      shadow(variant=="subclass" ? new DerivedShadow : new SoShadowGroup),sw(NULL),
      move(new SoTranslation),camera(new SoOrthographicCamera),
      action(SbViewportRegion(size,size)),path(NULL),active(true) {
    root->ref(); shadow->ref(); root->renderCaching=SoSeparator::OFF;
    camera->position.setValue(0,0,-9); camera->orientation.setValue(SbVec3f(0,1,0),float(std::acos(-1.0)));
    camera->height=8; camera->nearDistance=1; camera->farDistance=40; root->addChild(camera);
    SoEnvironment * env=new SoEnvironment; env->ambientIntensity=0; env->attenuation.setValue(0,0,1); root->addChild(env);
    SoMaterial * material=new SoMaterial; material->ambientColor.setValue(0,0,0); material->diffuseColor.setValue(1,1,1);
    material->specularColor.setValue(0,0,0); root->addChild(material);
    SoNormal * n=new SoNormal; n->vector.setValue(0,0,1); root->addChild(n);
    SoNormalBinding * nb=new SoNormalBinding; nb->value=SoNormalBinding::OVERALL; root->addChild(nb);
    root->addChild(ancestor); ancestor->renderCaching=mode; cacheMarker(ancestor,probe,true);
    SoGroup * parent=ancestor;
    for(int i=1;i<depth;++i) { inner=new SoSeparator; inner->renderCaching=mode; parent->addChild(inner); cacheMarker(inner,probe,true); parent=inner; }
    if(variant=="reentrant") {
      ReentrantSeparator * r=new ReentrantSeparator; r->renderCaching=mode; parent->addChild(r); parent=r;
    }
    if(variant=="switch") { sw=new SoSwitch; sw->whichChild=SO_SWITCH_NONE; parent->addChild(sw); parent=sw; active=false; }
#ifdef SHADOW_CACHE_HAVE_NODEKITS
    if(variant=="nodekit") {
      check(!SoBaseKit::isSearchingChildren(),"nodekit test starts with default hidden search children");
      SoWrapperKit * kit=new SoWrapperKit; parent->addChild(kit);
      check(kit->setPart("contents",shadow),"nodekit accepts ShadowGroup as hidden contents");
      check(!SoBaseKit::isSearchingChildren(),"constructing hidden shadow leaves search semantics unchanged");
    } else
#endif
      parent->addChild(shadow);
    groupSettings(shadow,.5f); shadow->renderCaching=mode;
    if(variant=="inactive") { shadow->isActive=FALSE; active=false; }
    lamp(shadow,spot,NULL,.7f);
    geometry->renderCaching=mode; shadow->addChild(geometry);
    // Probe the separator's body, so a descendant cache opened during either
    // pass cannot disappear behind a later successful ancestor traversal.
    cacheMarker(geometry,probe);
    SoSeparator * caster=new SoSeparator; caster->renderCaching=mode;
    caster->addChild(move); cacheMarker(caster,probe); rectangle(caster,-2.05f,-.7f,-.95f,.7f,1);
    geometry->addChild(caster);
    // The same caster subtree participates in a DAG; the second instance is
    // translated well away from the primary pixel samples.
    SoSeparator * instance=new SoSeparator; instance->renderCaching=mode;
    SoTranslation * offset=new SoTranslation; offset->translation.setValue(0,2.1f,0); instance->addChild(offset);
    instance->addChild(caster); geometry->addChild(instance);
    grid(geometry,-3.8f,3.8f);
    if(variant=="nested") {
      SoShadowGroup * child=new SoShadowGroup; groupSettings(child,.5f); child->renderCaching=mode;
      lamp(child,spot,plainCaster(1.5f),.1f); geometry->addChild(child);
      SoSeparator * leaf=new SoSeparator; leaf->renderCaching=mode; cacheMarker(leaf,probe); child->addChild(leaf);
      rectangle(leaf,2.8f,2.8f,3.1f,3.1f,0);
    }
    static int context=6300; action.setCacheContext(++context);
    if(inpath) { path=new SoPath; path->ref(); path->setHead(root); path->append(root->findChild(ancestor));
      if(inner) path->append(ancestor->findChild(inner));
      path->append((inner?inner:ancestor)->findChild(shadow)); path->append(shadow->findChild(geometry)); }
  }
  ~CacheFixture() { if(path) path->unref(); root->unref(); shadow->unref(); }
  Result frame(bool enabled=true) {
    probe.reset(); Result r;
    SoErrorCB * old=SoDebugError::getHandlerCallback(); void * olddata=SoDebugError::getHandlerData();
    SoDebugError::setHandlerCallback(error,&r);
    glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); glDepthFunc(GL_LEQUAL);
    glViewport(0,0,size,size); glClearColor(0,0,0,0); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(path) action.apply(path); else action.apply(root);
    glFinish(); glPixelStorei(GL_PACK_ALIGNMENT,1); glReadPixels(0,0,size,size,GL_RGB,GL_UNSIGNED_BYTE,&r.image[0]);
    SoDebugError::setHandlerCallback(old,olddata);
    check(r.errors==0,"no native shader errors"); check(glGetError()==GL_NO_ERROR,"GL render/readback has no error");
    if(probe.requireAncestor) check(probe.visits>0,"ancestor CPU callback still executes on every frame");
    if(enabled) { check(probe.receivers>0,"receiver CPU traversal executes on every active frame");
      check(probe.linked>0,"active frame observes an actually linked shader"); }
    return r;
  }
};
void samePixels(const Result & a,const Result & b,const char * reason) {
  // Compare stable interior controls and the two possible caster footprints;
  // retain tolerance for map quantization without hiding a lost shadow.
  const float x[]={1.5f,.3f,2.9f,-2.9f};
  for(unsigned i=0;i<4;++i) check(std::fabs(a.patch(x[i],i>1?-1.7f:0)-b.patch(x[i],i>1?-1.7f:0))<14,reason);
}
void exercise(bool spot,int mode,int depth,const std::string & variant,bool path=false,int numcaches=2) {
  label=std::string(spot?"spot":"directional")+" cache="+std::to_string(mode)+" depth="+std::to_string(depth)+" "+variant+(path?" path":"");
  SoSeparator::setNumRenderCaches(numcaches);
  CacheFixture reference(spot,SoSeparator::OFF,depth,variant,path),candidate(spot,mode,depth,variant,path);
  for(int phase=0;phase<6;++phase) {
    if(phase==1) { reference.move->translation.setValue(1.2f,0,0); candidate.move->translation.setValue(1.2f,0,0); }
    if(phase==2) { reference.camera->position.setValue(.25f,0,-9); candidate.camera->position.setValue(.25f,0,-9); }
    if(phase==3) {
      reference.move->translation.setValue(0,0,0); candidate.move->translation.setValue(0,0,0);
      reference.camera->position.setValue(0,0,-9); candidate.camera->position.setValue(0,0,-9);
      if(reference.sw) { reference.sw->whichChild=SO_SWITCH_ALL; candidate.sw->whichChild=SO_SWITCH_ALL; }
      reference.shadow->isActive=TRUE; candidate.shadow->isActive=TRUE;
      reference.active=candidate.active=true;
    }
    if(phase==4 && !path) {
      // Mutate only the ancestor structure: direct ShadowGroup child mutation
      // has a separate ownership contract. Removal must also restore caching.
      reference.ancestor->removeChild(reference.ancestor->getNumChildren()-1);
      candidate.ancestor->removeChild(candidate.ancestor->getNumChildren()-1);
      reference.active=candidate.active=false;
      reference.probe.requireAncestor=candidate.probe.requireAncestor=false;
    }
    if(phase==5 && !path) {
      reference.ancestor->addChild(reference.shadow); candidate.ancestor->addChild(candidate.shadow);
      reference.active=candidate.active=true;
      reference.probe.requireAncestor=candidate.probe.requireAncestor=true;
    }
    for(int frame=0;frame<10;++frame) {
      const Result a=reference.frame(reference.active),b=candidate.frame(candidate.active);
      samePixels(a,b,"cached input matches OFF reference including first compilation and replay frames");
      if(phase==0 && frame==0 && reference.active) check(reference.probe.maps>0,"cold reference proves physical map traversal");
      if(reference.active && phase==0) {
        check(a.patch(2.9f,-1.7f)>60,"lit control proves receiver and light");
        check(a.patch(1.5f)+35<a.patch(2.9f,-1.7f),"reference has a physically visible caster shadow");
      }
    }
  }
#ifdef SHADOW_CACHE_HAVE_NODEKITS
  check(!SoBaseKit::isSearchingChildren(),"rendering never changes global nodekit search semantics");
#endif
}
void plainCache(int mode) {
  label="plain cache="+std::to_string(mode); SoSeparator::setNumRenderCaches(2);
  SoSeparator * root=new SoSeparator; root->ref(); root->renderCaching=SoSeparator::OFF;
  SoOrthographicCamera * c=new SoOrthographicCamera; c->height=8; c->position.setValue(0,0,9); root->addChild(c);
  SoLightModel * lm=new SoLightModel; lm->model=SoLightModel::BASE_COLOR; root->addChild(lm);
  SoMaterial * m=new SoMaterial; m->diffuseColor.setValue(1,1,1); root->addChild(m);
  SoSeparator * s=new SoSeparator; s->renderCaching=mode; root->addChild(s);
  ReentrantSeparator * ordinary=new ReentrantSeparator; ordinary->renderCaching=SoSeparator::OFF; ordinary->armed=false; s->addChild(ordinary);
  CacheProbe p(true); cacheMarker(ordinary,p);
  for(int i=0;i<20;++i) { SoCube * cube=new SoCube; ordinary->addChild(cube); }
  SoGLRenderAction a(SbViewportRegion(size,size)); a.setCacheContext(6400+mode);
  int compiled=0,skipped=0; unsigned stableQueries=0;
  for(int frame=0;frame<24;++frame) {
    p.reset(); glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); a.apply(root); glFinish();
    unsigned char pixel[3]={}; glReadPixels(size/2,size/2,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
    check(pixel[0]>200,"plain cached geometry keeps physical pixels");
    compiled+=p.compiling; if(frame>12 && p.visits==0) ++skipped;
    if(frame==12) stableQueries=ordinary->queries;
  }
  check(compiled>0,"ordinary subtree positively opens an actual GL_COMPILE display list");
  check(skipped>0,"ordinary subtree replays cache without CPU callback");
  check(ordinary->queries-stableQueries<5,"stable ordinary subtree avoids a full structural query every frame");
  root->unref();
}
}
int main(int argc,char ** argv) {
  Display * display=XOpenDisplay(NULL); if(!display) return 77;
  int attrs[]={GLX_RGBA,GLX_DEPTH_SIZE,24,None}; XVisualInfo * visual=glXChooseVisual(display,DefaultScreen(display),attrs); if(!visual) return 77;
  GLXContext context=glXCreateContext(display,visual,NULL,True); if(!context) return 77;
  Colormap colormap=XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
  XSetWindowAttributes attr={}; attr.colormap=colormap;
  Window window=XCreateWindow(display,RootWindow(display,visual->screen),0,0,size,size,0,visual->depth,InputOutput,visual->visual,CWColormap,&attr);
  if(!glXMakeCurrent(display,window,context)) return 77;
  SoDB::init(); DerivedShadow::initClass(); ReentrantSeparator::initClass();
  if(!glGetString(GL_SHADING_LANGUAGE_VERSION)||!SoShadowGroup::isSupported()) return 77;
  const std::string mode=argc>1?argv[1]:"barriers";
  if(mode=="plain") { plainCache(SoSeparator::ON); plainCache(SoSeparator::AUTO); }
  else if(mode=="barriers") {
    for(int spot=0;spot<2;++spot) for(int caching=SoSeparator::ON;caching<=SoSeparator::AUTO;++caching)
      for(int depth=1;depth<=2;++depth) exercise(spot,caching,depth,"normal");
    exercise(false,SoSeparator::ON,2,"normal",true);
    exercise(true,SoSeparator::AUTO,1,"normal",false,0);
    exercise(false,SoSeparator::ON,2,"nested");
  }
  else if(mode=="structure") {
    const char * variants[]={"switch","inactive","subclass","reentrant"
#ifdef SHADOW_CACHE_HAVE_NODEKITS
      ,"nodekit"
#endif
    };
    for(unsigned i=0;i<sizeof(variants)/sizeof(variants[0]);++i)
      for(int caching=SoSeparator::ON;caching<=SoSeparator::AUTO;++caching)
        exercise(false,caching,2,variants[i]);
  }
  else { std::fprintf(stderr,"unknown cache test mode\n"); return 2; }
  SoDB::finish(); glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap); XFree(visual); XCloseDisplay(display);
  std::printf("Native shadow cache checks: %d failure(s)\n",failures); return failures?1:0;
}
