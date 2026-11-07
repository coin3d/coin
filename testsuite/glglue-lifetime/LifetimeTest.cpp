#include <cstdio>
#include <cstring>
#include <thread>
#ifdef COIN_GLGLUE_TEST_CGL
#include <OpenGL/OpenGL.h>
#else
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif
#include <Inventor/SoDB.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoContextHandler.h>
#include "glue/glp.h"
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define COIN_GLGLUE_TEST_ASAN
#endif
#endif
#if defined(__SANITIZE_ADDRESS__) || defined(COIN_GLGLUE_TEST_ASAN)
#include <sanitizer/asan_interface.h>
#define COIN_GLGLUE_CHECK_RELEASED
#endif
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#c); return 1; } } while (0)
// Only the native context setup differs; both backends run the same contracts.
#ifdef COIN_GLGLUE_TEST_CGL
struct NativeContexts {
  CGLContextObj contexts[2];
  NativeContexts() : contexts{} { }
  static const char * backend() { return "CGL"; }
  int initialize() {
    // A legacy context is sufficient: these tests do not render to a drawable.
    // Permit offline renderers on headless CI hosts; do not require acceleration.
    CGLPixelFormatAttribute attrs[] = {
      kCGLPFAAllowOfflineRenderers, kCGLPFAColorSize,
      static_cast<CGLPixelFormatAttribute>(24),
      static_cast<CGLPixelFormatAttribute>(0)
    };
    CGLPixelFormatObj pixel = NULL;
    GLint count = 0;
    CGLError error = CGLChoosePixelFormat(attrs, &pixel, &count);
    if (error != kCGLNoError || !pixel || count == 0) {
      std::fprintf(stderr, "CGLChoosePixelFormat: %s (%d formats)\n",
                   CGLErrorString(error), static_cast<int>(count));
      if (pixel) CGLDestroyPixelFormat(pixel);
      return 1;
    }
    for (CGLContextObj & context : contexts) {
      error = CGLCreateContext(pixel, NULL, &context);
      if (error != kCGLNoError || !context) {
        std::fprintf(stderr, "CGLCreateContext: %s\n", CGLErrorString(error));
        CGLDestroyPixelFormat(pixel);
        return 1;
      }
    }
    CGLDestroyPixelFormat(pixel);
    return 0;
  }
  bool current(int index) { return CGLSetCurrentContext(contexts[index]) == kCGLNoError; }
  bool clear() { return CGLSetCurrentContext(NULL) == kCGLNoError; }
  ~NativeContexts() {
    (void) clear();
    for (CGLContextObj context : contexts) if (context) CGLDestroyContext(context);
  }
};
#else
struct NativeContexts {
  struct Context { GLXContext context; Window window; Colormap colormap; } contexts[2];
  Display * display;
  NativeContexts() : contexts{}, display(NULL) { }
  static const char * backend() { return "GLX"; }
  int initialize() {
    if (!XInitThreads()) return 77;
    display = XOpenDisplay(NULL);
    if (!display) return 77;
    int attrs[] = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, None };
    XVisualInfo * visual = glXChooseVisual(display, DefaultScreen(display), attrs);
    if (!visual) return 77;
    for (Context & context : contexts) {
      context.colormap = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
      XSetWindowAttributes attrs = {};
      attrs.colormap = context.colormap;
      context.window = XCreateWindow(display, RootWindow(display, visual->screen), 0, 0,
        16, 16, 0, visual->depth, InputOutput, visual->visual, CWColormap, &attrs);
      XMapWindow(display, context.window);
      context.context = glXCreateContext(display, visual, NULL, True);
      if (!context.context) { XFree(visual); return 1; }
    }
    XFree(visual);
    XSync(display, False);
    return 0;
  }
  bool current(int index) { return glXMakeCurrent(display, contexts[index].window, contexts[index].context) == True; }
  bool clear() { return glXMakeCurrent(display, None, NULL) == True; }
  ~NativeContexts() {
    if (!display) return;
    if (contexts[0].context || contexts[1].context) (void) clear();
    for (Context & context : contexts) {
      if (context.context) glXDestroyContext(display, context.context);
      if (context.window) XDestroyWindow(display, context.window);
      if (context.colormap) XFreeColormap(display, context.colormap);
    }
    XCloseDisplay(display);
  }
};
#endif
struct Borrow {
  const cc_glglue * glue; const void * owned[6];
  explicit Borrow(int id) : glue(cc_glglue_instance(id)) {
    // Force ownership of a dynamic-library handle as well as extension cache.
    (void) cc_glglue_getprocaddress(glue,"glGetString");
    (void) cc_glglue_glext_supported(glue,"GL_ARB_multitexture");
    owned[0]=glue->versionstr; owned[1]=glue->vendorstr; owned[2]=glue->rendererstr;
    owned[3]=glue->extensionsstr; owned[4]=glue->glextdict; owned[5]=glue->dl_handle;
  }
  bool released() const {
#ifdef COIN_GLGLUE_CHECK_RELEASED
    if (!__asan_address_is_poisoned(glue)) return false;
    for (const void * pointer:owned) if (pointer && !__asan_address_is_poisoned(pointer)) return false;
#endif
    return true;
  }
};
struct Probe {
  uint32_t id; const cc_glglue * glue; int callbacks; bool valid;
  Probe(uint32_t id) : id(id),glue(NULL),callbacks(0),valid(true) { }
  static void callback(uint32_t id,void * userdata) {
    Probe * self=static_cast<Probe *>(userdata); if (self->id!=id) return;
    ++self->callbacks;
    const cc_glglue * current=cc_glglue_instance(id);
    unsigned int major,minor,release; cc_glglue_glversion(current,&major,&minor,&release);
    if (current!=self->glue || major!=43) self->valid=false;
  }
};
static int cycle(NativeContexts & native,int index,Probe & probe,int count) {
  if (!native.current(index)) return 1;
  for (int i=0;i<count;++i) {
    Borrow borrow(probe.id); probe.glue=borrow.glue;
    if (borrow.glue->version.major==43) return 2; // A reused ID must get fresh metadata.
    const_cast<cc_glglue *>(borrow.glue)->version.major=43;
    SoContextHandler::destructingContext(probe.id);
    if (!probe.valid || !borrow.released()) return 3;
  }
  return native.clear() ? 0 : 4;
}
int main(int argc,char ** argv) {
  const bool raw=argc>1 && std::strcmp(argv[1],"--no-callbacks")==0;
  NativeContexts native;
  const int setup = native.initialize();
  if (setup != 0) return setup;
  if (!raw) SoDB::init();
  CHECK(native.current(0));
  // Unknown IDs must not require a previously created callback registry.
  SoContextHandler::destructingContext(9998);
  if (raw) {
    Borrow borrow(9999); SoContextHandler::destructingContext(9999); CHECK(borrow.released());
  }
  else {
    Probe a(7001),b(7002);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&a);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&b);
    CHECK(cycle(native,0,a,32)==0 && a.callbacks==32);
    // Distinct contexts may be used and destroyed concurrently. Each context's
    // callers serialize borrowing and destruction within its owning thread.
    int resultA=-1,resultB=-1;
    std::thread threadA([&] { resultA=cycle(native,0,a,16); });
    std::thread threadB([&] { resultB=cycle(native,1,b,16); });
    threadA.join(); threadB.join();
    CHECK(resultA==0 && resultB==0 && a.callbacks==48 && b.callbacks==16);
    Probe high(0x80000001U),maximum(0xffffffffU);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&high);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&maximum);
    CHECK(cycle(native,0,high,4)==0 && high.callbacks==4);
    CHECK(cycle(native,0,maximum,4)==0 && maximum.callbacks==4);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&high);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&maximum);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&a);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&b);
    CHECK(native.current(0));
    // A still-live cached record is released during Coin shutdown too.
    Borrow active(7100); SoDB::finish(); CHECK(active.released());
  }
  CHECK(native.clear());
  std::printf("%s: %s\n", NativeContexts::backend(), raw ?
    "Glue released without registered callbacks." :
    "Glue: callbacks, owned allocations, 72 generations, two threads and shutdown passed.");
  return 0;
}
