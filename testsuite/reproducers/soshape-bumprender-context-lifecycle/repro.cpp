/*
  Regression reproducer for soshape_bumprender's per-context ARB program
  selection and lifetime.

  One SoIndexedFaceSet is rendered in two real, non-shared GLX contexts with
  distinct Coin cache-context IDs. Before B renders, A's numeric program names
  are deliberately materialized as empty programs in B. Reusing A's GLuints in
  B is therefore deterministic: the binds succeed, but program length is zero.
*/

#include "../CoinCleanup.h"

#include <cstdio>
#include <cstring>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoBumpMap.h>
#include <Inventor/nodes/SoBumpMapCoordinate.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>

namespace {

const int WIDTH = 96;
const int HEIGHT = 96;

struct ArbPrograms {
  PFNGLBINDPROGRAMARBPROC bind;
  PFNGLGETPROGRAMIVARBPROC get;
  PFNGLISPROGRAMARBPROC is;

  ArbPrograms() : bind(NULL), get(NULL), is(NULL) {}

  bool load()
  {
    this->bind = reinterpret_cast<PFNGLBINDPROGRAMARBPROC>(
      glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glBindProgramARB")));
    this->get = reinterpret_cast<PFNGLGETPROGRAMIVARBPROC>(
      glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glGetProgramivARB")));
    this->is = reinterpret_cast<PFNGLISPROGRAMARBPROC>(
      glXGetProcAddressARB(reinterpret_cast<const GLubyte *>("glIsProgramARB")));
    return this->bind && this->get && this->is;
  }
};

struct ProgramSet {
  GLuint fragment;
  GLuint directional;
  GLuint point;

  ProgramSet() : fragment(0), directional(0), point(0) {}
};

struct NativeContext {
  Window window;
  Colormap colormap;
  GLXContext context;
  uint32_t cacheid;
  bool notified;

  NativeContext()
    : window(0), colormap(0), context(NULL), cacheid(0), notified(false) {}
};

class GlxFixture {
public:
  GlxFixture() : display(NULL), visual(NULL) {}

  ~GlxFixture()
  {
    this->destroy(0);
    this->destroy(1);
    if (this->visual) XFree(this->visual);
    if (this->display) XCloseDisplay(this->display);
  }

  bool create()
  {
    this->display = XOpenDisplay(NULL);
    if (!this->display) return false;

    int attributes[] = {
      GLX_RGBA,
      GLX_RED_SIZE, 8,
      GLX_GREEN_SIZE, 8,
      GLX_BLUE_SIZE, 8,
      GLX_DEPTH_SIZE, 16,
      GLX_DOUBLEBUFFER,
      None
    };
    this->visual = glXChooseVisual(this->display,
                                   DefaultScreen(this->display), attributes);
    if (!this->visual) return false;
    return this->createOne(0, "Coin bump context A", 0) &&
           this->createOne(1, "Coin bump context B", WIDTH + 8);
  }

  bool makeCurrent(const int index)
  {
    NativeContext & item = this->items[index];
    if (!item.context || !item.window) return false;
    if (!glXMakeCurrent(this->display, item.window, item.context)) return false;
    glViewport(0, 0, WIDTH, HEIGHT);
    return true;
  }

  void notify(const int index)
  {
    NativeContext & item = this->items[index];
    if (item.context && item.cacheid && !item.notified && this->makeCurrent(index)) {
      SoContextHandler::destructingContext(item.cacheid);
      item.notified = true;
    }
  }

  void destroy(const int index)
  {
    NativeContext & item = this->items[index];
    if (!item.context) return;
    this->notify(index);
    glXMakeCurrent(this->display, None, NULL);
    glXDestroyContext(this->display, item.context);
    item.context = NULL;
    if (item.window) {
      XDestroyWindow(this->display, item.window);
      item.window = 0;
    }
    if (item.colormap) {
      XFreeColormap(this->display, item.colormap);
      item.colormap = 0;
    }
  }

  Display * display;
  XVisualInfo * visual;
  NativeContext items[2];

private:
  bool createOne(const int index, const char * title, const int x)
  {
    NativeContext & item = this->items[index];
    item.colormap = XCreateColormap(this->display,
                                    RootWindow(this->display, this->visual->screen),
                                    this->visual->visual, AllocNone);
    XSetWindowAttributes attributes;
    std::memset(&attributes, 0, sizeof(attributes));
    attributes.colormap = item.colormap;
    attributes.border_pixel = 0;
    attributes.event_mask = StructureNotifyMask;
    item.window = XCreateWindow(this->display,
                                RootWindow(this->display, this->visual->screen),
                                x, 0, WIDTH, HEIGHT, 0,
                                this->visual->depth, InputOutput,
                                this->visual->visual,
                                CWBorderPixel | CWColormap | CWEventMask,
                                &attributes);
    if (!item.window) return false;
    XStoreName(this->display, item.window, title);

    // A NULL share-list is essential: A and B must have incompatible object
    // namespaces for the regression to be meaningful.
    item.context = glXCreateContext(this->display, this->visual, NULL, True);
    if (!item.context) return false;
    XMapWindow(this->display, item.window);
    XSync(this->display, False);
    item.cacheid = SoGLCacheContextElement::getUniqueCacheContext();
    return true;
  }
};

class SceneFixture {
public:
  SceneFixture() : directional(NULL), point(NULL) {}
  ~SceneFixture() { this->destroy(); }

  void create()
  {
    SoSeparator * geometry = new SoSeparator;
    geometry->renderCaching = SoSeparator::OFF;

    SoMaterial * material = new SoMaterial;
    material->diffuseColor.setValue(0.35f, 0.35f, 0.35f);
    material->specularColor.setValue(1.0f, 1.0f, 1.0f);
    material->shininess = 0.7f;
    geometry->addChild(material);

    SoBumpMap * bump = new SoBumpMap;
    static const unsigned char pixels[] = {
      128, 128, 255, 128, 128, 255,
      128, 128, 255, 128, 128, 255
    };
    bump->image.setValue(SbVec2s(2, 2), 3, pixels);
    geometry->addChild(bump);

    SoShapeHints * hints = new SoShapeHints;
    hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    geometry->addChild(hints);

    SoCoordinate3 * coordinates = new SoCoordinate3;
    static const SbVec3f vertices[] = {
      SbVec3f(-1.0f, -1.0f, 0.0f), SbVec3f(1.0f, -1.0f, 0.0f),
      SbVec3f( 1.0f,  1.0f, 0.0f), SbVec3f(-1.0f, 1.0f, 0.0f)
    };
    coordinates->point.setValues(0, 4, vertices);
    geometry->addChild(coordinates);

    SoNormal * normals = new SoNormal;
    static const SbVec3f normalvalues[] = {
      SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(0.0f, 0.0f, 1.0f),
      SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(0.0f, 0.0f, 1.0f)
    };
    normals->vector.setValues(0, 4, normalvalues);
    geometry->addChild(normals);

    SoNormalBinding * binding = new SoNormalBinding;
    binding->value = SoNormalBinding::PER_VERTEX_INDEXED;
    geometry->addChild(binding);

    SoBumpMapCoordinate * bumpcoordinates = new SoBumpMapCoordinate;
    static const SbVec2f texcoords[] = {
      SbVec2f(0.0f, 0.0f), SbVec2f(1.0f, 0.0f),
      SbVec2f(1.0f, 1.0f), SbVec2f(0.0f, 1.0f)
    };
    bumpcoordinates->point.setValues(0, 4, texcoords);
    geometry->addChild(bumpcoordinates);

    SoIndexedFaceSet * shape = new SoIndexedFaceSet;
    static const int32_t indices[] = { 0, 1, 2, 3, -1 };
    shape->coordIndex.setValues(0, 5, indices);
    shape->normalIndex.setValues(0, 5, indices);
    geometry->addChild(shape);

    this->directional = makeRoot(geometry, false);
    this->point = makeRoot(geometry, true);
    this->directional->ref();
    this->point->ref();
  }

  void destroy()
  {
    if (this->directional) {
      this->directional->unref();
      this->directional = NULL;
    }
    if (this->point) {
      this->point->unref();
      this->point = NULL;
    }
  }

  SoSeparator * directional;
  SoSeparator * point;

private:
  static SoSeparator * makeRoot(SoSeparator * geometry, const bool usepoint)
  {
    SoSeparator * root = new SoSeparator;
    root->renderCaching = SoSeparator::OFF;
    SoPerspectiveCamera * camera = new SoPerspectiveCamera;
    camera->position.setValue(0.0f, 0.0f, 5.0f);
    camera->nearDistance = 1.0f;
    camera->farDistance = 10.0f;
    camera->heightAngle = 0.8f;
    root->addChild(camera);
    if (usepoint) {
      SoPointLight * light = new SoPointLight;
      light->location.setValue(1.0f, 1.0f, 3.0f);
      root->addChild(light);
    }
    else {
      SoDirectionalLight * light = new SoDirectionalLight;
      light->direction.setValue(-0.2f, -0.2f, -1.0f);
      root->addChild(light);
    }
    root->addChild(geometry);
    return root;
  }
};

static void drainErrors()
{
  while (glGetError() != GL_NO_ERROR) { }
}

static bool reportErrors(const char * phase)
{
  bool ok = true;
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::fprintf(stderr, "[repro] %s left GL error 0x%04x\n", phase,
                 static_cast<unsigned int>(error));
    ok = false;
  }
  return ok;
}

static GLint programLength(const ArbPrograms & gl, const GLenum target,
                           const GLuint name)
{
  if (!name || !gl.is(name)) return -1;
  gl.bind(target, name);
  GLint length = -1;
  gl.get(target, GL_PROGRAM_LENGTH_ARB, &length);
  return length;
}

static bool loaded(const ArbPrograms & gl, const GLenum target,
                   const GLuint name, const char * label)
{
  const GLint length = programLength(gl, target, name);
  std::fprintf(stderr, "[repro] %-27s name=%u length=%d\n", label,
               static_cast<unsigned int>(name), static_cast<int>(length));
  return name != 0 && length > 0;
}

static bool deleted(const ArbPrograms & gl, const GLuint name,
                    const char * label)
{
  const bool result = name && gl.is(name) == GL_FALSE;
  std::fprintf(stderr, "[repro] %-27s name=%u %s\n", label,
               static_cast<unsigned int>(name),
               result ? "deleted" : "still live");
  return result;
}

static bool render(GlxFixture & fixture, const int context,
                   SoGLRenderAction & action, SoNode * root,
                   const char * phase)
{
  if (!fixture.makeCurrent(context)) {
    std::fprintf(stderr, "[repro] %s: glXMakeCurrent failed\n", phase);
    return false;
  }
  drainErrors();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  action.apply(root);
  glFinish();
  return reportErrors(phase);
}

static bool captureDirectional(const ArbPrograms & gl, ProgramSet & set,
                               const char * prefix)
{
  GLint fragment = 0;
  GLint vertex = 0;
  gl.get(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &fragment);
  gl.get(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &vertex);
  set.fragment = static_cast<GLuint>(fragment);
  set.directional = static_cast<GLuint>(vertex);
  char flabel[80];
  char vlabel[80];
  std::snprintf(flabel, sizeof(flabel), "%s fragment", prefix);
  std::snprintf(vlabel, sizeof(vlabel), "%s directional", prefix);
  const bool fragmentloaded =
    loaded(gl, GL_FRAGMENT_PROGRAM_ARB, set.fragment, flabel);
  const bool vertexloaded =
    loaded(gl, GL_VERTEX_PROGRAM_ARB, set.directional, vlabel);
  return fragmentloaded && vertexloaded;
}

static bool capturePoint(const ArbPrograms & gl, ProgramSet & set,
                         const char * prefix)
{
  GLint fragment = 0;
  GLint vertex = 0;
  gl.get(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &fragment);
  gl.get(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_BINDING_ARB, &vertex);
  set.point = static_cast<GLuint>(vertex);
  char label[80];
  std::snprintf(label, sizeof(label), "%s point", prefix);
  const bool fragmentmatches = static_cast<GLuint>(fragment) == set.fragment;
  const bool vertexloaded = loaded(gl, GL_VERTEX_PROGRAM_ARB, set.point, label);
  return fragmentmatches && vertexloaded;
}

static void materializeEmptyPrograms(const ArbPrograms & gl,
                                     const ProgramSet & names)
{
  gl.bind(GL_FRAGMENT_PROGRAM_ARB, names.fragment);
  gl.bind(GL_VERTEX_PROGRAM_ARB, names.directional);
  gl.bind(GL_VERTEX_PROGRAM_ARB, names.point);
  gl.bind(GL_FRAGMENT_PROGRAM_ARB, 0);
  gl.bind(GL_VERTEX_PROGRAM_ARB, 0);
}

static bool supportsTest(const uint32_t cacheid)
{
  const cc_glglue * glue = cc_glglue_instance(static_cast<int>(cacheid));
  return SoGLDriverDatabase::isSupported(glue, SO_GL_BUMPMAPPING) &&
         SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_VERTEX_PROGRAM) &&
         SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_FRAGMENT_PROGRAM);
}

} // namespace

int main()
{
  SoDB::init();
  CoinReproducerCleanup coincleanup;
  GlxFixture fixture;
  if (!fixture.create()) {
    std::fprintf(stderr, "[repro] INCONCLUSIVE: could not create two GLX contexts\n");
    return 2;
  }

  ArbPrograms gl;
  if (!fixture.makeCurrent(0) || !gl.load() ||
      !supportsTest(fixture.items[0].cacheid) ||
      !fixture.makeCurrent(1) || !supportsTest(fixture.items[1].cacheid)) {
    std::fprintf(stderr,
                 "[repro] INCONCLUSIVE: driver lacks the compatibility "
                 "ARB program/bump-mapping path\n");
    return 2;
  }

  std::fprintf(stderr, "[repro] A cache=%u direct=%d; B cache=%u direct=%d\n",
               fixture.items[0].cacheid,
               glXIsDirect(fixture.display, fixture.items[0].context),
               fixture.items[1].cacheid,
               glXIsDirect(fixture.display, fixture.items[1].context));

  SceneFixture scene;
  scene.create();
  const SbViewportRegion viewport(WIDTH, HEIGHT);
  SoGLRenderAction actionA(viewport);
  SoGLRenderAction actionB(viewport);
  actionA.setCacheContext(fixture.items[0].cacheid);
  actionB.setCacheContext(fixture.items[1].cacheid);

  int failures = 0;
  ProgramSet programsA;
  ProgramSet programsB;
  bool renderok = render(fixture, 0, actionA, scene.directional, "A directional");
  bool captureok = captureDirectional(gl, programsA, "A");
  if (!renderok || !captureok) ++failures;
  renderok = render(fixture, 0, actionA, scene.point, "A point");
  captureok = capturePoint(gl, programsA, "A");
  if (!renderok || !captureok) ++failures;

  fixture.makeCurrent(1);
  drainErrors();
  materializeEmptyPrograms(gl, programsA);
  if (!reportErrors("B dummy setup")) ++failures;
  renderok = render(fixture, 1, actionB, scene.directional, "B directional");
  captureok = captureDirectional(gl, programsB, "B");
  if (!renderok || !captureok) ++failures;
  renderok = render(fixture, 1, actionB, scene.point, "B point");
  captureok = capturePoint(gl, programsB, "B");
  if (!renderok || !captureok) ++failures;
  if (programsB.fragment == programsA.fragment ||
      programsB.directional == programsA.directional ||
      programsB.point == programsA.point) {
    std::fprintf(stderr,
                 "[repro] FAIL: B reused names from A's incompatible namespace\n");
    ++failures;
  }

  ProgramSet repeatA;
  renderok = render(fixture, 0, actionA, scene.directional,
                    "A repeat directional");
  captureok = captureDirectional(gl, repeatA, "A repeat");
  if (!renderok || !captureok) ++failures;
  renderok = render(fixture, 0, actionA, scene.point, "A repeat point");
  captureok = capturePoint(gl, repeatA, "A repeat");
  if (!renderok || !captureok) ++failures;
  if (repeatA.fragment != programsA.fragment ||
      repeatA.directional != programsA.directional ||
      repeatA.point != programsA.point) {
    std::fprintf(stderr, "[repro] FAIL: A did not reuse its complete set\n");
    ++failures;
  }

  // Context first: A is current and unbound when Coin receives notification.
  fixture.makeCurrent(0);
  gl.bind(GL_FRAGMENT_PROGRAM_ARB, 0);
  gl.bind(GL_VERTEX_PROGRAM_ARB, 0);
  fixture.notify(0);
  const bool afragmentdeleted =
    deleted(gl, programsA.fragment, "A fragment context-first");
  const bool adirectionaldeleted =
    deleted(gl, programsA.directional, "A directional context-first");
  const bool apointdeleted =
    deleted(gl, programsA.point, "A point context-first");
  if (!afragmentdeleted || !adirectionaldeleted || !apointdeleted) ++failures;
  fixture.destroy(0);

  ProgramSet survivingB;
  renderok = render(fixture, 1, actionB, scene.directional,
                    "B after A destruction");
  captureok = captureDirectional(gl, survivingB, "B survivor");
  if (!renderok || !captureok) ++failures;
  if (survivingB.fragment != programsB.fragment ||
      survivingB.directional != programsB.directional) {
    std::fprintf(stderr, "[repro] FAIL: B's set changed when A died\n");
    ++failures;
  }

  // Owner first: deleting the shape schedules B's names; re-entering B through
  // an action drains SoGLCacheContextElement's delete queue.
  fixture.makeCurrent(1);
  gl.bind(GL_FRAGMENT_PROGRAM_ARB, 0);
  gl.bind(GL_VERTEX_PROGRAM_ARB, 0);
  scene.destroy();
  if (!gl.is(programsB.fragment) || !gl.is(programsB.directional) ||
      !gl.is(programsB.point)) {
    std::fprintf(stderr, "[repro] FAIL: owner-first deletion was not deferred\n");
    ++failures;
  }
  SoSeparator * empty = new SoSeparator;
  empty->ref();
  empty->renderCaching = SoSeparator::OFF;
  if (!render(fixture, 1, actionB, empty, "B scheduled-delete drain")) ++failures;
  empty->unref();
  const bool bfragmentdeleted =
    deleted(gl, programsB.fragment, "B fragment owner-first");
  const bool bdirectionaldeleted =
    deleted(gl, programsB.directional, "B directional owner-first");
  const bool bpointdeleted =
    deleted(gl, programsB.point, "B point owner-first");
  if (!bfragmentdeleted || !bdirectionaldeleted || !bpointdeleted) ++failures;

  fixture.notify(1);
  fixture.destroy(1);
  if (failures) {
    std::fprintf(stderr, "[repro] FAIL: %d contract check(s) failed\n", failures);
    return 1;
  }
  std::fprintf(stderr, "[repro] PASS: A -> B -> A and both lifetimes verified\n");
  return 0;
}
