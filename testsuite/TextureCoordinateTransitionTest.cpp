// Exercise the public traversal contract with an actual compatibility GL context.
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTextureCoordinateCube.h>
#include <Inventor/nodes/SoTextureCoordinateCylinder.h>
#include <Inventor/nodes/SoTextureCoordinateSphere.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {
int failures = 0, checks = 0;
std::string label;
void check(bool ok, const char * message) {
  ++checks;
  if (!ok) { ++failures; std::fprintf(stderr, "FAIL %s: %s\n", label.c_str(), message); }
}
enum Kind { DEFAULT, CUBE, CYLINDER, SPHERE, PLANE, ENVIRONMENT, OBJECT, REFLECTION, ARRAY };
SoNode * node(Kind kind) {
  switch (kind) {
  case DEFAULT: return new SoTextureCoordinateDefault;
  case CUBE: return new SoTextureCoordinateCube;
  case CYLINDER: return new SoTextureCoordinateCylinder;
  case SPHERE: return new SoTextureCoordinateSphere;
  case PLANE: return new SoTextureCoordinatePlane;
  case ENVIRONMENT: return new SoTextureCoordinateEnvironment;
  case OBJECT: {
    SoTextureCoordinateObject * p = new SoTextureCoordinateObject;
    p->factorS.setValue(2,0,0,.125f); p->factorT.setValue(0,3,0,.25f);
    p->factorR.setValue(0,0,4,.5f); p->factorQ.setValue(0,0,0,2);
    return p;
  }
  case REFLECTION: return new SoTextureCoordinateReflectionMap;
  case ARRAY: {
    SoTextureCoordinate2 * p = new SoTextureCoordinate2;
    p->point.set1Value(0, SbVec2f(.125f, .875f)); return p;
  }
  }
  std::abort();
}
void select(SoGroup * root, int unit) {
  SoTextureUnit * n = new SoTextureUnit; n->unit = unit; root->addChild(n);
}
struct Expect { int unit; Kind kind; };

// The inherited SoCube supplies a real shape tail and its [-1,1]^3 bbox.
// Never render geometry: inspect the state at exactly the traversal point where
// a shape would request generated coordinates.
class Probe : public SoCube {
public:
  std::vector<Expect> expected;
  int visits;
  Probe() : visits(0) { }
  void GLRender(SoGLRenderAction * a) override { inspect(a, true); }
  void callback(SoCallbackAction * a) override { inspect(a, false); }
  void rayPick(SoRayPickAction * a) override { inspect(a, false); }
  void inspect(SoAction * a, bool gl) {
    ++visits;
    const SoMultiTextureCoordinateElement * e = SoMultiTextureCoordinateElement::getInstance(a->getState());
    GLint active = GL_TEXTURE0;
    if (gl) glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
    for (size_t i = 0; i < expected.size(); ++i) {
      int u = expected[i].unit; Kind k = expected[i].kind;
      bool texgen = k >= PLANE && k <= REFLECTION;
      SoMultiTextureCoordinateElement::CoordType type = e->getType(u);
      check(type == (k == DEFAULT ? SoMultiTextureCoordinateElement::DEFAULT :
                      k == ARRAY ? SoMultiTextureCoordinateElement::EXPLICIT :
                      texgen && gl ? SoMultiTextureCoordinateElement::TEXGEN :
                      SoMultiTextureCoordinateElement::FUNCTION), "coordinate type for selected/preserved unit");
      if (gl) {
        glActiveTexture(GL_TEXTURE0 + u);
        const GLenum attr[] = {GL_TEXTURE_GEN_S, GL_TEXTURE_GEN_T, GL_TEXTURE_GEN_R, GL_TEXTURE_GEN_Q};
        for (int c = 0; c < 4; ++c) check(bool(glIsEnabled(attr[c])) == texgen, "S/T/R/Q texgen enable state");
        if (k == OBJECT) {
          const GLenum coords[] = {GL_S, GL_T, GL_R, GL_Q};
          const float planes[4][4] = {{2,0,0,.125f},{0,3,0,.25f},{0,0,4,.5f},{0,0,0,2}};
          for (int c = 0; c < 4; ++c) {
            GLint mode; GLfloat plane[4];
            glGetTexGeniv(coords[c],GL_TEXTURE_GEN_MODE,&mode);
            glGetTexGenfv(coords[c],GL_OBJECT_PLANE,plane);
            check(mode == GL_OBJECT_LINEAR, "restored/preserved texgen mode");
            for (int j = 0; j < 4; ++j) check(plane[j] == planes[c][j], "restored/preserved S/T/R/Q plane");
          }
        }
      }
      if (k == ARRAY) {
        check(e->getNum(u) == 1, "array count preserved");
        check(e->get2(u, 0) == SbVec2f(.125f,.875f), "array data preserved");
      }
      if (k == CUBE || k == CYLINDER || k == SPHERE) {
        // Fixed values are an independent numerical oracle, including R/Q.
        const SbVec3f p[] = {SbVec3f(1,0,0), SbVec3f(0,1,0), SbVec3f(0,0,1), SbVec3f(-1,0,0), SbVec3f(1,1,0), SbVec3f(-1,-1,0)};
        const SbVec3f n[] = {p[0],p[1],p[2],p[3],SbVec3f(1,0,0),SbVec3f(0,-1,0)};
        const float want[3][6][2] = {
          {{.5f,.5f},{.5f,.5f},{.5f,.5f},{.5f,.5f},{.5f,1.f},{0.f,.5f}},
          {{.75f,.5f},{.5f,.5f},{.5f,.5f},{.25f,.5f},{.75f,1.f},{0.f,.5f}},
          {{.75f,.5f},{.5f,1.f},{.5f,.5f},{.25f,.5f},{.75f,.75f},{.25f,.25f}}
        };
        for (int j = 0; j < 6; ++j) {
          SbVec4f v = e->get(u, p[j], n[j]);
          for (int c = 0; c < 4; ++c) {
            float w = c < 2 ? want[k-CUBE][j][c] : c == 2 ? 0.f : 1.f;
            // Cylinder caps use x/z on top, yielding (.5,.5) here.
            check(std::isfinite(v[c]) && std::fabs(v[c]-w) < 1e-5f, "CPU mapping and callback data");
          }
        }
      }
    }
    if (gl) { glActiveTexture(active); check(glGetError() == GL_NO_ERROR, "no invalid GL commands"); }
  }
};
Probe * probe(SoGroup * root, int u, Kind k, int other = -1, Kind otherkind = OBJECT) {
  Probe * p = new Probe; p->expected.push_back(Expect{u,k});
  if (other >= 0) p->expected.push_back(Expect{other,otherkind});
  root->addChild(p); return p;
}
void apply(SoGroup * root, bool gl) {
  root->ref();
  if (gl) {
    SoGLRenderAction a(SbViewportRegion(16,16)); a.setCacheContext(941); a.apply(root);
  } else {
    SoCallbackAction a; a.apply(root);
    SoRayPickAction r(SbViewportRegion(16,16)); r.setRay(SbVec3f(0,0,5), SbVec3f(0,0,-1)); r.apply(root);
  }
  root->unref();
}
void transitions(bool gl, int units, const std::string & filter, bool reflection = true) {
  const char * names[] = {"default", "cube", "cylinder", "sphere"};
  for (int target = DEFAULT; target <= SPHERE; ++target) {
    if (filter != "all" && filter != names[target] && filter != "cpu") continue;
    for (int u = 0; u < units; ++u) {
      for (int before = PLANE; before <= ARRAY; ++before) {
        // Reflection texgen requires cube-map support in a GL traversal.
        if (gl && before == REFLECTION && !reflection) continue;
        label = std::string(names[target]) + " unit=" + std::to_string(u) + " predecessor=" + std::to_string(before);
        SoGroup * root = new SoGroup;
        int other = units > 1 ? 1-u : -1;
        if (other >= 0) { select(root, other); root->addChild(node(OBJECT)); }
        select(root,u); root->addChild(node(Kind(before)));
        root->addChild(node(Kind(target)));
        Probe * p = probe(root,u,Kind(target),other);
        p->ref(); apply(root,gl); check(p->visits == (gl ? 1 : 2), "shape reached by each action"); p->unref();
      }
      if (units > 1) {
        for (int control = CUBE; control <= ARRAY; ++control) {
          if (control > SPHERE && control != ARRAY) continue;
          label = std::string(names[target]) + " preserve CPU/array unit=" + std::to_string(u);
          SoGroup * root = new SoGroup; select(root,1-u); root->addChild(node(Kind(control)));
          select(root,u); root->addChild(node(OBJECT)); root->addChild(node(Kind(target)));
          probe(root,u,Kind(target),1-u,Kind(control)); apply(root,gl);
        }
      }
      label = std::string(names[target]) + " fresh/sequential unit=" + std::to_string(u);
      SoGroup * fresh = new SoGroup; select(fresh,u);
      fresh->addChild(node(Kind(target))); probe(fresh,u,Kind(target));
      for (int k = CUBE; k <= SPHERE; ++k) { fresh->addChild(node(Kind(k))); probe(fresh,u,Kind(k)); }
      apply(fresh,gl);
      label = std::string(names[target]) + " separator unit=" + std::to_string(u);
      SoGroup * root = new SoGroup; select(root,u); root->addChild(node(OBJECT));
      SoSeparator * inner = new SoSeparator; inner->renderCaching = SoSeparator::OFF;
      inner->addChild(node(Kind(target))); probe(inner,u,Kind(target)); root->addChild(inner);
      probe(root,u,OBJECT); apply(root,gl);
    }
  }
}
void limit(int maxunits) {
  label = "unsupported unit";
  for (int k = DEFAULT; k <= SPHERE; ++k) {
    SoGroup * root = new SoGroup;
    select(root,0); root->addChild(node(OBJECT));
    select(root,maxunits); root->addChild(node(Kind(k)));
    probe(root,0,OBJECT); apply(root,true);
  }
}
}
int main(int argc, char ** argv) {
  std::string filter = argc > 1 ? argv[1] : "all";
  if (filter != "all" && filter != "cpu" && filter != "default" && filter != "cube" &&
      filter != "cylinder" && filter != "sphere" && filter != "limit") return 2;
  SoDB::init();
  if (filter == "cpu") {
    transitions(false,2,filter);
  } else {
    Display * d = XOpenDisplay(NULL);
    if (!d) { std::puts("SKIP: no X display"); return 77; }
    int attrs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    XVisualInfo * v = glXChooseVisual(d, DefaultScreen(d),attrs);
    if (!v) { XCloseDisplay(d); return 77; }
    GLXContext c = glXCreateContext(d,v,NULL,True);
    if (!c) { XFree(v); XCloseDisplay(d); return 77; }
    Colormap map = XCreateColormap(d,RootWindow(d,v->screen),v->visual,AllocNone);
    XSetWindowAttributes wa = {}; wa.colormap = map;
    Window w = XCreateWindow(d,RootWindow(d,v->screen),0,0,16,16,0,v->depth,InputOutput,v->visual,CWColormap,&wa);
    if (!glXMakeCurrent(d,w,c)) { glXDestroyContext(d,c); XDestroyWindow(d,w); XFreeColormap(d,map); XFree(v); XCloseDisplay(d); return 77; }
    const cc_glglue * glue = cc_glglue_instance(941);
    int units = cc_glglue_max_texture_units(glue);
    std::printf("GL renderer: %s; texture units: %d\n",glGetString(GL_RENDERER),units);
    int major = 0, minor = 0;
    const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (version) std::sscanf(version, "%d.%d", &major, &minor);
    const char * extensions = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    const std::string ext = std::string(" ") + (extensions ? extensions : "") + " ";
    const bool reflection = major > 1 || (major == 1 && minor >= 3) ||
      ext.find(" GL_ARB_texture_cube_map ") != std::string::npos ||
      ext.find(" GL_EXT_texture_cube_map ") != std::string::npos;
    if (!reflection) std::puts("OMIT: Reflection GL predecessor requires OpenGL 1.3 or texture_cube_map support");
    if (filter == "limit" || filter == "all") limit(units);
    if (filter != "limit") transitions(true, units > 1 ? 2 : 1, filter, reflection);
    SoContextHandler::destructingContext(941);
    glXMakeCurrent(d,None,NULL); glXDestroyContext(d,c); XDestroyWindow(d,w);
    XFreeColormap(d,map); XFree(v); XCloseDisplay(d);
  }
  std::printf("%d checks, %d failures\n",checks,failures);
  SoDB::finish(); return failures ? 1 : 0;
}
