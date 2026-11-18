// Traverse the legacy renderer in a real compatibility context. Each invalid
// unit/node pair runs in its own process, including INT_MIN before any indexing.
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoGLVBOElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTextureCoordinateCube.h>
#include <Inventor/nodes/SoTextureCoordinateCylinder.h>
#include <Inventor/nodes/SoTextureCoordinateSphere.h>
#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include "rendering/SoVBO.h"
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

namespace {
const uint32_t contextid = 967;
int checks = 0, failures = 0;
std::string label;
void check(bool ok, const char * msg) {
  ++checks;
  if (!ok) { ++failures; std::fprintf(stderr, "FAIL %s: %s\n", label.c_str(), msg); }
}
void equal(const SbVec4f & a, const SbVec4f & b, const char * msg) {
  for (int i=0;i<4;++i) check(std::isfinite(a[i]) && std::fabs(a[i]-b[i]) < 1e-5f, msg);
}
enum Kind { DEFAULT, CUBE, CYLINDER, SPHERE, ENVIRONMENT, OBJECT, PLANE, NORMAL, REFLECTION };
const char * names[] = {"default","cube","cylinder","sphere","environment","object","plane","normalmap","reflectionmap"};
SoNode * make(Kind k) {
  switch (k) {
  case DEFAULT: return new SoTextureCoordinateDefault;
  case CUBE: return new SoTextureCoordinateCube;
  case CYLINDER: return new SoTextureCoordinateCylinder;
  case SPHERE: return new SoTextureCoordinateSphere;
  case ENVIRONMENT: return new SoTextureCoordinateEnvironment;
  case OBJECT: {
    SoTextureCoordinateObject * n = new SoTextureCoordinateObject;
    n->factorS.setValue(2,0,0,.125f); n->factorT.setValue(0,3,0,.25f);
    n->factorR.setValue(0,0,4,.5f); n->factorQ.setValue(0,0,0,2); return n;
  }
  case PLANE: return new SoTextureCoordinatePlane;
  case NORMAL: return new SoTextureCoordinateNormalMap;
  case REFLECTION: return new SoTextureCoordinateReflectionMap;
  }
  return NULL;
}
// These probes retain a genuine SoShape tail. Generated callbacks use its bbox.
class Shape : public SoCube {
public:
  std::function<void(SoGLRenderAction *)> fn;
  int boxes = 0;
  void GLRender(SoGLRenderAction * a) override { if (fn) fn(a); }
  void computeBBox(SoAction * a, SbBox3f & b, SbVec3f & c) override {
    ++boxes; SoCube::computeBBox(a,b,c);
  }
};
Shape * call(SoGroup * g, std::function<void(SoGLRenderAction *)> fn) {
  Shape * p = new Shape; p->fn = fn; g->addChild(p); return p;
}
void unit(SoGroup * g, int u) {
  SoTextureUnit * n = new SoTextureUnit; n->unit = u; g->addChild(n);
}
void apply(SoGroup * g) {
  SoGLRenderAction a(SbViewportRegion(16,16)); a.setCacheContext(contextid); a.apply(g);
  check(glGetError()==GL_NO_ERROR,"traversal issued no invalid GL commands");
}
struct Function {
  SbVec4f value;
  int calls = 0;
  Function() : value(.37f,.23f,.79f,2.f) {}
  static const SbVec4f & run(void * p,const SbVec3f &,const SbVec3f &) {
    Function * f=static_cast<Function *>(p); ++f->calls; return f->value;
  }
};
const GLenum coords[] = {GL_S,GL_T,GL_R,GL_Q};
const GLenum enables[] = {GL_TEXTURE_GEN_S,GL_TEXTURE_GEN_T,GL_TEXTURE_GEN_R,GL_TEXTURE_GEN_Q};
struct GLUnit {
  GLboolean enabled[4]; GLint mode[4]; GLfloat object[4][4],eye[4][4];
  void capture(int u) {
    glActiveTexture(GL_TEXTURE0+u);
    for(int c=0;c<4;++c) {
      enabled[c]=glIsEnabled(enables[c]); glGetTexGeniv(coords[c],GL_TEXTURE_GEN_MODE,&mode[c]);
      glGetTexGenfv(coords[c],GL_OBJECT_PLANE,object[c]); glGetTexGenfv(coords[c],GL_EYE_PLANE,eye[c]);
    }
  }
  void verify(int u) const {
    GLUnit after; after.capture(u);
    for(int c=0;c<4;++c) {
      check(after.enabled[c]==enabled[c],"preserved S/T/R/Q enable");
      check(after.mode[c]==mode[c],"preserved S/T/R/Q mode");
      for(int j=0;j<4;++j) {
        check(after.object[c][j]==object[c][j],"preserved S/T/R/Q object plane");
        check(after.eye[c][j]==eye[c][j],"preserved S/T/R/Q eye plane");
      }
    }
  }
};
struct Buffer {
  SoVBO * ptr; SbUniqueId id; GLint name; const GLvoid * storage;
  std::vector<unsigned char> bytes;
  explicit Buffer(SoVBO * v) : ptr(v),id(0),name(0),storage(NULL) {
    if(!v) return;
    id=v->getBufferDataId(); const GLvoid * data=NULL; intptr_t n=0; v->getBufferData(data,n); storage=data;
    if(n>0 && data) bytes.assign(static_cast<const unsigned char *>(data),static_cast<const unsigned char *>(data)+n);
    v->bindBuffer(contextid); glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&name); glBindBuffer(GL_ARRAY_BUFFER,0);
  }
  void verify(SoVBO * v) const {
    check(v==ptr,"VBO slot identity preserved"); if(!v || v!=ptr) return;
    check(v->getBufferDataId()==id,"VBO data id preserved"); const GLvoid * data=NULL; intptr_t n=0; v->getBufferData(data,n);
    check(data==storage,"VBO CPU storage identity preserved");
    check(n==intptr_t(bytes.size()),"VBO CPU byte count preserved");
    if(n!=intptr_t(bytes.size()) || !n || !data) return;
    check(std::memcmp(data,&bytes[0],bytes.size())==0,"VBO CPU bytes preserved");
    v->bindBuffer(contextid); GLint actual=0,size=0; glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&actual);
    check(actual==name && glIsBuffer(GLuint(name)),"VBO GL resource identity preserved");
    glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&size);
    check(size==n,"VBO GL byte count preserved");
    if(size==n) {
      std::vector<unsigned char> got(bytes.size()); glGetBufferSubData(GL_ARRAY_BUFFER,0,n,&got[0]);
      check(got==bytes,"VBO GL bytes preserved");
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
  }
};
struct Snapshot {
  SoElement * match;
  GLint active;
  int slots;
  std::vector<GLUnit> gl;
  std::vector<Buffer> buffers;
  Buffer vertex;
  explicit Snapshot(SoState * s,int units) : match(SoMultiTextureCoordinateElement::getInstance(s)->copyMatchInfo()),
    slots(SoGLVBOElement::getInstance(s)->getNumTexCoordVBO()),vertex(SoGLVBOElement::getInstance(s)->getVertexVBO()) {
    glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
    for(int u=0;u<units;++u) {
      GLUnit x; x.capture(u); gl.push_back(x);
      buffers.push_back(Buffer(SoGLVBOElement::getInstance(s)->getTexCoordVBO(u)));
    }
    glActiveTexture(active);
  }
  ~Snapshot() { delete match; }
  void verify(SoState * s) const {
    GLint now=0; glGetIntegerv(GL_ACTIVE_TEXTURE,&now); check(now==active,"active GL texture preserved");
    check(SoMultiTextureCoordinateElement::getInstance(s)->matches(match),"CPU element slots and node identities preserved");
    const SoGLVBOElement * v=SoGLVBOElement::getInstance(s);
    check(v->getNumTexCoordVBO()==slots,"VBO slot count preserved"); vertex.verify(v->getVertexVBO());
    for(size_t u=0;u<gl.size();++u) { gl[u].verify(int(u)); buffers[u].verify(v->getTexCoordVBO(int(u))); }
    glActiveTexture(active);
  }
};
void seedGLPlanes(int units) {
  for(int u=0;u<units;++u) {
    glActiveTexture(GL_TEXTURE0+u);
    for(int c=0;c<4;++c) {
      GLfloat p[4]={float(u+1),float(c+2),float(u+c+3),float(c+4)};
      glTexGeni(coords[c],GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
      glTexGenfv(coords[c],GL_OBJECT_PLANE,p); glTexGenfv(coords[c],GL_EYE_PLANE,p);
    }
  }
  glActiveTexture(GL_TEXTURE0);
}
void invalid(Kind k,int bad,int units) {
  // Exercise predecessors independently: existing function, explicit array and
  // hardware texgen. Seed VBOs after coordinate setters, which clear that slot.
  const SbVec2f array[]={SbVec2f(.1f,.2f),SbVec2f(.3f,.4f)};
  const SbVec3f data[]={SbVec3f(17,23,31),SbVec3f(7,11,13)};
  for(int phase=0;phase<3;++phase) {
    label=std::string(names[k])+" invalid="+std::to_string(bad)+" predecessor="+std::to_string(phase);
    SoGroup * root=new SoGroup; root->ref(); Function fn;
    SoVBO vertex,texture0,texture1;
    vertex.setBufferData(data,sizeof(data),101); texture0.setBufferData(array,sizeof(array),102); texture1.setBufferData(data,sizeof(data),103);
    Snapshot * before=NULL;
    unit(root,1); root->addChild(make(OBJECT));
    call(root,[&](SoGLRenderAction * a) {
      SoState * s=a->getState();
      SoGLMultiTextureCoordinateElement::setTexGen(s,root,0,NULL);
      if(phase==0) SoMultiTextureCoordinateElement::setFunction(s,root,0,Function::run,&fn);
      else if(phase==1) SoMultiTextureCoordinateElement::set2(s,root,0,2,array);
      else {
        // Use the public GL node on a selected valid unit for a texgen predecessor.
        SoTextureUnitElement::set(s,root,0); root->getChild(1)->GLRender(a);
      }
      SoGLVBOElement::setVertexVBO(s,&vertex); SoGLVBOElement::setTexCoordVBO(s,0,&texture0); SoGLVBOElement::setTexCoordVBO(s,1,&texture1);
      glActiveTexture(GL_TEXTURE1); before=new Snapshot(s,units);
      SoTextureUnitElement::set(s,root,bad);
    });
    root->addChild(make(k));
    call(root,[&](SoGLRenderAction * a) {
      SoState * s=a->getState(); before->verify(s);
      const SoMultiTextureCoordinateElement * e=SoMultiTextureCoordinateElement::getInstance(s);
      check(e->getType(0)==(phase==0?SoMultiTextureCoordinateElement::FUNCTION:phase==1?SoMultiTextureCoordinateElement::EXPLICIT:SoMultiTextureCoordinateElement::TEXGEN),"predecessor type preserved");
      if(phase==0) {
        int n=fn.calls; equal(e->get(0,SbVec3f(1,0,0),SbVec3f(1,0,0)),fn.value,"existing CPU function data preserved");
        check(fn.calls==n+1,"existing function and userdata invoked");
      } else if(phase==1) {
        check(e->getNum(0)==2 && e->getDimension(0)==2,"array count/dimension preserved");
        check(e->getArrayPtr2(0)==array,"array storage identity preserved");
        check(e->get2(0,0)==array[0] && e->get2(0,1)==array[1],"array values preserved");
      } else equal(e->get(0,SbVec3f(1,0,0),SbVec3f(1,0,0)),SbVec4f(2.125f,.25f,.5f,2),"existing hardware CPU callback preserved");
      equal(e->get(1,SbVec3f(1,0,0),SbVec3f(1,0,0)),SbVec4f(2.125f,.25f,.5f,2),"other unit CPU callback preserved");
    });
    apply(root); delete before; root->unref();
  }
}
void privateContext(Kind k,int bad) {
  label=std::string(names[k])+" private context invalid="+std::to_string(bad);
  SoGroup * outer=new SoGroup; outer->ref(); unit(outer,0); SoNode * generator=make(k); outer->addChild(generator);
  Shape * outershape=NULL;
  outershape=call(outer,[&](SoGLRenderAction * a) {
    const SoMultiTextureCoordinateElement * e=SoMultiTextureCoordinateElement::getInstance(a->getState());
    SbVec3f p(1,.5f,.25f),n(1,0,0); SbVec4f expected=e->get(0,p,n);
    const int boxes=outershape->boxes;
    SoGroup * inner=new SoGroup; inner->ref(); Shape * innershape=NULL;
    innershape=call(inner,[&](SoGLRenderAction * other) {
      SoTextureUnitElement::set(other->getState(),inner,bad);
      generator->GLRender(other);
      equal(e->get(0,p,n),expected,"invalid visit in other action preserves live CPU callback context");
      check(outershape->boxes==boxes,"invalid visit preserves cached outer shape");
      check(innershape->boxes==0,"CPU callback does not use invalid action's shape/state");
    });
    innershape->width=4; innershape->height=8; innershape->depth=6;
    // Evaluate while the other action still has a live, real shape tail. This
    // isolates invalid-visit side effects from unrelated valid reentrancy.
    apply(inner); inner->unref();
    // A wrongly retained pointer would dangle after the nested action dies;
    // the checks above catch it without dereferencing that dangling pointer.
  });
  apply(outer); outer->unref();
}
void validCPU(SoState * s,int u,Kind k) {
  const SoMultiTextureCoordinateElement * e=SoMultiTextureCoordinateElement::getInstance(s);
  const bool hardware=k>=ENVIRONMENT;
  check(e->getType(u)==(k==DEFAULT?SoMultiTextureCoordinateElement::DEFAULT:hardware?SoMultiTextureCoordinateElement::TEXGEN:SoMultiTextureCoordinateElement::FUNCTION),"valid unit coordinate type");
  if(k!=DEFAULT) {
    const SbVec4f want[]={SbVec4f(),SbVec4f(.5f,.5f,0,1),SbVec4f(.75f,.5f,0,1),SbVec4f(.75f,.5f,0,1),
      SbVec4f(.14644661f,.5f,0,1),SbVec4f(2.125f,.25f,.5f,2),SbVec4f(1,0,0,1),SbVec4f(1,0,0,1),SbVec4f(-1,0,0,1)};
    equal(e->get(u,SbVec3f(1,0,0),SbVec3f(1,0,0)),want[k],"valid CPU mapping");
  }
  GLint active=0; glGetIntegerv(GL_ACTIVE_TEXTURE,&active); glActiveTexture(GL_TEXTURE0+u);
  for(int c=0;c<4;++c) {
    check(bool(glIsEnabled(enables[c]))==hardware,"valid S/T/R/Q enable state");
    if(hardware && (k!=ENVIRONMENT || c<2) && (k!=NORMAL && k!=REFLECTION || c<3)) {
      GLint mode=0; glGetTexGeniv(coords[c],GL_TEXTURE_GEN_MODE,&mode);
      check(mode==GLint(k==ENVIRONMENT?GL_SPHERE_MAP:k==NORMAL?GL_NORMAL_MAP:k==REFLECTION?GL_REFLECTION_MAP:GL_OBJECT_LINEAR),"valid texgen mode");
    }
    if(k==OBJECT || k==PLANE) {
      const GLfloat object[][4]={{2,0,0,.125f},{0,3,0,.25f},{0,0,4,.5f},{0,0,0,2}};
      const GLfloat plane[][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
      GLfloat got[4]; glGetTexGenfv(coords[c],GL_OBJECT_PLANE,got);
      for(int j=0;j<4;++j) check(got[j]==(k==OBJECT?object[c][j]:plane[c][j]),"valid object planes");
    }
  }
  glActiveTexture(active);
}
void valid(Kind k,int u,int units) {
  label=std::string(names[k])+" valid unit="+std::to_string(u);
  const int other=u==0?1:0;
  const SbVec2f array[]={SbVec2f(.11f,.22f),SbVec2f(.33f,.44f)};
  // Arrays and software/hardware predecessors, sequential Group updates and
  // Separator push/pop all exercise the selected unit and a second sentinel.
  for(int predecessor=0;predecessor<3;++predecessor) {
    SoGroup * root=new SoGroup; root->ref(); unit(root,other); root->addChild(make(OBJECT));
    unit(root,u); Function f; SoVBO selected,otherbuffer,vertex;
    selected.setBufferData(array,sizeof(array),201); otherbuffer.setBufferData(array,sizeof(array),202); vertex.setBufferData(array,sizeof(array),203);
    call(root,[&](SoGLRenderAction * a) {
      if(predecessor==0) SoMultiTextureCoordinateElement::set2(a->getState(),root,u,2,array);
      if(predecessor==1) SoMultiTextureCoordinateElement::setFunction(a->getState(),root,u,Function::run,&f);
    });
    if(predecessor==2) root->addChild(make(OBJECT));
    call(root,[&](SoGLRenderAction * a) {
      SoGLVBOElement::setTexCoordVBO(a->getState(),u,&selected);
      SoGLVBOElement::setTexCoordVBO(a->getState(),other,&otherbuffer);
      SoGLVBOElement::setVertexVBO(a->getState(),&vertex);
    });
    root->addChild(make(k));
    call(root,[&](SoGLRenderAction * a) { validCPU(a->getState(),u,k); validCPU(a->getState(),other,OBJECT);
      const SoGLVBOElement * v=SoGLVBOElement::getInstance(a->getState());
      check(v->getTexCoordVBO(u)==NULL,"valid generator clears selected VBO");
      check(v->getTexCoordVBO(other)==&otherbuffer && v->getVertexVBO()==&vertex,"valid generator preserves other/vertex VBO");
    });
    // Visit every other generator sequentially then return to the target.
    root->addChild(make(k==OBJECT?CUBE:OBJECT)); root->addChild(make(k));
    call(root,[&](SoGLRenderAction * a) { validCPU(a->getState(),u,k); });
    Snapshot * before=NULL;
    call(root,[&](SoGLRenderAction * a) { before=new Snapshot(a->getState(),units); });
    SoSeparator * sep=new SoSeparator; sep->renderCaching=SoSeparator::OFF; root->addChild(sep);
    const Kind nested=k==CUBE?CYLINDER:CUBE; sep->addChild(make(nested));
    call(sep,[&](SoGLRenderAction * a) { validCPU(a->getState(),u,nested); });
    call(root,[&](SoGLRenderAction * a) { before->verify(a->getState()); validCPU(a->getState(),u,k); });
    apply(root); delete before; root->unref();
  }
}
}
int main(int argc,char ** argv) {
  if(argc!=3) return 2;
  Kind k=DEFAULT; while(k<=REFLECTION && std::string(argv[1])!=names[k]) k=Kind(int(k)+1);
  const std::string mode=argv[2];
  if(k>REFLECTION || (mode!="valid" && mode!="negative" && mode!="min" && mode!="limit" && mode!="above")) return 2;
  SoDB::init(); Display * d=XOpenDisplay(NULL); if(!d) { std::puts("SKIP: no X display"); return 77; }
  int attrs[]={GLX_RGBA,GLX_DEPTH_SIZE,16,None}; XVisualInfo * v=glXChooseVisual(d,DefaultScreen(d),attrs); if(!v) return 77;
  GLXContext c=glXCreateContext(d,v,NULL,True); if(!c) return 77;
  Colormap map=XCreateColormap(d,RootWindow(d,v->screen),v->visual,AllocNone); XSetWindowAttributes wa={}; wa.colormap=map;
  Window w=XCreateWindow(d,RootWindow(d,v->screen),0,0,16,16,0,v->depth,InputOutput,v->visual,CWColormap,&wa);
  if(!glXMakeCurrent(d,w,c)) return 77;
  const cc_glglue * glue=cc_glglue_instance(contextid); const int units=cc_glglue_max_texture_units(glue);
  std::printf("renderer=%s units=%d node=%s mode=%s\n",glGetString(GL_RENDERER),units,names[k],mode.c_str());
  if(units<2 || !cc_glglue_has_vertex_buffer_object(glue)) { std::puts("SKIP: multitexture/VBO capability unavailable"); return 77; }
  int major=0,minor=0;
  const char * version=reinterpret_cast<const char *>(glGetString(GL_VERSION));
  if(version) std::sscanf(version,"%d.%d",&major,&minor);
  const char * raw=reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
  const std::string ext=std::string(" ")+(raw?raw:"")+" ";
  const bool cubemap=major>1 || (major==1 && minor>=3) ||
    ext.find(" GL_ARB_texture_cube_map ")!=std::string::npos || ext.find(" GL_EXT_texture_cube_map ")!=std::string::npos;
  if((k==NORMAL || k==REFLECTION) && !cubemap) { std::puts("SKIP: cube map capability unavailable"); return 77; }
  seedGLPlanes(units);
  if(mode=="valid") {
    valid(k,0,units); valid(k,1,units); if(units>2) valid(k,units-1,units);
  } else {
    const int bad=mode=="negative"?-1:mode=="min"?INT_MIN:mode=="limit"?units:units+5;
    if(k==CUBE || k==CYLINDER || k==SPHERE) privateContext(k,bad);
    invalid(k,bad,units);
  }
  SoContextHandler::destructingContext(contextid); glXMakeCurrent(d,None,NULL); glXDestroyContext(d,c);
  XDestroyWindow(d,w); XFreeColormap(d,map); XFree(v); XCloseDisplay(d); SoDB::finish();
  std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0;
}
