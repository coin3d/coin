#include <cstdio>
#include <cstdlib>
#include <memory>
#include <new>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include "glue/glp.h"
#include "rendering/SoVBO.h"
#include "shaders/SoGLSLShaderProgram.h"
#include "shaders/SoGLShaderObject.h"
#include "shaders/SoGLSLShaderParameter.h"
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#c); return 1; } } while (0)
struct Context {
  GLXContext glx; Window window; Colormap colormap; uint32_t id; cc_glglue * glue;
  COIN_PFNGLGENBUFFERSPROC gen; COIN_PFNGLDELETEBUFFERSPROC delbuf;
  COIN_PFNGLCREATEPROGRAMOBJECTARBPROC program; COIN_PFNGLCREATESHADEROBJECTARBPROC shader;
  COIN_PFNGLDELETEOBJECTARBPROC delobj;
  int buffers, programs, shaders, generated, createdprograms, createdshaders;
};
static Context * current;
static void APIENTRY genBuffers(GLsizei n,GLuint * names) {
  current->gen(n,names); current->buffers+=n; current->generated+=n;
}
static void APIENTRY deleteBuffers(GLsizei n,const GLuint * names) {
  current->delbuf(n,names); current->buffers-=n;
}
static COIN_GLhandle APIENTRY createProgram() {
  COIN_GLhandle h=current->program(); if (h) { ++current->programs; ++current->createdprograms; } return h;
}
static COIN_GLhandle APIENTRY createShader(GLenum kind) {
  COIN_GLhandle h=current->shader(kind); if (h) { ++current->shaders; ++current->createdshaders; } return h;
}
static void APIENTRY deleteObject(COIN_GLhandle h) {
  if (glIsProgram(h)) --current->programs;
  else if (glIsShader(h)) --current->shaders;
  current->delobj(h);
}
static bool select(Display * display,Context & c) {
  current=&c; return glXMakeCurrent(display,c.window,c.glx)==True;
}
class CountParameter : public SoGLSLShaderParameter {
public:
  static int live; SoShader::Type kind;
  explicit CountParameter(SoShader::Type kind) : kind(kind) { ++live; }
  ~CountParameter() { --live; }
  SoShader::Type shaderType() const override { return kind; }
};
int CountParameter::live;
class MockShader : public SoGLShaderObject {
public:
  SoShader::Type kind; bool fail;
  explicit MockShader(uint32_t id) : SoGLShaderObject(id),kind(SoShader::GLSL_SHADER),fail(false) { }
  SbBool isLoaded() const override { return TRUE; }
  void load(const char *) override { }
  void unload() override { }
  SoShader::Type shaderType() const override { return kind; }
  SoGLShaderParameter * getNewParameter() const override {
    if (fail) throw std::bad_alloc();
    return new CountParameter(kind);
  }
};
class UniformProbe : public SoShaderParameter1f {
public:
  using SoUniformShaderParameter::ensureParameter;
  using SoUniformShaderParameter::getGLShaderParameter;
};
static bool render(SoSeparator * root,Context & c) {
  SoGLRenderAction action(SbViewportRegion(64,64)); action.setCacheContext(c.id);
  glViewport(0,0,64,64); glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  action.apply(root); glFinish(); unsigned char pixel[4];
  glReadPixels(32,32,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);
  return pixel[0]>150 && pixel[1]>30 && pixel[1]<90 && pixel[2]>40 && pixel[2]<110 && glGetError()==GL_NO_ERROR;
}
int main() {
  Display * display=XOpenDisplay(NULL); if (!display) return 77;
  int attrs[]={GLX_RGBA,GLX_DOUBLEBUFFER,GLX_RED_SIZE,8,GLX_GREEN_SIZE,8,GLX_BLUE_SIZE,8,GLX_DEPTH_SIZE,16,None};
  XVisualInfo * visual=glXChooseVisual(display,DefaultScreen(display),attrs); if (!visual) return 77;
  SoDB::init();
  Context contexts[5]={};
  for (int i=0;i<5;++i) {
    Context & c=contexts[i]; c.id=SoGLCacheContextElement::getUniqueCacheContext();
    c.colormap=XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
    XSetWindowAttributes swa={}; swa.colormap=c.colormap;
    c.window=XCreateWindow(display,RootWindow(display,visual->screen),0,0,64,64,0,visual->depth,InputOutput,visual->visual,CWColormap,&swa);
    XMapWindow(display,c.window); XSync(display,False);
    c.glx=glXCreateContext(display,visual,NULL,True); CHECK(c.glx && select(display,c));
    c.glue=const_cast<cc_glglue *>(cc_glglue_instance(c.id));
    CHECK(c.glue->glCreateShaderObjectARB && c.glue->glCreateProgramObjectARB && c.glue->glGenBuffers);
    c.gen=c.glue->glGenBuffers; c.delbuf=c.glue->glDeleteBuffers;
    c.program=c.glue->glCreateProgramObjectARB; c.shader=c.glue->glCreateShaderObjectARB; c.delobj=c.glue->glDeleteObjectARB;
    c.glue->glGenBuffers=genBuffers; c.glue->glDeleteBuffers=deleteBuffers;
    c.glue->glCreateProgramObjectARB=createProgram; c.glue->glCreateShaderObjectARB=createShader; c.glue->glDeleteObjectARB=deleteObject;
  }
  XFree(visual);
  std::unique_ptr<SoVBO> vbo(new SoVBO);
  float data[4]={1,2,3,4}; vbo->setBufferData(data,sizeof(data));
  std::unique_ptr<SoGLSLShaderProgram> handles(new SoGLSLShaderProgram);
  UniformProbe * uniform=new UniformProbe; uniform->ref();
  std::unique_ptr<MockShader> mocks[5];
  SoSeparator * root=new SoSeparator; root->ref(); root->renderCaching=SoSeparator::OFF;
  SoOrthographicCamera * camera=new SoOrthographicCamera; camera->position.setValue(0,0,3); root->addChild(camera);
  SoShaderProgram * program=new SoShaderProgram;
  SoVertexShader * vertex=new SoVertexShader;
  vertex->sourceType=SoShaderObject::GLSL_PROGRAM; vertex->sourceProgram="void main() { gl_Position=ftransform(); }";
  SoFragmentShader * fragment=new SoFragmentShader;
  fragment->sourceType=SoShaderObject::GLSL_PROGRAM;
  fragment->sourceProgram="uniform float gain; void main() { gl_FragColor=vec4(gain,0.2,0.3,1.0); }";
  SoShaderParameter1f * gain=new SoShaderParameter1f; gain->name="gain"; gain->value=0.8f;
  fragment->parameter.set1Value(0,gain);
  program->shaderObject.set1Value(0,vertex); program->shaderObject.set1Value(1,fragment);
  root->addChild(program); root->addChild(new SoCube);
  for (int i=0;i<4;++i) {
    Context & c=contexts[i]; CHECK(select(display,c));
    vbo->bindBuffer(c.id); CHECK(handles->getProgramHandle(c.glue,TRUE)!=0);
    mocks[i].reset(new MockShader(c.id)); uniform->ensureParameter(mocks[i].get());
    CHECK(render(root,c));
    const int gen=c.generated, progs=c.createdprograms, shaders=c.createdshaders;
    vbo->bindBuffer(c.id); CHECK(handles->getProgramHandle(c.glue,FALSE)!=0); CHECK(render(root,c));
    CHECK(c.generated==gen && c.createdprograms==progs && c.createdshaders==shaders);
  }
  Context & fifth=contexts[4]; CHECK(select(display,fifth));
  bool caught=false; SbSmallMap<uint32_t,GLuint>::failNextAllocationForTesting();
  try { vbo->bindBuffer(fifth.id); } catch (const std::bad_alloc &) { caught=true; }
  CHECK(caught && fifth.buffers==0 && fifth.generated==1);
  vbo->bindBuffer(fifth.id); CHECK(fifth.buffers==1 && fifth.generated==2);
  caught=false; SbSmallMap<uint32_t,COIN_GLhandle>::failNextAllocationForTesting();
  try { handles->getProgramHandle(fifth.glue,TRUE); } catch (const std::bad_alloc &) { caught=true; }
  CHECK(caught && fifth.programs==0 && handles->getProgramHandle(fifth.glue,FALSE)==0);
  CHECK(handles->getProgramHandle(fifth.glue,TRUE)!=0);
  mocks[4].reset(new MockShader(fifth.id)); caught=false;
  SbSmallMap<uint32_t,SoGLShaderParameter *>::failNextAllocationForTesting();
  try { uniform->ensureParameter(mocks[4].get()); } catch (const std::bad_alloc &) { caught=true; }
  CHECK(caught && CountParameter::live==4 && uniform->getGLShaderParameter(fifth.id)==NULL);
  uniform->ensureParameter(mocks[4].get()); CHECK(CountParameter::live==5);
  SoGLShaderParameter * prior=uniform->getGLShaderParameter(fifth.id);
  mocks[4]->kind=SoShader::ARB_SHADER; mocks[4]->fail=true; caught=false;
  try { uniform->ensureParameter(mocks[4].get()); } catch (const std::bad_alloc &) { caught=true; }
  CHECK(caught && uniform->getGLShaderParameter(fifth.id)==prior && CountParameter::live==5);
  mocks[4]->fail=false; uniform->ensureParameter(mocks[4].get());
  CHECK(uniform->getGLShaderParameter(fifth.id)->shaderType()==SoShader::ARB_SHADER && CountParameter::live==5);
  caught=false; SbSmallMap<uint32_t,SoGLShaderObject *>::failNextAllocationForTesting();
  try { render(root,fifth); } catch (const std::bad_alloc &) { caught=true; }
  CHECK(caught && fifth.shaders==0 && fifth.createdshaders==1);
  CHECK(render(root,fifth)); CHECK(fifth.shaders==2 && fifth.createdshaders==3);
  // Return to old contexts after real spills: no new resources, same pixel.
  for (int i=0;i<4;++i) {
    Context & c=contexts[i]; CHECK(select(display,c));
    const int gen=c.generated,progs=c.createdprograms,shaders=c.createdshaders;
    vbo->bindBuffer(c.id); CHECK(handles->getProgramHandle(c.glue,FALSE)!=0); CHECK(render(root,c));
    CHECK(c.generated==gen && c.createdprograms==progs && c.createdshaders==shaders);
  }
  // Context first, while owners remain alive.
  for (int i=0;i<2;++i) {
    Context & c=contexts[i]; CHECK(select(display,c)); SoContextHandler::destructingContext(c.id);
    CHECK(uniform->getGLShaderParameter(c.id)==NULL && CountParameter::live==4-i);
    CHECK(c.buffers==0 && c.programs==0 && c.shaders==0);
  }
  // Owner first: deferred GL callbacks must later delete exactly once.
  CHECK(select(display,contexts[4])); uniform->unref(); CHECK(CountParameter::live==0);
  root->unref(); handles.reset(); vbo.reset();
  for (int i=2;i<5;++i) {
    Context & c=contexts[i]; CHECK(select(display,c)); SoContextHandler::destructingContext(c.id);
    CHECK(c.buffers==0 && c.programs==0 && c.shaders==0);
  }
  for (int i=0;i<5;++i) mocks[i].reset();
  glXMakeCurrent(display,None,NULL);
  for (Context & c:contexts) { glXDestroyContext(display,c.glx); XDestroyWindow(display,c.window); XFreeColormap(display,c.colormap); }
  XCloseDisplay(display); SoDB::finish();
  std::puts("Four GL maps: five unshared contexts, insertion faults, retry, hits, pixels and both teardown orders passed.");
  return 0;
}
