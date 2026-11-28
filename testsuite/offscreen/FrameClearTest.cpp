#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>


#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
#include <Inventor/system/gl.h>
#include <dlfcn.h>
namespace {
int clearCalls=0;
int clearBeforePreparation=0;
int expectedViewportWidth=0,expectedViewportHeight=0;
}
extern "C" void glClear(GLbitfield mask) {
  using Clear=void (*)(GLbitfield);
  static Clear real=reinterpret_cast<Clear>(dlsym(RTLD_NEXT,"glClear"));
  if(!real) std::abort();
  if((mask&(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT))==
      (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)) {
    GLint viewport[4];glGetIntegerv(GL_VIEWPORT,viewport);
    if(viewport[2]!=expectedViewportWidth ||
        viewport[3]!=expectedViewportHeight) ++clearBeforePreparation;
    ++clearCalls;
  }
  real(mask);
}
#endif

namespace {
constexpr int width=37,height=29;
bool require(bool good,const std::string & label) {
  if(!good) std::cerr << "offscreen_frame_clear FAIL " << label << '\n';
  return good;
}
struct Diagnostics {
  SoErrorCB * previous=SoDebugError::getHandlerCallback();
  void * previousData=SoDebugError::getHandlerData();
  int missingRemoval=0;
  int unexpectedErrors=0;
  Diagnostics() {SoDebugError::setHandlerCallback(capture,this);}
  ~Diagnostics() {SoDebugError::setHandlerCallback(previous,previousData);}
  static void capture(const SoError * error,void * data) {
    auto * self=static_cast<Diagnostics *>(data);
    const char * message=error->getDebugString().getString();
    if(std::strstr(message,"removePreRenderCallback")) ++self->missingRemoval;
    const auto * debug=static_cast<const SoDebugError *>(error);
    if(debug->getSeverity()==SoDebugError::ERROR) ++self->unexpectedErrors;
    std::cerr << "offscreen_frame_clear Coin diagnostic: " << message << '\n';
  }
};
void userPreRender(void * context,SoGLRenderAction * action) {
#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
  const SbVec2s size=action->getViewportRegion().getViewportSizePixels();
  expectedViewportWidth=size[0];expectedViewportHeight=size[1];
#endif
  (void)action;
  ++*static_cast<int *>(context);
}
struct MultiApplyAction : SoGLRenderAction {
  explicit MultiApplyAction(const SbViewportRegion & viewport):SoGLRenderAction(viewport) {}
  SoNode * overlay=nullptr;
  int applications=0;
  void expectViewport() {
#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
    const SbVec2s size=getViewportRegion().getViewportSizePixels();
    expectedViewportWidth=size[0];expectedViewportHeight=size[1];
#endif
  }
  void apply(SoNode * root) override {
    expectViewport();
    ++applications;
    SoGLRenderAction::apply(root);
    if(overlay) SoGLRenderAction::apply(overlay);
  }
  void apply(SoPath * path) override {
    expectViewport();
    ++applications;
    SoGLRenderAction::apply(path);
    if(overlay) SoGLRenderAction::apply(overlay);
  }
};
SoSeparator * rectangle(float left,float right,const SbColor & color,float transparency) {
  auto * root=new SoSeparator;
  auto * camera=new SoOrthographicCamera;
  camera->position.setValue(0,0,3);camera->height=2;
  camera->nearDistance=.1f;camera->farDistance=10;
  root->addChild(camera);
  auto * lighting=new SoLightModel;
  lighting->model=SoLightModel::BASE_COLOR;
  root->addChild(lighting);
  auto * material=new SoMaterial;
  material->diffuseColor.setValue(color);
  material->transparency=transparency;
  root->addChild(material);
  auto * points=new SoCoordinate3;
  const SbVec3f vertices[]={SbVec3f(left,-.75f,0),SbVec3f(right,-.75f,0),
    SbVec3f(right,.75f,0),SbVec3f(left,.75f,0)};
  points->point.setValues(0,4,vertices);root->addChild(points);
  auto * face=new SoFaceSet;face->numVertices.set1Value(0,4);root->addChild(face);
  return root;
}
bool sample(const SoOffscreenRenderer & renderer,int x,int y,
            int red,int green,int blue,const std::string & label) {
  const auto * image=renderer.getBuffer();
  if(!require(image!=nullptr,label+" readback")) return false;
  const size_t at=size_t(y*width+x)*3;
  const int actual[]={image[at],image[at+1],image[at+2]};
  const int expected[]={red,green,blue};
  bool pass=true;
  for(int channel=0;channel<3;++channel)
    pass &= std::abs(actual[channel]-expected[channel])<=4;
  if(!pass) std::cerr << label << " x=" << x << " y=" << y << " RGB="
    << actual[0] << ',' << actual[1] << ',' << actual[2] << " expected="
    << red << ',' << green << ',' << blue << '\n';
  return require(pass,label+" pixel");
}
bool verify(const SoOffscreenRenderer & renderer,bool overlay,const std::string & label) {
  bool ok=true;
  ok &= sample(renderer,3,3,0,0,0,label+" corner-bottom-left");
  ok &= sample(renderer,33,25,0,0,0,label+" corner-top-right");
  ok &= sample(renderer,8,14,255,0,0,label+" base-left");
  ok &= sample(renderer,28,14,0,overlay?128:0,0,label+" overlay-right");
  // Analytic orthographic image; skip a two-pixel band around polygon edges.
  // This includes interiors and background in every non-divisible border tile.
  for(int y=0;y<height;++y) for(int x=0;x<width;++x) {
    const float wx=(2.0f*(x+.5f)-width)/height;
    const float wy=(2.0f*(y+.5f)-height)/height;
    const float edges[]={-.85f,-.15f,.15f,.85f};
    bool nearEdge=std::abs(std::abs(wy)-.75f)<.14f;
    for(float edge:edges) nearEdge |= std::abs(wx-edge)<.14f;
    if(nearEdge) continue;
    const bool inY=std::abs(wy)<.75f;
    const bool inBase=inY && wx>-.85f && wx<-.15f;
    const bool inOverlay=overlay && inY && wx>.15f && wx<.85f;
    ok &= sample(renderer,x,y,inBase?255:0,inOverlay?128:0,0,label+" analytic");
  }
  return ok;
}
}

int main(int argc,char ** argv) {
  const bool tiled=argc==2 && std::string(argv[1])=="--tiled";
  if(argc>2 || (argc==2 && !tiled)) return 2;
#ifndef _WIN32
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING","1",0);
  if(tiled) {
    setenv("COIN_OFFSCREENRENDERER_TILEWIDTH","16",1);
    setenv("COIN_OFFSCREENRENDERER_TILEHEIGHT","11",1);
  }
  else {
    unsetenv("COIN_FORCE_TILED_OFFSCREENRENDERING");
    unsetenv("COIN_OFFSCREENRENDERER_TILEWIDTH");
    unsetenv("COIN_OFFSCREENRENDERER_TILEHEIGHT");
    unsetenv("COIN_OFFSCREENRENDERER_MAX_TILESIZE");
  }
#endif
  SoDB::init();
  Diagnostics diagnostics;
  SoSeparator * base=rectangle(-.85f,-.15f,SbColor(1,0,0),0);
  SoSeparator * overlay=rectangle(.15f,.85f,SbColor(0,1,0),.5f);
  base->ref();overlay->ref();
  const SbViewportRegion viewport(width,height);
  MultiApplyAction multi(viewport);
  multi.overlay=overlay;
  multi.setTransparencyType(SoGLRenderAction::BLEND);
  int firstCallback=0,secondCallback=0;
  multi.addPreRenderCallback(userPreRender,&firstCallback);
  bool ok=true;
  {
    SoOffscreenRenderer renderer(viewport);
    renderer.setGLRenderAction(&multi);
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0,0,0));
    if(!renderer.render(base)) {
      std::cerr << "offscreen_frame_clear GL context unavailable\n";
      base->unref();overlay->unref();
      return 77;
    }
    ok &= verify(renderer,true,"first-two-applies");
#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
    ok &= require(clearCalls==(tiled?9:1),"one color/depth clear per tile/frame");
    ok &= require(clearBeforePreparation==0,"clear follows action viewport preparation");
#endif
    ok &= require(multi.applications==(tiled?9:1),"real 37x29 canvas uses nine 16x11 tiles or one frame");
    const int afterFirst=firstCallback;
    ok &= require(afterFirst==(tiled?18:2),"user callback ran for both applies and tiles");
    ok &= require(renderer.render(base),"successive render");
    ok &= verify(renderer,true,"successive-two-applies");
#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
    ok &= require(clearCalls==(tiled?18:2),"next render rearms each clear");
#endif
    ok &= require(firstCallback==afterFirst*2,"user callback survived successive render");

    SoPath * path=new SoPath(base);path->ref();
    ok &= require(renderer.render(path),"SoPath with overlay");
    ok &= verify(renderer,true,"path-two-applies");
    multi.overlay=nullptr;
    renderer.setBackgroundColor(SbColor(0,0,1));
    static_cast<SoMaterial *>(base->getChild(2))->diffuseColor.setValue(1,1,0);
    ok &= require(renderer.render(path),"SoPath render");
    ok &= sample(renderer,3,3,0,0,255,"new-background");
    ok &= sample(renderer,8,14,255,255,0,"changed-base");
    ok &= sample(renderer,28,14,0,0,255,"previous-overlay-cleared");
    renderer.setBackgroundColor(SbColor(0,0,0));
    static_cast<SoMaterial *>(base->getChild(2))->diffuseColor.setValue(1,0,0);
    path->unref();

    SoGLRenderAction replacement(viewport);
    replacement.setTransparencyType(SoGLRenderAction::BLEND);
    replacement.addPreRenderCallback(userPreRender,&secondCallback);
    renderer.setGLRenderAction(&replacement);
    const int oldCount=firstCallback;
    ok &= require(renderer.render(base),"external replacement action render");
    ok &= verify(renderer,false,"replacement-action");
    ok &= require(firstCallback==oldCount && secondCallback>0,
      "callback registration stays with its own action");
    replacement.removePreRenderCallback(userPreRender,&secondCallback);
    renderer.setGLRenderAction(&multi);
  }
  multi.removePreRenderCallback(userPreRender,&firstCallback);
#ifdef COIN_OFFSCREEN_OBSERVE_CLEAR
  ok &= require(clearCalls==(tiled?45:5),"exactly one clear for all node/path frames and tiles");
  ok &= require(clearBeforePreparation==0,"all clears use the prepared viewport");
#endif
  ok &= require(diagnostics.missingRemoval==0 && diagnostics.unexpectedErrors==0,
    "no unbalanced callback diagnostic or other GL error");
  std::cout << "offscreen_frame_clear tiled=" << tiled
            << " first_callback=" << firstCallback
            << " replacement_callback=" << secondCallback
            << " missing_removal=" << diagnostics.missingRemoval << '\n';
  base->unref();overlay->unref();
  return ok?0:1;
}
