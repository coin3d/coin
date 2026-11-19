// Public CPU texgen callbacks, evaluated at shape traversal points.
// The oracle uses explicit point/dual maps and double arithmetic, without
// SbMatrix multiplication, inverse, transpose, or SbVec3f normalization.
// Undefined incident directions instead reproduce the upstream Float algorithm.
#ifdef COIN_ENVREF_GLX
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
#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <string>
#include <vector>

namespace {
int checks = 0, failures = 0;
const uint32_t contextid = 979;
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
  for (int i = 0; i < 4; ++i) {
    if(std::isnan(want[i])) ok=ok && std::isnan(got[i]);
    else if(std::isinf(want[i])) ok=ok && std::isinf(got[i]) && std::signbit(got[i])==std::signbit(want[i]);
    else ok=ok && std::isfinite(got[i]) && std::fabs(got[i]-want[i])<2e-5f;
  }
  ++checks;
  if (!ok) {
    ++failures;
    std::fprintf(stderr, "FAIL %s %s: got (%g,%g,%g,%g), want (%g,%g,%g,%g)\n",
      actionname.c_str(), label.c_str(), got[0],got[1],got[2],got[3],
      want[0],want[1],want[2],want[3]);
  }
}
enum Model { IDENTITY, ROTATE_Z, STRETCH, SHEAR, STRETCH_SHEAR, REFLECT, HUGE_SCALE, TINY_SCALE };
enum View { NO_CAMERA, FRONT, CAMERA_Y, CAMERA_Z };
enum Generator { OBJECT, ENVIRONMENT, REFLECTION };
struct Expected {
  Model model;
  View view;
  int unit;
  Generator generator;
  bool translated;
  Expected(Model m, View v, int u, Generator g, bool t = false)
    : model(m),view(v),unit(u),generator(g),translated(t) {}
};
struct Vec {
  double x,y,z;
  Vec(double a,double b,double c) : x(a),y(b),z(c) {}
  void normalize() {
    const double length = std::sqrt(x*x+y*y+z*z);
    if (length != 0) { x/=length; y/=length; z/=length; }
  }
};
SbMatrix modelMatrix(Model model, bool translated);
SbVec4f legacyDegenerate(const SbVec3f & normal,const Expected & e,double * sphereDenominator) {
  SbVec3f wn; modelMatrix(e.model,false).multDirMatrix(normal,wn);
  SbVec3f u=normal; u.normalize(); wn.normalize();
  SbVec3f r=u-SbVec3f(2.0f*wn[0]*wn[0]*u[0],2.0f*wn[1]*wn[1]*u[1],2.0f*wn[2]*wn[2]*u[2]);
  r.normalize();
  if(e.generator==REFLECTION) return SbVec4f(r[0],r[1],r[2],1);
  const float tmp=1.0f+r[2];
  float m=2.0f*float(std::sqrt(r[0]*r[0]+r[1]*r[1]+tmp*tmp));
  if(std::fabs(m)<=FLT_EPSILON) m=1.0f;
  if(sphereDenominator) *sphereDenominator=m;
  return SbVec4f(r[0]/m+.5f,r[1]/m+.5f,0,1);
}
SbVec4f oracle(const SbVec3f & point, const SbVec3f & normal, const Expected & e, double * sphereDenominator = NULL) {
  Vec p(point[0],point[1],point[2]), n(normal[0],normal[1],normal[2]);
  const double px=p.x,py=p.y,nx=n.x,ny=n.y;
  switch (e.model) {
  case ROTATE_Z: p.x=-py; p.y=px; n.x=-ny; n.y=nx; break;
  case STRETCH: p.x*=2; p.y*=3; p.z*=4; n.x/=2; n.y/=3; n.z/=4; break;
  case SHEAR: p.y=2*px+py; n.x=nx-2*ny; break;
  case STRETCH_SHEAR:
    p.x=2*px; p.y=px+3*py; p.z*=4;
    n.x=nx/2-ny/6; n.y=ny/3; n.z/=4; break;
  case REFLECT: p.x*=-2; p.y*=3; p.z*=4; n.x/=-2; n.y/=3; n.z/=4; break;
  case HUGE_SCALE: p.x*=1e20f; p.y*=1e20f; p.z*=1e20f; n.x/=1e20f; n.y/=1e20f; n.z/=1e20f; break;
  case TINY_SCALE: p.x*=1e-20f; p.y*=1e-20f; p.z*=1e-20f; n.x/=1e-20f; n.y/=1e-20f; n.z/=1e-20f; break;
  case IDENTITY: break;
  }
  if (e.translated) { p.x+=7; p.y-=11; p.z+=19; }
  if (e.view != NO_CAMERA) {
    if (e.translated) { p.x-=13; p.y+=17; p.z-=23; }
    else p.z-=8;
  }
  const double mx=p.x,my=p.y,mz=p.z, ax=n.x,ay=n.y,az=n.z;
  if (e.view == CAMERA_Y) { p.x=-mz; p.z=mx; n.x=-az; n.z=ax; }
  if (e.view == CAMERA_Z) { p.x=my; p.y=-mx; n.x=ay; n.y=-ax; }
  if((normal[0]==0 && normal[1]==0 && normal[2]==0) || (p.x==0 && p.y==0 && p.z==0))
    return legacyDegenerate(normal,e,sphereDenominator);
  p.normalize(); n.normalize();
  const double dot=p.x*n.x+p.y*n.y+p.z*n.z;
  const Vec r(p.x-2*dot*n.x,p.y-2*dot*n.y,p.z-2*dot*n.z);
  if (e.generator == REFLECTION) return SbVec4f(float(r.x),float(r.y),float(r.z),1);
  const double m=2*std::sqrt(r.x*r.x+r.y*r.y+(r.z+1)*(r.z+1));
  if (sphereDenominator) *sphereDenominator=m;
  if (m == 0) return SbVec4f(.5f,.5f,0,1);
  return SbVec4f(float(r.x/m+.5),float(r.y/m+.5),0,1);
}
SoNode * generator(Generator g) {
  return g == ENVIRONMENT ? static_cast<SoNode *>(new SoTextureCoordinateEnvironment)
                          : static_cast<SoNode *>(new SoTextureCoordinateReflectionMap);
}
SbMatrix modelMatrix(Model model, bool translated = false) {
  SbMatrix m = SbMatrix::identity();
  switch (model) {
  case ROTATE_Z: m[0][0]=0; m[0][1]=1; m[1][0]=-1; m[1][1]=0; break;
  case STRETCH: m[0][0]=2; m[1][1]=3; m[2][2]=4; break;
  case SHEAR: m[0][1]=2; break;
  case STRETCH_SHEAR: m[0][0]=2; m[0][1]=1; m[1][1]=3; m[2][2]=4; break;
  case REFLECT: m[0][0]=-2; m[1][1]=3; m[2][2]=4; break;
  case HUGE_SCALE: m[0][0]=m[1][1]=m[2][2]=1e20f; break;
  case TINY_SCALE: m[0][0]=m[1][1]=m[2][2]=1e-20f; break;
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
  if (view == NO_CAMERA) return;
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
    const SbVec3f points[] = {
      SbVec3f(0,0,0),SbVec3f(17,-23,31),SbVec3f(-9,4,-7),
      SbVec3f(1,0,0),SbVec3f(0,1,0),SbVec3f(0,0,1),SbVec3f(0,0,-1),
      SbVec3f(1e-7f,0,-1),SbVec3f(0,-1e-7f,-1),
      SbVec3f(2e20f,-3e20f,6e20f),SbVec3f(2e-30f,-3e-30f,6e-30f)
    };
    for (size_t i = 0; i < expected.size(); ++i) {
      const Expected & item = expected[i];
      const std::string where = label + " generator="+std::to_string(item.generator)+" unit=" + std::to_string(item.unit);
      check(e->getType(item.unit) == (gl ? SoMultiTextureCoordinateElement::TEXGEN :
         SoMultiTextureCoordinateElement::FUNCTION),where,"registered coordinate function/type");
      if (item.generator == OBJECT) {
        equal(e->get(item.unit,SbVec3f(1,2,3),SbVec3f(0,0,1)),
              SbVec4f(2.125f,6.25f,12.5f,2),where+" preserved other generator");
      } else {
        for (size_t j = 0; j < sizeof(normals)/sizeof(normals[0]); ++j) {
          for (size_t p = 0; p < sizeof(points)/sizeof(points[0]); ++p) {
            double denominator=0;
            const SbVec4f want = oracle(points[p],normals[j],item,&denominator);
            // A nominal quarter-turn camera uses a rounded quaternion. At
            // the sphere-map pole its tiny angular error is discontinuous.
            // Exercise exact and near poles with the exact identity view;
            // keep rotated-camera comparisons away from that singularity.
            if (item.generator == ENVIRONMENT &&
                (item.view == CAMERA_Y || item.view == CAMERA_Z) && denominator < 1e-5) continue;
            const std::string sample = where+" normal="+std::to_string(j)+" point="+std::to_string(p);
            const SbVec4f got = e->get(item.unit,points[p],normals[j]);
            equal(got,want,sample);
            // Calling another normal first exposes stale result/cached input.
            (void)e->get(item.unit,points[(p+1)%3],SbVec3f(3,7,-2));
            if (expected.size()>1) (void)e->get(expected[(i+1)%expected.size()].unit,points[p],normals[j]);
            equal(e->get(item.unit,points[p],normals[j]),want,sample+" repeated");
          }
        }
      }
#ifdef COIN_ENVREF_GLX
      if (gl) {
        GLint active = 0; glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
        glActiveTexture(GL_TEXTURE0+item.unit);
        const GLenum coords[] = {GL_S,GL_T,GL_R,GL_Q};
        const GLenum enables[] = {GL_TEXTURE_GEN_S,GL_TEXTURE_GEN_T,GL_TEXTURE_GEN_R,GL_TEXTURE_GEN_Q};
        for (int c = 0; c < (item.generator == REFLECTION ? 3 : 4); ++c) {
          GLint mode = 0; glGetTexGeniv(coords[c],GL_TEXTURE_GEN_MODE,&mode);
          check(glIsEnabled(enables[c]) == GL_TRUE,where,"hardware texgen remains enabled");
          check(mode == (item.generator == OBJECT ? GL_OBJECT_LINEAR :
              (item.generator == REFLECTION ? GL_REFLECTION_MAP : (c<2 ? GL_SPHERE_MAP : GL_OBJECT_LINEAR))),where,"hardware texgen mode preserved");
        }
        glActiveTexture(active);
        check(glGetError() == GL_NO_ERROR,where,"no invalid GL commands");
      }
#endif
    }
  }
};
Probe * probe(SoGroup * g,const std::string & label,Model m,View v,int u,
               Generator kind,bool translated=false,int other=-1,Generator otherKind=OBJECT) {
  Probe * p = new Probe; p->label=label;
  p->expected.push_back(Expected(m,v,u,kind,translated));
  if (other>=0) p->expected.push_back(Expected(m,v,other,otherKind,translated));
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
#ifdef COIN_ENVREF_GLX
    else { SoGLRenderAction a(SbViewportRegion(16,16)); a.setCacheContext(contextid); a.apply(root); }
#endif
    for (size_t i = 0; i < probes.size(); ++i)
      check(probes[i]->visits == repeat+1,probes[i]->label,"probe reached on repeated traversal");
  }
  root->unref();
}
void numericalCases(Generator kind) {
  struct Case { const char * name; Model model; View view; bool translated; };
  const Case cases[] = {
    {"identity",IDENTITY,FRONT,false},
    {"huge uniform scale",HUGE_SCALE,NO_CAMERA,false},
    {"tiny uniform scale",TINY_SCALE,NO_CAMERA,false}, {"model rotation",ROTATE_Z,FRONT,false},
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
        if (before) root->addChild(generator(kind));
        transform(root,c.model,c.translated);
        if (!before) root->addChild(generator(kind));
        Probe * p = probe(root,std::string(c.name)+(before?" generator first":" transform first"),
                         c.model,c.view,unit,kind,c.translated,1-unit);
        apply(root,std::vector<Probe *>(1,p));
      }
    }
  }
}
void defaultView(Generator kind) {
  SoGroup * root = new SoGroup; root->addChild(generator(kind));
  Probe * p = probe(root,"default view without camera",IDENTITY,NO_CAMERA,0,kind);
  apply(root,std::vector<Probe *>(1,p));
}
void scopes(Generator kind) {
  for (int unit = 0; unit <= 1; ++unit) {
    SoGroup * root = new SoGroup; camera(root,FRONT);
    select(root,1-unit); object(root); select(root,unit);
    // Reuse this public node in distinct scopes and units. Its callback must
    // remain tied to the current element state, including after Separator pop.
    SoNode * shared = generator(kind);
    root->addChild(shared);
    std::vector<Probe *> probes;
    probes.push_back(probe(root,"outer identity",IDENTITY,FRONT,unit,kind,false,1-unit));
    SoSeparator * sep = new SoSeparator;
    sep->renderCaching=SoSeparator::OFF; sep->renderCulling=SoSeparator::OFF; sep->pickCulling=SoSeparator::OFF;
    root->addChild(sep); camera(sep,CAMERA_Y,true); transform(sep,STRETCH_SHEAR,true);
    probes.push_back(probe(sep,"inherited callback inside separator",STRETCH_SHEAR,CAMERA_Y,unit,kind,true,1-unit));
    // Install the same generator on the other slot inside the separator.
    select(sep,1-unit); sep->addChild(shared);
    probes.push_back(probe(sep,"same node on two slots",STRETCH_SHEAR,CAMERA_Y,unit,kind,true,1-unit,kind));
    probes.push_back(probe(root,"separator restores both slots and matrices",IDENTITY,FRONT,unit,kind,false,1-unit));
    SoGroup * group = new SoGroup; root->addChild(group);
    transform(group,ROTATE_Z); group->addChild(shared);
    probes.push_back(probe(group,"group updated normal matrix",ROTATE_Z,FRONT,unit,kind,false,1-unit));
    probes.push_back(probe(root,"group state persists",ROTATE_Z,FRONT,unit,kind,false,1-unit));
    apply(root,probes);
  }
}
#ifdef COIN_ENVREF_GLX
// Read coordinates produced by the real fixed-function implementation. This
// independent control uses an identity eye matrix and well-conditioned inputs;
// the shape probes above separately verify Coin's modes and CPU callbacks.
void hardwareControl() {
  glActiveTexture(GL_TEXTURE0);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  glMatrixMode(GL_TEXTURE); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  // Supply a complete active texture so fixed-function implementations must
  // evaluate the texture-coordinate pipeline before writing feedback.
  GLuint texture=0;
  glGenTextures(1,&texture); glBindTexture(GL_TEXTURE_2D,texture);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  const GLubyte texel[4]={255,255,255,255};
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,texel);
  glDisable(GL_TEXTURE_CUBE_MAP); glEnable(GL_TEXTURE_2D);
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_GEN_S); glEnable(GL_TEXTURE_GEN_T); glEnable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);
  const SbVec3f points[] = {SbVec3f(.2f,-.3f,-.6f),SbVec3f(-.4f,.1f,.7f)};
  const SbVec3f normals[] = {SbVec3f(1,0,0),SbVec3f(0,0,1),SbVec3f(2,-3,6)};
  for (int k=ENVIRONMENT;k<=REFLECTION;++k) {
    const Generator kind=static_cast<Generator>(k);
    const GLenum mode=kind==ENVIRONMENT ? GL_SPHERE_MAP : GL_REFLECTION_MAP;
    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,mode);
    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,mode);
    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,kind==ENVIRONMENT ? GL_OBJECT_LINEAR : mode);
    for (int p=0;p<2;++p) for (int n=0;n<3;++n) {
      GLfloat feedback[32]={};
      glFeedbackBuffer(32,GL_3D_COLOR_TEXTURE,feedback);
      (void)glRenderMode(GL_FEEDBACK);
      glBegin(GL_POINTS);
      glNormal3fv(normals[n].getValue()); glVertex3fv(points[p].getValue());
      glEnd();
      const GLint count=glRenderMode(GL_RENDER);
      check(count==12 && feedback[0]==GL_POINT_TOKEN,"hardware numerical control","one feedback point");
      const SbVec4f want=oracle(points[p],normals[n],Expected(IDENTITY,NO_CAMERA,0,kind));
      if (count==12) {
        const int dims=kind==ENVIRONMENT ? 2 : 3;
        for (int d=0;d<dims;++d) {
          const bool ok=std::isfinite(feedback[8+d]) && std::fabs(feedback[8+d]-want[d])<2e-5;
          check(ok,"hardware numerical control","GL texgen agrees with independent oracle");
          if (!ok) std::fprintf(stderr,
            "  generator=%d point=%d normal=%d coordinate=%d got=%g want=%g feedback=(%g,%g,%g,%g)\n",
            k,p,n,d,feedback[8+d],want[d],feedback[8],feedback[9],feedback[10],feedback[11]);
        }
      }
      check(glGetError()==GL_NO_ERROR,"hardware numerical control","valid feedback and texgen commands");
    }
  }
  glDisable(GL_TEXTURE_2D); glDeleteTextures(1,&texture);
}
#endif

}
int main(int argc,char ** argv) {
  actionname = argc > 1 ? argv[1] : "callback";
  if (actionname != "callback" && actionname != "pick" && actionname != "gl") return 2;
#ifndef COIN_ENVREF_GLX
  if (actionname == "gl") return 77;
#endif
  SoDB::init();
#ifdef COIN_ENVREF_GLX
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
  for (int k=ENVIRONMENT;k<=REFLECTION;++k) {
    const Generator kind=static_cast<Generator>(k);
    defaultView(kind); numericalCases(kind); scopes(kind);
  }
#ifdef COIN_ENVREF_GLX
  if (display) {
    hardwareControl();
    SoContextHandler::destructingContext(contextid);
    glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
    XDestroyWindow(display,window); XFreeColormap(display,map); XFree(visual); XCloseDisplay(display);
  }
#endif
  SoDB::finish();
  std::printf("%s: %d checks, %d failures\n",actionname.c_str(),checks,failures);
  return failures ? 1 : 0;
}
