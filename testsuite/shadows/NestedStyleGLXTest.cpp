// Observe the actual RTT action and linked programs through public callbacks.
#define COIN_SHADOW_GLX_FIXTURE_ONLY
#include "DirectionalGLXTest.cpp"
#include <Inventor/SoPath.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>
#include <map>

namespace {
struct Observation;
struct Probe { Observation * owner; int kind; int expectedStyle; };
struct Observation {
  SoGLRenderAction * visible;
  typedef std::pair<SoAction *, GLint> PassKey;
  std::map<PassKey, GLint> moments;
  GLint outputFramebuffer;
  int visits, maps, leaves, visibleLeaves, styleVisits;
  GLint visibleProgram;
  unsigned visibleFlags;
  Observation() : visible(NULL), outputFramebuffer(0) { reset(); }
  void reset() { moments.clear(); visits=maps=leaves=visibleLeaves=styleVisits=0; visibleProgram=0; visibleFlags=0; }
};
void observe(void * data, SoAction * action) {
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  Probe & p=*static_cast<Probe *>(data); Observation & o=*p.owner;
  // Stop recursive production with a diagnostic before an unbounded stack or
  // driver allocation failure can become the only regression signal.
  if (++o.visits>256) {
    std::fprintf(stderr,"FAIL [%s]: more than 256 actual producer/visible callbacks; nested map production is recursive\n",label.c_str());
    std::exit(1);
  }
  // SoSceneTexture2 can reuse the visible action while switching its
  // traversal node. Identify the producer by its physical render target;
  // SHADOWMAP itself is the behavior under test, never the pass oracle.
  GLint framebuffer=0; glGetIntegerv(GL_FRAMEBUFFER_BINDING,&framebuffer);
  const bool map=framebuffer!=o.outputFramebuffer;
  const Observation::PassKey pass(action,framebuffer);
  if (p.kind==4 && map)
    SoShapeStyleElement::setShadowsRendering(action->getState(),TRUE);
  const unsigned flags=SoShapeStyleElement::get(action->getState())->getFlags();
  if (p.kind==2)
    check(SoShadowStyleElement::get(action->getState())==p.expectedStyle,
          "style node updates its element in visible and map traversal");
  GLint program=0; glGetIntegerv(GL_CURRENT_PROGRAM,&program);
  if (map) {
    ++o.maps;
    check((flags & SoShapeStyleElement::SHADOWMAP)!=0,"actual RTT traversal carries SHADOWMAP");
    if (p.kind==0) o.moments[pass]=program;
    if (p.kind==1 || p.kind==2 || p.kind==5) {
      ++o.leaves;
      check(program!=0,"map geometry keeps an enabled VSM GLSL program");
      const std::map<Observation::PassKey, GLint>::const_iterator it=o.moments.find(pass);
      if (it!=o.moments.end())
        check(program==it->second,"nested groups/styles preserve the ancestor's actual VSM program");
    }
    if (p.kind==2) {
      ++o.styleVisits;
      check(SoShadowStyleElement::get(action->getState())==p.expectedStyle,
            "map traversal always updates the public style element");
    }
  }
  else {
    check(!(flags & SoShapeStyleElement::SHADOWMAP),"visible action never leaks SHADOWMAP");
    if (p.kind==0) { o.visibleProgram=program; o.visibleFlags=flags; }
    if (p.kind==5 && o.visibleProgram && p.owner->visible==action) {
      check(program==o.visibleProgram,"child restores the visible parent shader program");
      check((flags & SoShapeStyleElement::SHADOWS)==(o.visibleFlags & SoShapeStyleElement::SHADOWS),
            "child restores parent SHADOWS state");
    }
    if (p.kind==1) {
      ++o.visibleLeaves;
      const bool receives=(SoShadowStyleElement::get(action->getState()) &
                           SoShadowStyleElement::SHADOWED)!=0;
      check(receives ? program!=0 : program==0,
            "visible receiver program follows SHADOWED independently of casting");
    }
    if (p.kind==3) {
      check(program==0,"program is restored after the outer shadow group");
      check(!(flags & SoShapeStyleElement::SHADOWS),"SHADOWS is restored after the outer group");
    }
  }
  if (program) {
    GLint linked=0; glGetProgramiv(GLuint(program),GL_LINK_STATUS,&linked);
    check(linked!=0,"observed GL program is actually linked");
    GLuint shaders[8]; GLsizei n=0; glGetAttachedShaders(GLuint(program),8,&n,shaders);
    check(n>=2,"linked VSM/receiver has both shader stages");
    for (int i=0;i<n;++i) { GLint compiled=0; glGetShaderiv(shaders[i],GL_COMPILE_STATUS,&compiled);
      check(compiled!=0,"observed attached shader is actually compiled"); }
  }
}
void marker(SoGroup * g, Probe * p) { SoCallback * c=new SoCallback; c->setCallback(observe,p); g->addChild(c); }
void rectangle(SoGroup * g,float x0,float y0,float x1,float y1,float z) {
  SoCoordinate3 * c=new SoCoordinate3;
  const SbVec3f p[]={SbVec3f(x0,y0,z),SbVec3f(x1,y0,z),SbVec3f(x1,y1,z),SbVec3f(x0,y1,z)};
  c->point.setValues(0,4,p); g->addChild(c);
  SoFaceSet * f=new SoFaceSet; f->numVertices=4; g->addChild(f);
}
void grid(SoGroup * g,float x0,float x1) {
  // Vertex-lit spots require vertices inside the cone, including lit controls.
  for (int y=0;y<16;++y) for(int x=0;x<12;++x)
    rectangle(g,x0+(x1-x0)*x/12,-3.8f+7.6f*y/16,
              x0+(x1-x0)*(x+1)/12,-3.8f+7.6f*(y+1)/16,0);
}
SoSeparator * plainCaster(float x) {
  SoSeparator * g=new SoSeparator; g->renderCaching=SoSeparator::OFF;
  rectangle(g,x-.55f,-.7f,x+.55f,.7f,1); return g;
}
void lamp(SoShadowGroup * g,bool spot,SoNode * scene,float intensity) {
  if (spot) {
    SoShadowSpotLight * l=new SoShadowSpotLight;
    l->location.setValue(0,0,6); l->direction.setValue(0,0,-1);
    l->cutOffAngle=.75f; l->dropOffRate=0; l->intensity=intensity;
    l->nearDistance=.1f; l->farDistance=12;
    if(scene) l->shadowMapScene=scene; g->addChild(l);
  }
  else {
    SoShadowDirectionalLight * l=new SoShadowDirectionalLight;
    l->direction.setValue(0,0,-1); l->intensity=intensity;
    l->bboxCenter.setValue(0,0,0); l->bboxSize.setValue(9,9,6);
    if(scene) l->shadowMapScene=scene; g->addChild(l);
  }
}
void groupSettings(SoShadowGroup * g,float q) {
  g->renderCaching=SoSeparator::OFF; g->quality=q; g->precision=.2f;
  g->epsilon=.00001f; g->threshold=.1f;
}
struct NestedFixture {
  SoSeparator * root;
  SoShadowGroup * outer,*child;
  SoShadowStyle * casterStyle,*receiverStyle;
  SoGLRenderAction action;
  Observation observed;
  Probe start,leaf,styles[4],outside[4],forceShadows,afterChild,afterGroup;
  Result result;
  bool custom, active;
  SoPath * path;
  NestedFixture(bool spot,bool customScene,float q,int depth,bool enabled,bool wrapper,bool inpath)
    : root(new SoSeparator),outer(new SoShadowGroup),child(NULL),
      casterStyle(new SoShadowStyle),receiverStyle(new SoShadowStyle),
      action(SbViewportRegion(size,size)),custom(customScene),active(enabled),path(NULL) {
    root->ref(); root->renderCaching=SoSeparator::OFF;
    observed.visible=&action;
    start={&observed,0,3}; leaf={&observed,1,3};
    afterChild={&observed,5,3}; afterGroup={&observed,3,3};
    forceShadows={&observed,4,3};
    SoOrthographicCamera * camera=new SoOrthographicCamera;
    camera->position.setValue(0,0,-9); camera->orientation.setValue(SbVec3f(0,1,0),float(std::acos(-1.0)));
    camera->height=8; camera->nearDistance=1; camera->farDistance=40; root->addChild(camera);
    SoEnvironment * env=new SoEnvironment; env->ambientIntensity=0; env->attenuation.setValue(0,0,1); root->addChild(env);
    SoMaterial * material=new SoMaterial; material->ambientColor.setValue(0,0,0);
    material->diffuseColor.setValue(1,1,1); material->specularColor.setValue(0,0,0); root->addChild(material);
    SoNormal * n=new SoNormal; n->vector.setValue(0,0,1); root->addChild(n);
    SoNormalBinding * nb=new SoNormalBinding; nb->value=SoNormalBinding::OVERALL; root->addChild(nb);
    for(int i=0;i<4;++i) {
      SoSeparator * s=new SoSeparator; s->renderCaching=SoSeparator::OFF; root->addChild(s);
      SoShadowStyle * style=new SoShadowStyle; style->style=i; s->addChild(style);
      outside[i]={&observed,2,i}; marker(s,&outside[i]);
    }
    groupSettings(outer,q); root->addChild(outer);
    SoSeparator * producer=new SoSeparator; producer->renderCaching=SoSeparator::OFF;
    marker(producer,&start);
    SoGroup * parent=producer;
    if(wrapper) { SoSeparator * s=new SoSeparator; s->renderCaching=SoSeparator::OFF; parent->addChild(s); parent=s; }
    for(int i=0;i<depth;++i) {
      SoShadowGroup * g=new SoShadowGroup; groupSettings(g,q); g->isActive=enabled;
      parent->addChild(g); parent=g; child=g;
      // A disjoint explicit scene prevents scene-graph cycles even on baseline.
      // It also lets the visible child retain its independent receiver behavior.
      lamp(g,spot,plainCaster(1.5f),.35f);
    }
    // Exercise all four styles in the real moments pass, without tying shader
    // validity to the style used for the physical caster below.
    for(int i=0;i<4;++i) {
      SoSeparator * s=new SoSeparator; s->renderCaching=SoSeparator::OFF; parent->addChild(s);
      // The public flags can coexist during ancestor traversal. Reception
      // must never switch off moments production, even in that state.
      marker(s,&forceShadows);
      SoShadowStyle * style=new SoShadowStyle; style->style=i; s->addChild(style);
      styles[i]={&observed,2,i}; marker(s,&styles[i]);
    }
    SoSeparator * caster=new SoSeparator; caster->renderCaching=SoSeparator::OFF; parent->addChild(caster);
    caster->addChild(casterStyle); marker(caster,&leaf);
    rectangle(caster,-2.05f,-.7f,-.95f,.7f,1);
    // A receiver within the child tests its own visible shadow shader. It is
    // also in custom map scenes, where it must stay an ordinary depth caster.
    parent->addChild(receiverStyle); grid(parent,.3f,3.8f);
    marker(producer,&afterChild);
    // Independent sibling caster survives child style/state changes.
    producer->addChild(plainCaster(-1.5f));
    // Move that sibling's independent footprint away from the tested caster.
    // y translation is scoped so it cannot move the receiver.
    SoSeparator * sibling=static_cast<SoSeparator *>(producer->getChild(producer->getNumChildren()-1));
    SoTranslation * t=new SoTranslation; t->translation.setValue(0,2.1f,0); sibling->insertChild(t,0);
    lamp(outer,spot,custom ? producer : NULL,.35f);
    if(!custom) outer->addChild(producer);
    // Custom scene is held by the light; normal scene by the outer group.
    grid(outer,-3.8f,.2f);
    marker(root,&afterGroup);
    action.setCacheContext(5391);
    if(inpath) { path=new SoPath; path->ref(); path->setHead(root);
      path->append(root->findChild(outer)); path->append(outer->getNumChildren()-1); }
  }
  ~NestedFixture() { if(path) path->unref(); root->unref(); }
  Result frame() {
    observed.reset(); result=Result();
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&observed.outputFramebuffer);
    SoErrorCB * old=SoDebugError::getHandlerCallback(); void * data=SoDebugError::getHandlerData();
    SoDebugError::setHandlerCallback(error,&result);
    glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); glDepthFunc(GL_LEQUAL);
    glViewport(0,0,size,size); glClearColor(0,0,0,0); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(path) action.apply(path); else action.apply(root);
    glFinish(); glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(0,0,size,size,GL_RGB,GL_UNSIGNED_BYTE,&result.image[0]);
    SoDebugError::setHandlerCallback(old,data);
    check(result.errors==0,"no compilation/link errors");
    check(observed.maps>0 && observed.leaves>0 && observed.styleVisits>0,
          "actual map pass visits nested geometry and every style probe");
    check(glGetError()==GL_NO_ERROR,"GL rendering/readback succeeds");
    return result;
  }
  double left(const Result & r) { return r.patch(1.5f); } // below camera reverses X
  double sibling(const Result & r) { return r.patch(1.5f,2.3f); }
  double lit(const Result & r) { return r.patch(2.9f,-1.7f); }
};
void nestedCase(bool spot,bool custom,float q,int depth,bool active,bool wrapper,bool path) {
  label=std::string(spot?"spot":"directional")+(custom?" custom":" normal")+
    " q="+std::to_string(q)+" depth="+std::to_string(depth)+
    (active?" active":" inactive")+(wrapper?" wrapper":"")+(path?" path":"");
  NestedFixture f(spot,custom,q,depth,active,wrapper,path);
  double casting[4]={};
  for(int style=0;style<4;++style) {
    f.casterStyle->style=style;
    const Result cold=f.frame(); const Result warm=f.frame();
    casting[style]=f.left(warm);
    check(f.lit(warm)>60,"unoccluded receiver proves geometry and lighting");
    check(std::fabs(f.left(cold)-f.left(warm))<14,"cold/warm frames retain caster result");
    check(f.sibling(warm)+35<f.lit(warm),"sibling caster keeps its ancestor-map shadow");
    if(style&1) check(f.left(warm)+35<f.lit(warm),"CASTS_SHADOW keeps nested caster in ancestor map");
    else check(f.left(warm)>f.lit(warm)-16,"without CASTS_SHADOW nested caster is omitted");
  }
  check(casting[0]>casting[1]+35 && casting[2]>casting[3]+35,
        "casting depends on CASTS_SHADOW independently of SHADOWED");
  f.casterStyle->style=SoShadowStyle::CASTS_SHADOW_AND_SHADOWED;
  const Result restored=f.frame();
  check(std::fabs(f.left(restored)-casting[3])<14,"notified style mutation restores original casting");
  // Normal visible traversal must still render the active child's receiver.
  if(!custom && active && depth==1) {
    f.receiverStyle->style=SoShadowStyle::NO_SHADOWING; const Result unshadowed=f.frame();
    f.receiverStyle->style=SoShadowStyle::SHADOWED; const Result received=f.frame();
    check(unshadowed.patch(-1.5f)>received.patch(-1.5f)+35,
          "visible active child retains its own shadow reception after ancestor production");
    check(f.observed.visibleLeaves>0,"visible child is traversed with its own shader");
  }
  std::printf("[%s] styles=(%.1f %.1f %.1f %.1f) map callbacks=%d\n",label.c_str(),
              casting[0],casting[1],casting[2],casting[3],f.observed.maps);
}
}
int main(int argc,char ** argv) {
  Display * display=XOpenDisplay(NULL); if(!display) return 77;
  int attributes[]={GLX_RGBA,GLX_DEPTH_SIZE,24,None};
  XVisualInfo * visual=glXChooseVisual(display,DefaultScreen(display),attributes); if(!visual) return 77;
  GLXContext context=glXCreateContext(display,visual,NULL,True); if(!context) return 77;
  Colormap colormap=XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
  XSetWindowAttributes attr={}; attr.colormap=colormap;
  Window window=XCreateWindow(display,RootWindow(display,visual->screen),0,0,size,size,0,
    visual->depth,InputOutput,visual->visual,CWColormap,&attr);
  if(!glXMakeCurrent(display,window,context)) return 77;
  SoDB::init(); SoSeparator::setNumRenderCaches(0);
  if(!glGetString(GL_SHADING_LANGUAGE_VERSION)||!SoShadowGroup::isSupported()) return 77;
  const std::string mode=argc>1?argv[1]:"all";
  for(int spot=0;spot<2;++spot) for(int custom=0;custom<2;++custom) {
    const std::string name=std::string(spot?"spot":"directional")+(custom?"-custom":"-normal");
    if(mode!="all" && mode!=name) continue;
    nestedCase(spot,custom,.5f,0,true,true,false); // equivalent plain Separator control
    for(int q=0;q<3;++q) {
      nestedCase(spot,custom,q*.5f,1,true,false,false);
      nestedCase(spot,custom,q*.5f,2,true,true,false);
      nestedCase(spot,custom,q*.5f,1,false,true,false);
    }
    nestedCase(spot,custom,.5f,1,true,true,true);
  }
  SoDB::finish(); glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap); XFree(visual); XCloseDisplay(display);
  std::printf("Native nested/style shadow checks: %d failure(s)\n",failures);
  return failures?1:0;
}
