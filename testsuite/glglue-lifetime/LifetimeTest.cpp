#include <cstdio>
#include <cstring>
#include <thread>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <Inventor/SoDB.h>
#include <Inventor/misc/SoContextHandler.h>
#include "glue/glp.h"
#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#endif
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#c); return 1; } } while (0)
struct NativeContext { GLXContext context; Window window; Colormap colormap; };
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
#ifdef __SANITIZE_ADDRESS__
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
static bool current(Display * display,NativeContext & native) {
  return glXMakeCurrent(display,native.window,native.context)==True;
}
static int cycle(Display * display,NativeContext & native,Probe & probe,int count) {
  if (!current(display,native)) return 1;
  for (int i=0;i<count;++i) {
    Borrow borrow(probe.id); probe.glue=borrow.glue;
    if (borrow.glue->versionmajor==43) return 2; // A reused ID must get fresh metadata.
    const_cast<cc_glglue *>(borrow.glue)->versionmajor=43;
    SoContextHandler::destructingContext(probe.id);
    if (!probe.valid || !borrow.released()) return 3;
  }
  glXMakeCurrent(display,None,NULL); return 0;
}
int main(int argc,char ** argv) {
  const bool raw=argc>1 && std::strcmp(argv[1],"--no-callbacks")==0;
  if (!XInitThreads()) return 77;
  Display * display=XOpenDisplay(NULL); if (!display) return 77;
  int attrs[]={GLX_RGBA,GLX_RED_SIZE,8,GLX_GREEN_SIZE,8,GLX_BLUE_SIZE,8,None};
  XVisualInfo * visual=glXChooseVisual(display,DefaultScreen(display),attrs); if (!visual) return 77;
  NativeContext native[2]={};
  for (NativeContext & n:native) {
    n.colormap=XCreateColormap(display,RootWindow(display,visual->screen),visual->visual,AllocNone);
    XSetWindowAttributes attrs={}; attrs.colormap=n.colormap;
    n.window=XCreateWindow(display,RootWindow(display,visual->screen),0,0,16,16,0,visual->depth,InputOutput,visual->visual,CWColormap,&attrs);
    XMapWindow(display,n.window); n.context=glXCreateContext(display,visual,NULL,True);
    CHECK(n.context);
  }
  XFree(visual); XSync(display,False);
  if (!raw) SoDB::init();
  CHECK(current(display,native[0]));
  // Unknown IDs must not require a previously created callback registry.
  SoContextHandler::destructingContext(9998);
  if (raw) {
    Borrow borrow(9999); SoContextHandler::destructingContext(9999); CHECK(borrow.released());
  }
  else {
    Probe a(7001),b(7002);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&a);
    SoContextHandler::addContextDestructionCallback(Probe::callback,&b);
    CHECK(cycle(display,native[0],a,32)==0 && a.callbacks==32);
    // Distinct contexts may be used and destroyed concurrently. Each context's
    // callers serialize borrowing and destruction within its owning thread.
    int resultA=-1,resultB=-1;
    std::thread threadA([&] { resultA=cycle(display,native[0],a,16); });
    std::thread threadB([&] { resultB=cycle(display,native[1],b,16); });
    threadA.join(); threadB.join();
    CHECK(resultA==0 && resultB==0 && a.callbacks==48 && b.callbacks==16);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&a);
    SoContextHandler::removeContextDestructionCallback(Probe::callback,&b);
    CHECK(current(display,native[0]));
    // A still-live cached record is released during Coin shutdown too.
    Borrow active(7100); SoDB::finish(); CHECK(active.released());
  }
  glXMakeCurrent(display,None,NULL);
  for (NativeContext & n:native) { glXDestroyContext(display,n.context); XDestroyWindow(display,n.window); XFreeColormap(display,n.colormap); }
  XCloseDisplay(display);
  std::puts(raw ? "Glue released without registered callbacks." : "Glue: callbacks, owned allocations, 64 generations, two threads and shutdown passed.");
  return 0;
}
