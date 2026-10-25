// Native compatibility-GL VSM clipping, checked through public scene nodes.
#define COIN_SHADOW_GLX_FIXTURE_ONLY
#include "DirectionalGLXTest.cpp"
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>

namespace {
const float angle = .4f;
SbVec3f rotatePoint(float x, float y) {
  return SbVec3f(std::cos(angle)*x-std::sin(angle)*y,
                 std::sin(angle)*x+std::cos(angle)*y, 1);
}
void quad(SoSeparator * scene, float x0, float y0, float x1, float y1, float z) {
  SoCoordinate3 * c = new SoCoordinate3;
  const SbVec3f p[] = {SbVec3f(x0,y0,z),SbVec3f(x1,y0,z),
                       SbVec3f(x1,y1,z),SbVec3f(x0,y1,z)};
  c->point.setValues(0,4,p); scene->addChild(c);
  SoFaceSet * f = new SoFaceSet; f->numVertices = 4; scene->addChild(f);
}
struct ClipFixture {
  bool spot;
  float offset;
  SoSeparator * root;
  SoClipPlane * first;
  SoClipPlane * second;
  Result receiver, producer;
  SoGLRenderAction action;
  ClipFixture(bool isspot, bool custom, float quality, bool planes, float translation)
    : spot(isspot), offset(translation), root(new SoSeparator), first(NULL), second(NULL),
      action(SbViewportRegion(size,size)) {
    root->ref(); root->renderCaching = SoSeparator::OFF;
    SoOrthographicCamera * camera = new SoOrthographicCamera;
    // Looking from below hides normal-traversal casters behind the receiver,
    // without changing visibility, transparency or shadow-style traversal.
    camera->position.setValue(offset,0,-9);
    camera->orientation.setValue(SbVec3f(0,1,0),float(std::acos(-1.0)));
    camera->height = 8; camera->nearDistance = 1; camera->farDistance = 40;
    root->addChild(camera);
    SoEnvironment * env = new SoEnvironment; env->ambientIntensity = 0;
    env->attenuation.setValue(0,0,1); root->addChild(env);
    SoShadowGroup * group = new SoShadowGroup;
    group->quality = quality; group->precision = .2f;
    group->epsilon = .00001f; group->threshold = .1f;
    group->renderCaching = SoSeparator::OFF; root->addChild(group);
    SoSeparator * casters = new SoSeparator; casters->renderCaching = SoSeparator::OFF;
    SoTranslation * t = new SoTranslation; t->translation.setValue(offset,0,0);
    casters->addChild(t);
    SoRotation * rotation = new SoRotation;
    rotation->rotation.setValue(SbVec3f(0,0,1),angle); casters->addChild(rotation);
    SoSeparator * clipped = new SoSeparator; clipped->renderCaching = SoSeparator::OFF;
    casters->addChild(clipped);
    if (planes) {
      first = new SoClipPlane; first->plane = SbPlane(SbVec3f(1,0,0),.35f);
      second = new SoClipPlane; second->plane = SbPlane(SbVec3f(0,1,0),.15f);
      first->on = FALSE; second->on = FALSE;
      clipped->addChild(first); clipped->addChild(second);
    }
    SoCallback * cb = new SoCallback; cb->setCallback(inspect,&producer); clipped->addChild(cb);
    // Every active plane intersects this face's bbox. Whole-bbox culling
    // cannot implement the expected partial removal of either half.
    quad(clipped,-2,-1.5f,2,1.5f,1);
    // This sibling is outside BOTH allowed halfspaces and must still cast.
    SoSeparator * sibling = new SoSeparator; casters->addChild(sibling);
    quad(sibling,-1.8f,-2.4f,-.8f,-1.8f,1);
    SbVec3f direction(-.12f,-.08f,-1); direction.normalize();
    if (spot) {
      SoShadowSpotLight * light = new SoShadowSpotLight;
      light->location.setValue(offset+.72f,.48f,6); light->direction = direction;
      light->cutOffAngle = .65f; light->dropOffRate = 0; light->intensity = .7f;
      // Explicit positive near/far keep all sample rays within the physical
      // frustum and the radial VSM epsilon well below the 1-unit depth gap.
      light->nearDistance = .1f; light->farDistance = 12;
      if (custom) light->shadowMapScene = casters;
      group->addChild(light);
    }
    else {
      SoShadowDirectionalLight * light = new SoShadowDirectionalLight;
      light->direction = direction; light->intensity = .7f;
      light->bboxCenter.setValue(offset,0,0); light->bboxSize.setValue(9,9,6);
      if (custom) light->shadowMapScene = casters;
      group->addChild(light);
    }
    if (!custom) group->addChild(casters);
    SoMaterial * m = new SoMaterial; m->ambientColor.setValue(0,0,0);
    m->diffuseColor.setValue(1,1,1); m->specularColor.setValue(0,0,0); group->addChild(m);
    SoNormal * n = new SoNormal; n->vector.setValue(0,0,1); group->addChild(n);
    SoNormalBinding * nb = new SoNormalBinding; nb->value = SoNormalBinding::OVERALL;
    group->addChild(nb);
    SoCallback * rx = new SoCallback; rx->setCallback(inspect,&receiver); group->addChild(rx);
    // Receiver is outside the clipped Separator, remains visible everywhere,
    // and is also an ordinary caster in the normal-traversal test.
    quad(group,offset-3.8f,-3.8f,offset+3.8f,3.8f,0);
    action.setCacheContext(5391);
  }
  ~ClipFixture() { root->unref(); }
  Result frame() {
    receiver = Result(); producer = Result();
    SoErrorCB * old = SoDebugError::getHandlerCallback(); void * data = SoDebugError::getHandlerData();
    SoDebugError::setHandlerCallback(error,&receiver);
    glViewport(0,0,size,size); glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    action.apply(root); glFinish(); glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(0,0,size,size,GL_RGB,GL_UNSIGNED_BYTE,&receiver.image[0]);
    SoDebugError::setHandlerCallback(old,data);
    check(receiver.errors == 0,"no shader compilation/link diagnostics");
    check(receiver.linked > 0 && !receiver.vertex.empty() && !receiver.fragment.empty(),
          "receiver runs linked, compiled vertex and fragment shaders");
    // Producer is visited in both normal and custom scenes. In the latter,
    // these callbacks can only come from the actual moments pass.
    check(producer.linked > 0 && !producer.vertex.empty() && !producer.fragment.empty(),
          "caster executes a linked, compiled GLSL program");
    check(glGetError() == GL_NO_ERROR,"render/readback has no GL error");
    return receiver;
  }
  double sample(const Result & r, float localx, float localy) const {
    SbVec3f p = rotatePoint(localx,localy);
    // Intersect light ray through the z=1 caster with the z=0 receiver.
    const float x = spot ? 1.2f*p[0]-.144f : p[0]-.12f;
    const float y = spot ? 1.2f*p[1]-.096f : p[1]-.08f;
    // Below-facing camera reverses screen X; global translation is canceled
    // by the observer, while still exercising the light's actual view matrix.
    return r.patch(-x,y);
  }
};
void equalSample(double a, double b, const char * message) {
  check(std::fabs(a-b)<14,message);
}
void clipCase(bool spot, bool custom, float q, float offset) {
  label = std::string(spot ? "spot" : "directional") + (custom ? " custom" : " normal") +
    " clipping q=" + std::to_string(q) + " offset=" + std::to_string(offset);
  ClipFixture f(spot,custom,q,true,offset);
  const Result off = f.frame();
  const double lit = off.patch(-2.9f,2.8f);
  check(lit>125,"unoccluded receiver has a physically lit control");
  const float points[][2] = {{1.15f,.85f},{-1.15f,.85f},{1.15f,-.85f},{-1.3f,-2.1f}};
  for (int i=0;i<4;++i)
    check(f.sample(off,points[i][0],points[i][1])+55<lit,
          "on=FALSE control casts at every retained/removed/sibling sample");
  f.first->on = TRUE;
  const Result one = f.frame();
  equalSample(f.sample(one,1.15f,.85f),f.sample(off,1.15f,.85f),"first plane retains positive-side shadow");
  check(f.sample(one,-1.15f,.85f)>lit-20,"first plane removes negative-side shadow, receiver remains drawn");
  equalSample(f.sample(one,-1.3f,-2.1f),f.sample(off,-1.3f,-2.1f),"first plane does not leak into sibling caster");
  f.second->on = TRUE;
  const Result two = f.frame();
  equalSample(f.sample(two,1.15f,.85f),f.sample(off,1.15f,.85f),"two planes retain their intersection shadow");
  check(f.sample(two,1.15f,-.85f)>lit-20,"second plane independently removes its negative-side shadow");
  check(f.sample(two,-1.15f,.85f)>lit-20,"first plane still clips with second active");
  equalSample(f.sample(two,-1.3f,-2.1f),f.sample(off,-1.3f,-2.1f),"multiple planes restore state before sibling");
  // Flip the plane equation on the SAME graph/action, including nonzero
  // distance, to test field notification independently of on/off changes.
  f.second->on = FALSE;
  f.first->plane = SbPlane(SbVec3f(-1,0,0),.35f);
  const Result flipped = f.frame();
  check(f.sample(flipped,1.15f,.85f)>lit-20,"plane notification reverses removed halfspace");
  equalSample(f.sample(flipped,-1.15f,.85f),f.sample(off,-1.15f,.85f),"flipped plane retains opposite shadow");
  f.first->plane = SbPlane(SbVec3f(1,0,0),1.6f);
  const Result shifted = f.frame();
  check(f.sample(shifted,1.15f,.85f)>lit-20,
        "nonzero plane distance moves the boundary across a previously retained sample");
  equalSample(f.sample(shifted,-1.3f,-2.1f),f.sample(off,-1.3f,-2.1f),
              "moving plane distance still restores sibling state");
  f.first->on = FALSE;
  const Result restored = f.frame();
  for (int i=0;i<4;++i)
    equalSample(f.sample(restored,points[i][0],points[i][1]),f.sample(off,points[i][0],points[i][1]),
                "disabled planes restore original shadow on later frame");
  ClipFixture absent(spot,custom,q,false,offset);
  const Result noPlanes = absent.frame();
  for (int i=0;i<4;++i)
    equalSample(absent.sample(noPlanes,points[i][0],points[i][1]),f.sample(off,points[i][0],points[i][1]),
                "on=FALSE matches a graph with no clip nodes");
  std::printf("[%s] lit=%.1f off=(%.1f %.1f %.1f) one=(%.1f %.1f %.1f) two=(%.1f %.1f %.1f) sibling=%.1f\n",
    label.c_str(),lit,f.sample(off,1.15f,.85f),f.sample(off,-1.15f,.85f),f.sample(off,1.15f,-.85f),
    f.sample(one,1.15f,.85f),f.sample(one,-1.15f,.85f),f.sample(one,1.15f,-.85f),
    f.sample(two,1.15f,.85f),f.sample(two,-1.15f,.85f),f.sample(two,1.15f,-.85f),f.sample(two,-1.3f,-2.1f));
}
}

int main(int argc, char ** argv) {
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display for native clipping GLX tests"); return 77; }
  int attributes[] = {GLX_RGBA,GLX_DEPTH_SIZE,24,None};
  XVisualInfo * visual = glXChooseVisual(display,DefaultScreen(display),attributes);
  if (!visual) { XCloseDisplay(display); return 77; }
  GLXContext context = glXCreateContext(display,visual,NULL,True);
  if (!context) { XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
  XSetWindowAttributes attr = {}; attr.colormap = colormap;
  Window window = XCreateWindow(display,RootWindow(display,visual->screen),0,0,size,size,0,
    visual->depth,InputOutput,visual->visual,CWColormap,&attr);
  if (!glXMakeCurrent(display,window,context)) return 77;
  SoDB::init();
  GLint units = 0; glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&units);
  if (!glGetString(GL_SHADING_LANGUAGE_VERSION) || units < 4 || !SoShadowGroup::isSupported()) return 77;
  std::printf("GL vendor=%s renderer=%s version=%s\n",glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_VERSION));
  const std::string mode = argc>1 ? argv[1] : "all";
  for (int spot=0;spot<2;++spot) for (int custom=0;custom<2;++custom) {
    const std::string name = std::string(spot ? "spot" : "directional") + (custom ? "-custom" : "-normal");
    if (mode != "all" && mode != name) continue;
    const float qualities[] = {0,.5f,1};
    for (int q=0;q<3;++q) {
      clipCase(spot,custom,qualities[q],0);
      clipCase(spot,custom,qualities[q],11);
    }
  }
  SoDB::finish(); glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap); XFree(visual); XCloseDisplay(display);
  std::printf("Native VSM clipping checks: %d failure(s)\n",failures);
  return failures ? 1 : 0;
}
