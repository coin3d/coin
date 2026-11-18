// Evaluate the public CPU coordinate function at a real shape traversal point.
// Analytical normal transforms below deliberately avoid SbMatrix::inverse()
// and transpose(), so the numerical oracle is separate from the implementation.
#ifdef COIN_NORMALMAP_GLX
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoContextHandler.h>
#endif
#include <Inventor/SoDB.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbRotation.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {
int checks = 0, failures = 0;
const uint32_t contextid = 978;
std::string actionname;
void check(bool ok, const std::string & label, const char * message) {
  ++checks;
  if (!ok) {
    ++failures;
    std::fprintf(stderr, "FAIL %s %s: %s\n", actionname.c_str(), label.c_str(), message);
  }
}
void equal(const SbVec4f & got, const SbVec4f & want, const std::string & label) {
  bool ok = true;
  for (int i = 0; i < 4; ++i)
    ok = ok && std::isfinite(got[i]) && std::fabs(got[i] - want[i]) < 2e-5f;
  ++checks;
  if (!ok) {
    ++failures;
    std::fprintf(stderr, "FAIL %s %s: got (%g,%g,%g,%g), want (%g,%g,%g,%g)\n",
      actionname.c_str(), label.c_str(), got[0],got[1],got[2],got[3],
      want[0],want[1],want[2],want[3]);
  }
}
enum Model { IDENTITY, ROTATE_Z, STRETCH, SHEAR, STRETCH_SHEAR, REFLECT };
enum View { FRONT, CAMERA_Y, CAMERA_Z };
struct Expected {
  Model model;
  View view;
  int unit;
  bool normalmap;
  Expected(Model m, View v, int u, bool n = true) : model(m),view(v),unit(u),normalmap(n) {}
};
SbVec4f oracle(const SbVec3f & n, Model model, View view) {
  // Matrices use Coin's row-vector convention. These formulas are the exact
  // dual maps of modelMatrix() below, including a negative determinant.
  double x = n[0], y = n[1], z = n[2];
  const double nx = x, ny = y;
  switch (model) {
  case ROTATE_Z: x = -ny; y = nx; break;
  case STRETCH: x /= 2; y /= 3; z /= 4; break;
  case SHEAR: x = nx - 2*ny; break;
  case STRETCH_SHEAR: x = nx/2 - ny/6; y = ny/3; z /= 4; break;
  case REFLECT: x /= -2; y /= 3; z /= 4; break;
  case IDENTITY: break;
  }
  const double mx = x, my = y, mz = z;
  // Camera orientations rotate eye space into world space; normals use the
  // reverse rotation, after the model's normal transform.
  if (view == CAMERA_Y) { x = -mz; z = mx; }
  if (view == CAMERA_Z) { x = my; y = -mx; }
  const double length = std::sqrt(x*x + y*y + z*z);
  if (length == 0) return SbVec4f(0,0,0,1);
  return SbVec4f(float(x/length),float(y/length),float(z/length),1);
}
SbMatrix modelMatrix(Model model, bool translated = false) {
  SbMatrix m = SbMatrix::identity();
  switch (model) {
  case ROTATE_Z: m[0][0]=0; m[0][1]=1; m[1][0]=-1; m[1][1]=0; break;
  case STRETCH: m[0][0]=2; m[1][1]=3; m[2][2]=4; break;
  case SHEAR: m[0][1]=2; break;
  case STRETCH_SHEAR: m[0][0]=2; m[0][1]=1; m[1][1]=3; m[2][2]=4; break;
  case REFLECT: m[0][0]=-2; m[1][1]=3; m[2][2]=4; break;
  case IDENTITY: break;
  }
  if (translated) { m[3][0]=7; m[3][1]=-11; m[3][2]=19; }
  return m;
}
void transform(SoGroup * g, Model m, bool translated = false) {
  SoMatrixTransform * t = new SoMatrixTransform;
  t->matrix = modelMatrix(m,translated); g->addChild(t);
}
void camera(SoGroup * g, View view, bool translated = false) {
  SoOrthographicCamera * c = new SoOrthographicCamera;
  c->position.setValue(translated ? SbVec3f(13,-17,23) : SbVec3f(0,0,8));
  c->height = 100; c->nearDistance = .1f; c->farDistance = 1000;
  const float halfpi = 1.5707963267948966f;
  if (view == CAMERA_Y) c->orientation = SbRotation(SbVec3f(0,1,0),halfpi);
  if (view == CAMERA_Z) c->orientation = SbRotation(SbVec3f(0,0,1),halfpi);
  g->addChild(c);
}
void select(SoGroup * g, int unit) {
  SoTextureUnit * n = new SoTextureUnit; n->unit = unit; g->addChild(n);
}
void object(SoGroup * g) {
  SoTextureCoordinateObject * n = new SoTextureCoordinateObject;
  n->factorS.setValue(2,0,0,.125f); n->factorT.setValue(0,3,0,.25f);
  n->factorR.setValue(0,0,4,.5f); n->factorQ.setValue(0,0,0,2);
  g->addChild(n);
}
class Probe : public SoCube {
public:
  std::string label;
  std::vector<Expected> expected;
  int visits = 0;
  void callback(SoCallbackAction * a) override { inspect(a,false); }
  void rayPick(SoRayPickAction * a) override { inspect(a,false); }
  void GLRender(SoGLRenderAction * a) override { inspect(a,true); }
  void inspect(SoAction * a, bool gl) {
    ++visits;
    const SoMultiTextureCoordinateElement * e = SoMultiTextureCoordinateElement::getInstance(a->getState());
    const SbVec3f normals[] = {
      SbVec3f(1,0,0),SbVec3f(-1,0,0),SbVec3f(0,1,0),SbVec3f(0,-1,0),
      SbVec3f(0,0,1),SbVec3f(0,0,-1),SbVec3f(2,-3,6),SbVec3f(-4,6,-12),
      SbVec3f(0,0,0),SbVec3f(2e20f,-3e20f,6e20f),SbVec3f(2e-30f,-3e-30f,6e-30f)
    };
    const SbVec3f points[] = {SbVec3f(0,0,0),SbVec3f(17,-23,31),SbVec3f(-9,4,-7)};
    for (size_t i = 0; i < expected.size(); ++i) {
      const Expected & item = expected[i];
      const std::string where = label + " unit=" + std::to_string(item.unit);
      check(e->getType(item.unit) == (gl ? SoMultiTextureCoordinateElement::TEXGEN :
         SoMultiTextureCoordinateElement::FUNCTION),where,"registered coordinate function/type");
      if (!item.normalmap) {
        equal(e->get(item.unit,SbVec3f(1,2,3),SbVec3f(0,0,1)),
              SbVec4f(2.125f,6.25f,12.5f,2),where+" preserved other generator");
      } else {
        for (size_t j = 0; j < sizeof(normals)/sizeof(normals[0]); ++j) {
          const SbVec4f want = oracle(normals[j],item.model,item.view);
          for (size_t p = 0; p < sizeof(points)/sizeof(points[0]); ++p) {
            const std::string sample = where+" normal="+std::to_string(j)+" point="+std::to_string(p);
            const SbVec4f got = e->get(item.unit,points[p],normals[j]);
            equal(got,want,sample);
            // Calling another normal first exposes stale result/cached input.
            (void)e->get(item.unit,points[(p+1)%3],SbVec3f(3,7,-2));
            equal(e->get(item.unit,points[p],normals[j]),want,sample+" repeated");
          }
        }
      }
#ifdef COIN_NORMALMAP_GLX
      if (gl) {
        GLint active = 0; glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
        glActiveTexture(GL_TEXTURE0+item.unit);
        const GLenum coords[] = {GL_S,GL_T,GL_R};
        const GLenum enables[] = {GL_TEXTURE_GEN_S,GL_TEXTURE_GEN_T,GL_TEXTURE_GEN_R};
        for (int c = 0; c < 3; ++c) {
          GLint mode = 0; glGetTexGeniv(coords[c],GL_TEXTURE_GEN_MODE,&mode);
          check(glIsEnabled(enables[c]) == GL_TRUE,where,"hardware texgen remains enabled");
          check(mode == (item.normalmap ? GL_NORMAL_MAP : GL_OBJECT_LINEAR),where,"hardware texgen mode preserved");
        }
        glActiveTexture(active);
        check(glGetError() == GL_NO_ERROR,where,"no invalid GL commands");
      }
#endif
    }
  }
};
Probe * probe(SoGroup * g,const std::string & label,Model m,View v,int u,
               int other = -1,bool otherNormal = false) {
  Probe * p = new Probe; p->label = label; p->expected.push_back(Expected(m,v,u));
  if (other >= 0) p->expected.push_back(Expected(m,v,other,otherNormal));
  g->addChild(p); return p;
}
void apply(SoGroup * root, const std::vector<Probe *> & probes) {
  root->ref();
  for (int repeat = 0; repeat < 2; ++repeat) {
    if (actionname == "callback") { SoCallbackAction a; a.apply(root); }
    else if (actionname == "pick") {
      SoRayPickAction a(SbViewportRegion(16,16));
      a.enableCulling(FALSE); a.setRay(SbVec3f(0,0,8),SbVec3f(0,0,-1)); a.apply(root);
    }
#ifdef COIN_NORMALMAP_GLX
    else { SoGLRenderAction a(SbViewportRegion(16,16)); a.setCacheContext(contextid); a.apply(root); }
#endif
    for (size_t i = 0; i < probes.size(); ++i)
      check(probes[i]->visits == repeat+1,probes[i]->label,"probe reached on repeated traversal");
  }
  root->unref();
}
void numericalCases() {
  struct Case { const char * name; Model model; View view; bool translated; };
  const Case cases[] = {
    {"identity",IDENTITY,FRONT,false}, {"model rotation",ROTATE_Z,FRONT,false},
    {"camera rotation Y",IDENTITY,CAMERA_Y,false}, {"camera rotation Z",IDENTITY,CAMERA_Z,false},
    {"model and camera noncommuting",ROTATE_Z,CAMERA_Y,false},
    {"nonuniform scale",STRETCH,FRONT,false}, {"shear",SHEAR,FRONT,false},
    {"scale shear and camera",STRETCH_SHEAR,CAMERA_Y,false},
    {"negative determinant",REFLECT,CAMERA_Z,false},
    {"model and camera translation",IDENTITY,FRONT,true},
    {"scale shear camera translation",STRETCH_SHEAR,CAMERA_Z,true}
  };
  for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
    const Case & c = cases[i];
    for (int unit = 0; unit <= 1; ++unit) {
      // Install the callback both before and after the model matrix. The
      // function must use the shape's current traversal state in either case.
      for (int before = 0; before < 2; ++before) {
        SoGroup * root = new SoGroup; camera(root,c.view,c.translated);
        select(root,1-unit); object(root); select(root,unit);
        if (before) root->addChild(new SoTextureCoordinateNormalMap);
        transform(root,c.model,c.translated);
        if (!before) root->addChild(new SoTextureCoordinateNormalMap);
        Probe * p = probe(root,std::string(c.name)+(before?" generator first":" transform first"),
                         c.model,c.view,unit,1-unit);
        apply(root,std::vector<Probe *>(1,p));
      }
    }
  }
}
void defaultView() {
  SoGroup * root = new SoGroup; root->addChild(new SoTextureCoordinateNormalMap);
  Probe * p = probe(root,"default view without camera",IDENTITY,FRONT,0);
  apply(root,std::vector<Probe *>(1,p));
}
void scopes() {
  for (int unit = 0; unit <= 1; ++unit) {
    SoGroup * root = new SoGroup; camera(root,FRONT);
    select(root,1-unit); object(root); select(root,unit);
    // Reuse this public node in distinct scopes and units. Its callback must
    // remain tied to the current element state, including after Separator pop.
    SoTextureCoordinateNormalMap * shared = new SoTextureCoordinateNormalMap;
    root->addChild(shared);
    std::vector<Probe *> probes;
    probes.push_back(probe(root,"outer identity",IDENTITY,FRONT,unit,1-unit));
    SoSeparator * sep = new SoSeparator;
    sep->renderCaching=SoSeparator::OFF; sep->renderCulling=SoSeparator::OFF; sep->pickCulling=SoSeparator::OFF;
    root->addChild(sep); camera(sep,CAMERA_Y,true); transform(sep,STRETCH_SHEAR,true);
    probes.push_back(probe(sep,"inherited callback inside separator",STRETCH_SHEAR,CAMERA_Y,unit,1-unit));
    // Install the same NormalMap on the other slot inside the separator.
    select(sep,1-unit); sep->addChild(shared);
    probes.push_back(probe(sep,"same node on two slots",STRETCH_SHEAR,CAMERA_Y,unit,1-unit,true));
    probes.push_back(probe(root,"separator restores both slots and matrices",IDENTITY,FRONT,unit,1-unit));
    SoGroup * group = new SoGroup; root->addChild(group);
    transform(group,ROTATE_Z); group->addChild(shared);
    probes.push_back(probe(group,"group updated normal matrix",ROTATE_Z,FRONT,unit,1-unit));
    probes.push_back(probe(root,"group state persists",ROTATE_Z,FRONT,unit,1-unit));
    apply(root,probes);
  }
}
}
int main(int argc,char ** argv) {
  actionname = argc > 1 ? argv[1] : "callback";
  if (actionname != "callback" && actionname != "pick" && actionname != "gl") return 2;
#ifndef COIN_NORMALMAP_GLX
  if (actionname == "gl") return 77;
#endif
  SoDB::init();
#ifdef COIN_NORMALMAP_GLX
  Display * display = NULL; XVisualInfo * visual = NULL; GLXContext context = NULL;
  Colormap map = 0; Window window = 0;
  if (actionname == "gl") {
    display = XOpenDisplay(NULL);
    if (!display) { std::puts("SKIP: no X display"); return 77; }
    int attrs[] = {GLX_RGBA,GLX_DEPTH_SIZE,16,None};
    visual = glXChooseVisual(display,DefaultScreen(display),attrs);
    if (!visual) return 77;
    context = glXCreateContext(display,visual,NULL,True); if (!context) return 77;
    map = XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
    XSetWindowAttributes wa = {}; wa.colormap = map;
    window = XCreateWindow(display,RootWindow(display,visual->screen),0,0,16,16,0,
      visual->depth,InputOutput,visual->visual,CWColormap,&wa);
    if (!glXMakeCurrent(display,window,context)) return 77;
    const cc_glglue * glue = cc_glglue_instance(contextid);
    if (cc_glglue_max_texture_units(glue) < 2) { std::puts("SKIP: two texture units unavailable"); return 77; }
    int major = 0,minor = 0;
    const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (version) std::sscanf(version,"%d.%d",&major,&minor);
    const char * raw = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    const std::string extensions = std::string(" ")+(raw?raw:"")+" ";
    if (!(major > 1 || (major == 1 && minor >= 3) ||
          extensions.find(" GL_ARB_texture_cube_map ") != std::string::npos ||
          extensions.find(" GL_EXT_texture_cube_map ") != std::string::npos)) {
      std::puts("SKIP: cube map texgen unavailable"); return 77;
    }
    std::printf("renderer=%s\n",glGetString(GL_RENDERER));
  }
#endif
  defaultView(); numericalCases(); scopes();
#ifdef COIN_NORMALMAP_GLX
  if (display) {
    SoContextHandler::destructingContext(contextid);
    glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
    XDestroyWindow(display,window); XFreeColormap(display,map); XFree(visual); XCloseDisplay(display);
  }
#endif
  SoDB::finish();
  std::printf("%s: %d checks, %d failures\n",actionname.c_str(),checks,failures);
  return failures ? 1 : 0;
}
