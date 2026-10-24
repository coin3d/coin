// Public native-GL regression tests: no CoinRender, private headers or shader seams.
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/annex/FXViz/nodes/SoShadowGroup.h>
#include <Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h>
#include <Inventor/annex/FXViz/nodes/SoShadowSpotLight.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {
const int size = 128;
int failures = 0;
std::string label;
void check(bool value, const char * message) {
  if (!value) { ++failures; std::fprintf(stderr, "FAIL [%s]: %s\n", label.c_str(), message); }
}
struct Result {
  std::vector<unsigned char> image;
  std::string vertex, fragment;
  int linked, callbacks, errors;
  Result() : image(size * size * 3), linked(0), callbacks(0), errors(0) {}
  double patch(float x, float y = 0) const {
    const int px = int((x / 8.0f + 0.5f) * size);
    const int py = int((y / 8.0f + 0.5f) * size);
    double sum = 0;
    for (int j = -2; j <= 2; ++j) for (int i = -2; i <= 2; ++i)
      sum += image[((py+j)*size+px+i)*3];
    return sum / 25.0;
  }
};
void error(const SoError * e, void * data) {
  Result * r = static_cast<Result *>(data);
  const SoDebugError * d = static_cast<const SoDebugError *>(e);
  if (d->getSeverity() == SoDebugError::ERROR ||
      std::strstr(e->getDebugString().getString(), "shader") ||
      std::strstr(e->getDebugString().getString(), "Shader")) {
    ++r->errors;
    std::fprintf(stderr, "[%s] %s\n", label.c_str(), e->getDebugString().getString());
  }
}
void inspect(void * data, SoAction * action) {
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  Result & r = *static_cast<Result *>(data);
  ++r.callbacks;
  GLint p = 0; glGetIntegerv(GL_CURRENT_PROGRAM, &p);
  if (!p) return;
  GLint linked = 0; glGetProgramiv(GLuint(p), GL_LINK_STATUS, &linked);
  if (!linked) return;
  ++r.linked;
  GLuint shaders[8]; GLsizei count = 0;
  glGetAttachedShaders(GLuint(p), 8, &count, shaders);
  for (int i = 0; i < count; ++i) {
    GLint type = 0, length = 0, compiled = 0;
    glGetShaderiv(shaders[i], GL_SHADER_TYPE, &type);
    glGetShaderiv(shaders[i], GL_SHADER_SOURCE_LENGTH, &length);
    glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &compiled);
    check(compiled != 0, "attached shader must have compiled");
    if (length > 0) {
      std::vector<char> source(length);
      glGetShaderSource(shaders[i], length, NULL, &source[0]);
      (type == GL_VERTEX_SHADER ? r.vertex : r.fragment) = &source[0];
    }
  }
}
struct Options {
  float quality;
  int directional, spots, inherited, textures;
  bool blocker, ordinary, reverse;
  float offset, cameraDistance;
  Options(float q) : quality(q), directional(1), spots(0), inherited(0), textures(0),
    blocker(false), ordinary(false), reverse(false), offset(0), cameraDistance(8) {}
};
SoSeparator * mapScene(bool blocker, float offset) {
  SoSeparator * scene = new SoSeparator;
  SoTranslation * translation = new SoTranslation;
  // An absent blocker is moved outside the explicit shadow bounds, preserving
  // the producer's nonempty geometry without putting anything over the receiver.
  translation->translation.setValue(blocker ? -1.5f + offset : 30.0f, 0, 1);
  scene->addChild(translation);
  SoCube * cube = new SoCube;
  cube->width = 1.6f; cube->height = 2; cube->depth = 0.6f;
  scene->addChild(cube);
  return scene;
}
Result render(const Options & o, bool requireProgram = true) {
  Result r;
  SoSeparator * root = new SoSeparator; root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(o.offset, 0, o.cameraDistance);
  camera->height = 8; camera->nearDistance = 1; camera->farDistance = 40;
  root->addChild(camera);
  SoEnvironment * environment = new SoEnvironment;
  environment->ambientIntensity = 0; root->addChild(environment);
  for (int i = 0; i < o.inherited; ++i) {
    SoDirectionalLight * light = new SoDirectionalLight;
    light->intensity = 0; root->addChild(light);
  }
  SoShadowGroup * group = new SoShadowGroup;
  group->quality = o.quality; group->precision = 0.2f;
  group->renderCaching = SoSeparator::OFF;
  group->epsilon = 0.00001f; group->threshold = 0.1f;
  root->addChild(group);
  if (o.ordinary) {
    SoDirectionalLight * light = new SoDirectionalLight;
    light->direction.setValue(0, 0, o.reverse ? 1 : -1);
    light->intensity = 0.7f; group->addChild(light);
  }
  for (int i = 0; i < o.directional; ++i) {
    SoShadowDirectionalLight * light = new SoShadowDirectionalLight;
    light->direction.setValue(0, 0, o.reverse ? 1 : -1);
    light->intensity = o.ordinary || o.spots ? 0.35f : 0.7f;
    light->bboxCenter.setValue(o.offset, 0, 0);
    light->bboxSize.setValue(8, 8, 6);
    light->shadowMapScene = mapScene(o.blocker, o.offset);
    group->addChild(light);
  }
  for (int i = 0; i < o.spots; ++i) {
    SoShadowSpotLight * light = new SoShadowSpotLight;
    light->location.setValue(o.offset, 0, 6);
    light->direction.setValue(0, 0, -1);
    light->cutOffAngle = 0.75f; light->dropOffRate = 0;
    light->intensity = o.directional ? 0.35f : 0.7f;
    light->shadowMapScene = mapScene(o.blocker, o.offset);
    group->addChild(light);
  }
  SoMaterial * material = new SoMaterial;
  material->ambientColor.setValue(0,0,0); material->specularColor.setValue(0,0,0);
  material->diffuseColor.setValue(1,1,1); group->addChild(material);
  for (int i = 0; i < o.textures; ++i) {
    SoTextureUnit * unit = new SoTextureUnit; unit->unit = i; group->addChild(unit);
    SoTexture2 * texture = new SoTexture2;
    const unsigned char white[] = {255,255,255,255,255,255,255,255,255,255,255,255};
    texture->image.setValue(SbVec2s(2,2), 3, white);
    group->addChild(texture);
  }
  SoNormal * normal = new SoNormal; normal->vector.setValue(0,0,1); group->addChild(normal);
  SoNormalBinding * binding = new SoNormalBinding;
  binding->value = SoNormalBinding::OVERALL; group->addChild(binding);
  SoCoordinate3 * coordinates = new SoCoordinate3;
  const SbVec3f points[] = {SbVec3f(-3.8f+o.offset,-3.8f,0), SbVec3f(3.8f+o.offset,-3.8f,0),
    SbVec3f(3.8f+o.offset,3.8f,0), SbVec3f(-3.8f+o.offset,3.8f,0)};
  coordinates->point.setValues(0,4,points); group->addChild(coordinates);
  SoCallback * callback = new SoCallback; callback->setCallback(inspect, &r); group->addChild(callback);
  SoFaceSet * face = new SoFaceSet; face->numVertices.setValue(4); group->addChild(face);
  SoErrorCB * old = SoDebugError::getHandlerCallback(); void * olddata = SoDebugError::getHandlerData();
  SoDebugError::setHandlerCallback(error, &r);
  {
    SoGLRenderAction action(SbViewportRegion(size,size));
    action.setCacheContext(5391);
    glViewport(0,0,size,size); glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    action.apply(root); glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(0,0,size,size,GL_RGB,GL_UNSIGNED_BYTE,&r.image[0]);
  }
  SoDebugError::setHandlerCallback(old, olddata);
  check(r.errors == 0, "no shader compilation/link diagnostics");
  check(r.callbacks > 0, "receiver callback reached");
  if (requireProgram) {
    check(r.linked > 0, "receiver must execute with an actually linked GLSL program");
    check(!r.vertex.empty() && !r.fragment.empty(), "both actual shader stages are available");
  }
  check(glGetError() == GL_NO_ERROR, "render/readback has no GL error");
  std::printf("[%s] q=%.9g linked=%d left=%.1f right=%.1f\n", label.c_str(),
              o.quality, r.linked, r.patch(-1.5f), r.patch(1.5f));
  root->unref();
  return r;
}
void lighting(float q) {
  label = "directional lighting, empty producer";
  Options o(q); Result lit = render(o);
  check(lit.patch(-1.5f) > 130 && lit.patch(1.5f) > 130,
        "front-facing diffuse receiver must receive directional illumination");
  check(std::fabs(lit.patch(-1.5f)-lit.patch(1.5f)) < 8,
        "directional diffuse illumination is independent of lateral position");
  label = "back-facing directional control"; o.reverse = true; Result dark = render(o);
  check(dark.patch(1.5f) < 12, "reversing the light must remove diffuse illumination");
  label = "ordinary directional control"; o.reverse = false; o.directional = 0; o.ordinary = true;
  Result ordinary = render(o, false);
  check(std::fabs(ordinary.patch(1.5f)-lit.patch(1.5f)) < 12,
        "a shadow directional light has the same unoccluded diffuse response as an ordinary directional light");
}
void metric(float q) {
  label = "directional VSM blocker"; Options o(q); o.blocker = true;
  Result shadow = render(o);
  check(shadow.patch(1.5f) > 130, "unoccluded receiver remains lit");
  check(shadow.patch(-1.5f) + 65 < shadow.patch(1.5f),
        "a separate elevated caster casts a measurable shadow on its planar receiver");
  // Translating everything laterally and moving the eye along the light axis
  // leaves directional illumination and depth ordering unchanged. Radial
  // receiver depth is particularly wrong for this adversarial configuration.
  label = "directional VSM translated/deep-eye control";
  o.offset = 11; o.cameraDistance = 17; Result translated = render(o);
  check(translated.patch(1.5f) > 130, "translated unoccluded receiver remains lit");
  check(translated.patch(-1.5f)+65 < translated.patch(1.5f), "translated caster still occludes");
  check(std::fabs(translated.patch(1.5f)-shadow.patch(1.5f)) < 12,
        "directional lit response survives lateral and eye-depth translation");
}
void matrix() {
  const float qs[] = {0,0.5f,1};
  for (int q = 0; q < 3; ++q) for (int kind = 0; kind < 5; ++kind) for (int tex = 0; tex <= 2; ++tex) {
    Options o(qs[q]); o.textures = tex; o.inherited = 2;
    o.directional = kind == 0 || kind == 2 || kind == 3 ? 1 : 0;
    o.spots = kind == 1 || kind == 2 ? 1 : 0;
    o.ordinary = kind == 3 || kind == 4;
    label = std::string("link/index/texture matrix kind=") + char('0'+kind) + " tex=" + char('0'+tex);
    Result r = render(o, o.directional || o.spots);
    check(r.patch(1.5f) > 50, "matrix has a visible diffuse receiver");
  }
  label = "zero lights and zero maps"; Options empty(0.5f); empty.directional = 0;
  render(empty, false);
}
void boundaries() {
  // Preserve float-field versus double-literal comparisons: float(.3) is
  // ABOVE .3, while float(.7) is BELOW .7. Check actual generated, compiled
  // stage equivalence rather than rewriting these comparisons as float ones.
  const float a = 0.3f, b = 0.7f;
  const float qs[] = {0, std::nextafter(a,0.0f), a, std::nextafter(a,1.0f),
    0.5f, std::nextafter(b,0.0f), b, std::nextafter(b,1.0f),1};
  std::vector<Result> results;
  for (unsigned i = 0; i < sizeof(qs)/sizeof(qs[0]); ++i) {
    label = "quality boundary compiled stages"; results.push_back(render(Options(qs[i])));
  }
  const int representative[] = {0,0,4,4,4,4,4,8,8};
  for (int i = 0; i < 9; ++i) {
    const Result & expected = results[representative[i]];
    check(results[i].vertex == expected.vertex && results[i].fragment == expected.fragment,
          "representable neighbor must retain its established shader-quality branch");
  }
  check(results[0].vertex != results[4].vertex || results[0].fragment != results[4].fragment,
        "low and intermediate branches are distinct");
  check(results[4].vertex != results[8].vertex || results[4].fragment != results[8].fragment,
        "intermediate and high branches are distinct");
}
}
int main(int argc, char ** argv) {
  Display * display = XOpenDisplay(NULL);
  if (!display) { std::puts("SKIP: no X display for native shadow GLX tests"); return 77; }
  int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE,24,None};
  XVisualInfo * visual = glXChooseVisual(display,DefaultScreen(display),attributes);
  if (!visual) { XCloseDisplay(display); std::puts("SKIP: no GLX visual"); return 77; }
  GLXContext context = glXCreateContext(display,visual,NULL,True);
  if (!context) { XFree(visual); XCloseDisplay(display); return 77; }
  Colormap colormap = XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
  XSetWindowAttributes attr = {}; attr.colormap = colormap;
  Window window = XCreateWindow(display,RootWindow(display,visual->screen),0,0,size,size,0,
    visual->depth,InputOutput,visual->visual,CWColormap,&attr);
  if (!glXMakeCurrent(display,window,context)) return 77;
  SoDB::init();
  GLint units = 0; glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&units);
  if (!glGetString(GL_SHADING_LANGUAGE_VERSION) || units < 4 || !SoShadowGroup::isSupported()) {
    std::puts("SKIP: native GLSL/VSM shadow support unavailable"); return 77;
  }
  std::printf("GL vendor=%s renderer=%s version=%s\n", glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_VERSION));
  const std::string mode = argc > 1 ? argv[1] : "all";
  if (mode == "all" || mode == "low") { lighting(0); metric(0); }
  if (mode == "all" || mode == "medium") { lighting(0.5f); metric(0.5f); }
  if (mode == "all" || mode == "high") { lighting(1); metric(1); }
  if (mode == "all" || mode == "matrix") matrix();
  if (mode == "all" || mode == "boundaries") boundaries();
  SoDB::finish();
  glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap); XFree(visual); XCloseDisplay(display);
  std::printf("Native shadow checks: %d failure(s)\n",failures);
  return failures ? 1 : 0;
}
