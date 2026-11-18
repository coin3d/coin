// Inspect the legacy traversal and read back actual GL buffers, without a shape fallback.
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoGLVBOElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include "rendering/SoVBO.h"
#include <cstdio>
#include <cstring>
#include <functional>
#include <list>
#include <string>
#include <vector>
namespace {
const uint32_t contextid=952;
int checks=0, failures=0;
std::string label;
void check(bool ok,const char * msg) { ++checks; if (!ok) { ++failures; std::fprintf(stderr,"FAIL %s: %s\n",label.c_str(),msg); } }
std::vector<SbVec3f> values(int n,float offset=0) {
  std::vector<SbVec3f> v; for(int i=0;i<n;++i) v.push_back(SbVec3f(offset+.125f+i,offset+.375f-i,offset+.875f+2*i)); return v;
}
void assign(SoTextureCoordinate3 * t,const std::vector<SbVec3f> & v) { t->point.setNum(int(v.size())); if(!v.empty()) t->point.setValues(0,int(v.size()),&v[0]); }
SoVBO * slot(SoState * s,int u) { return SoGLVBOElement::getInstance(s)->getTexCoordVBO(u); }
void bytes(SoVBO * v,const std::vector<SbVec3f> & expected) {
  check(v!=NULL,"VBO present (no CPU fallback)"); if(!v) return;
  const GLvoid * data=NULL; intptr_t size=-1; v->getBufferData(data,size);
  intptr_t want=intptr_t(expected.size()*sizeof(SbVec3f)); check(size==want,"VBO CPU byte count");
  if(size!=want || !want) return;
  check(data && std::memcmp(data,&expected[0],size_t(want))==0,"VBO CPU bytes");
  v->bindBuffer(contextid); GLint name=0,gpuSize=-1; glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&name);
  check(name!=0,"actual GL buffer bound"); glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&gpuSize);
  check(gpuSize==want,"actual GL byte count");
  if(gpuSize==want) { std::vector<SbVec3f> actual(expected.size()); glGetBufferSubData(GL_ARRAY_BUFFER,0,want,&actual[0]); check(std::memcmp(&actual[0],&expected[0],size_t(want))==0,"actual GL bytes"); }
  glBindBuffer(GL_ARRAY_BUFFER,0);
}
void cpu(SoState * s,int u,const std::vector<SbVec3f> & expected) {
  const SoMultiTextureCoordinateElement * e=SoMultiTextureCoordinateElement::getInstance(s);
  check(e->getNum(u)==int(expected.size()),"CPU coordinate count");
  if(!expected.empty()) {
    check(e->getType(u)==SoMultiTextureCoordinateElement::EXPLICIT,"explicit array replaces texgen"); check(e->getDimension(u)==3,"dimension three");
    if(e->getNum(u)==int(expected.size())) for(size_t i=0;i<expected.size();++i) check(e->get3(u,int(i))==expected[i],"CPU UVW values");
  }
}
struct Scene {
  SoGroup * root; SoGLRenderAction action; SoVBO vertex,other,previous;
  std::vector<SbVec3f> xyz,uvOther;
  std::list<std::function<void(SoState *)> > callbacks;
  Scene():root(new SoGroup),action(SbViewportRegion(16,16)),xyz(values(5,100)),uvOther(values(4,20)) {
    root->ref(); action.setCacheContext(contextid);
    vertex.setBufferData(&xyz[0],xyz.size()*sizeof(SbVec3f),100);
    other.setBufferData(&uvOther[0],uvOther.size()*sizeof(SbVec3f),101);
    previous.setBufferData(&uvOther[0],uvOther.size()*sizeof(SbVec3f),102);
  }
  ~Scene() { root->unref(); }
  static void dispatch(void * p,SoAction * a) { if(a->isOfType(SoGLRenderAction::getClassTypeId())) (*static_cast<std::function<void(SoState *)> *>(p))(a->getState()); }
  void call(SoGroup * g,std::function<void(SoState *)> fn) { callbacks.push_back(fn); SoCallback * c=new SoCallback; c->setCallback(dispatch,&callbacks.back()); g->addChild(c); }
  void unit(SoGroup * g,int u) { SoTextureUnit * t=new SoTextureUnit; t->unit=u; g->addChild(t); }
  void seed(int target,int preserved) {
    call(root,[=](SoState * s) { SoGLVBOElement::setVertexVBO(s,&vertex); SoMultiTextureCoordinateElement::set3(s,root,preserved,int(uvOther.size()),&uvOther[0]); SoGLVBOElement::setTexCoordVBO(s,preserved,&other); SoGLVBOElement::setTexCoordVBO(s,target,&previous); });
  }
  void preserved(SoState * s,int u) {
    check(SoGLVBOElement::getInstance(s)->getVertexVBO()==&vertex,"position VBO sentinel preserved"); check(slot(s,u)==&other,"other unit VBO sentinel preserved"); cpu(s,u,uvOther); bytes(&vertex,xyz); bytes(&other,uvOther);
  }
  void apply() { action.apply(root); check(glGetError()==GL_NO_ERROR,"no invalid GL commands"); }
};
void lifecycle(int target,int otherunit,bool disabled) {
  Scene s; s.seed(target,otherunit); s.unit(s.root,target); s.root->addChild(new SoTextureCoordinateObject);
  SoTextureCoordinate3 * t=new SoTextureCoordinate3; s.root->addChild(t);
  std::vector<SbVec3f> expected; bool want=false; SoVBO * last=NULL;
  s.call(s.root,[&](SoState * state) {
    cpu(state,target,expected); s.preserved(state,otherunit);
    glActiveTexture(GL_TEXTURE0+target); check(!glIsEnabled(GL_TEXTURE_GEN_S) && !glIsEnabled(GL_TEXTURE_GEN_T) && !glIsEnabled(GL_TEXTURE_GEN_R),"texgen disabled"); glActiveTexture(GL_TEXTURE0);
    if(want) { last=slot(state,target); bytes(last,expected); } else check(slot(state,target)==NULL,"no VBO below threshold, disabled, or empty");
  });
  const int counts[]={2,3,4,4,0,3,1,4,0,3};
  for(int step=0;step<10;++step) {
    label="lifecycle unit="+std::to_string(target)+" step="+std::to_string(step);
    SoVBO::setVertexCountLimits(step==8?0:3,100); expected=values(counts[step],float(step)); assign(t,expected); want=!disabled && counts[step]>=3; s.apply();
    if(!want && last) { const GLvoid * data=NULL; intptr_t size=-1; last->getBufferData(data,size); check(size==0 && last->getBufferDataId()==0,"retired VBO releases data and identity"); }
  }
}
void separator(int target,int otherunit) {
  label="separator/reuse unit="+std::to_string(target); SoVBO::setVertexCountLimits(3,100);
  Scene s; s.seed(target,otherunit); s.unit(s.root,target);
  SoTextureCoordinate3 * shared=new SoTextureCoordinate3; std::vector<SbVec3f> outer=values(4),inner=values(3,10); assign(shared,outer); s.root->addChild(shared); SoVBO * saved=NULL;
  s.call(s.root,[&](SoState * state) { saved=slot(state,target); cpu(state,target,outer); bytes(saved,outer); });
  SoSeparator * sep=new SoSeparator; sep->renderCaching=SoSeparator::OFF; s.root->addChild(sep); SoTextureCoordinate3 * nested=new SoTextureCoordinate3; assign(nested,inner); sep->addChild(nested);
  s.call(sep,[&](SoState * state) { cpu(state,target,inner); bytes(slot(state,target),inner); s.preserved(state,otherunit); });
  s.call(s.root,[&](SoState * state) { check(slot(state,target)==saved,"Separator restores VBO"); cpu(state,target,outer); bytes(slot(state,target),outer); s.preserved(state,otherunit); });
  s.unit(s.root,otherunit); s.root->addChild(shared);
  s.call(s.root,[&](SoState * state) { cpu(state,otherunit,outer); cpu(state,target,outer); bytes(slot(state,otherunit),outer); bytes(slot(state,target),outer); check(SoGLVBOElement::getInstance(state)->getVertexVBO()==&s.vertex,"reused node preserves positions"); });
  for(int i=0;i<3;++i) s.apply();
}
void invalid(int bad) {
  label="invalid unit="+std::to_string(bad); Scene s; s.seed(0,1); s.unit(s.root,0); std::vector<SbVec3f> expected=values(4);
  SoTextureCoordinate3 * t=new SoTextureCoordinate3; t->ref(); assign(t,expected); s.root->addChild(t); SoVBO * saved=NULL;
  s.call(s.root,[&](SoState * state) { saved=slot(state,0); bytes(saved,expected); }); s.apply();
  s.root->removeAllChildren(); s.seed(0,1);
  s.call(s.root,[&](SoState * state) { SoMultiTextureCoordinateElement::set3(state,s.root,0,int(expected.size()),&expected[0]); SoGLVBOElement::setTexCoordVBO(state,0,saved); SoTextureUnitElement::set(state,s.root,bad); });
  // Inject the unit element directly: global SoTextureUnit policy is outside this test.
  s.root->addChild(t);
  s.call(s.root,[&](SoState * state) { check(slot(state,0)==saved,"invalid unit preserves valid slot"); cpu(state,0,expected); s.preserved(state,1); bytes(saved,expected); });
  s.apply(); SoVBO::setVertexCountLimits(1000,2000); s.apply(); t->unref();
}
}
int main(int argc,char ** argv) {
  const std::string mode=argc>1?argv[1]:"lifecycle";
  if(mode!="lifecycle" && mode!="separator" && mode!="disabled" && mode!="invalid-negative" && mode!="invalid-limit" && mode!="invalid-above") return 2;
  SoDB::init(); Display * d=XOpenDisplay(NULL); if(!d) { std::puts("SKIP: no X display"); return 77; }
  int attrs[]={GLX_RGBA,GLX_DEPTH_SIZE,16,None}; XVisualInfo * v=glXChooseVisual(d,DefaultScreen(d),attrs); if(!v) { XCloseDisplay(d); return 77; }
  GLXContext c=glXCreateContext(d,v,NULL,True); if(!c) { XFree(v); XCloseDisplay(d); return 77; }
  Colormap map=XCreateColormap(d,RootWindow(d,v->screen),v->visual,AllocNone); XSetWindowAttributes wa={}; wa.colormap=map;
  Window w=XCreateWindow(d,RootWindow(d,v->screen),0,0,16,16,0,v->depth,InputOutput,v->visual,CWColormap,&wa); if(!glXMakeCurrent(d,w,c)) return 77;
  const cc_glglue * glue=cc_glglue_instance(contextid); const int units=cc_glglue_max_texture_units(glue);
  std::printf("renderer=%s units=%d mode=%s\n",glGetString(GL_RENDERER),units,mode.c_str());
  if(units<2 || !cc_glglue_has_vertex_buffer_object(glue) ||
     !SoGLDriverDatabase::isSupported(glue,SO_GL_FRAMEBUFFER_OBJECT)) { std::puts("SKIP: required multitexture/VBO/FBO capability unavailable"); return 77; }
  SoVBO::setVertexCountLimits(3,100);
  if(mode.find("invalid-")==0) invalid(mode=="invalid-negative"?-1:mode=="invalid-limit"?units:units+5);
  else { std::vector<int> targets; targets.push_back(0); targets.push_back(1); if(units>2) targets.push_back(units-1);
    for(size_t i=0;i<targets.size();++i) { int u=targets[i],other=u==0?1:0; if(mode=="separator") separator(u,other); else lifecycle(u,other,mode=="disabled"); }
  }
  SoContextHandler::destructingContext(contextid); glXMakeCurrent(d,None,NULL); glXDestroyContext(d,c); XDestroyWindow(d,w); XFreeColormap(d,map); XFree(v); XCloseDisplay(d); SoDB::finish();
  std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0;
}
