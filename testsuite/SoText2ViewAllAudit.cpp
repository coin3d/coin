#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoFont.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
struct Counts {int pixel[3]={};};
static Counts render(SoSeparator *root,SoOffscreenRenderer &r,int w,int h,const std::string &file){
 if(!r.render(root)){std::fprintf(stderr,"render unavailable\n");std::exit(77);}
 auto *p=r.getBuffer();Counts c;
 for(int i=0;i<w*h;++i)for(int k=0;k<3;++k)if(p[3*i+k]>30 && p[3*i+(k+1)%3]<10 && p[3*i+(k+2)%3]<10)++c.pixel[k];
 if(!file.empty()){
  FILE *f=std::fopen(file.c_str(),"wb");if(!f)std::exit(2);
  std::fprintf(f,"P6\n%d %d\n255\n",w,h);
  for(int y=h-1;y>=0;--y)std::fwrite(p+3*w*y,1,3*w,f);
  std::fclose(f);
 }
 return c;
}
int main(int argc,char **argv){
#if defined(__linux__)
 const char *display=std::getenv("DISPLAY");
 if(!display || !display[0]){std::fprintf(stderr,"GLX rendering needs an X display; use xvfb-run for headless testing.\n");return 77;}
#endif
 SoDB::init();const std::string prefix=argc>1?argv[1]:"";
 const bool diagnostic=argc>2 && std::strcmp(argv[2],"--diagnostic")==0;const bool anchorpolicy=argc>3 && std::strcmp(argv[3],"--anchor-only")==0;
 int failures=0,cases=0;
 for(int ortho=0;ortho<2;++ortho)for(int fontsize: {10,24,96})for(float slack:{1.f,1.1f}){
  auto *root=new SoSeparator;root->ref();SoCamera *camera=ortho?static_cast<SoCamera *>(new SoOrthographicCamera):new SoPerspectiveCamera;
  root->addChild(camera);auto *font=new SoFont;font->size=float(fontsize);
  if(const char *name=std::getenv("COIN_VIEWALL_TEST_FONT"))font->name=name;
  root->addChild(font);
  const char *labels[]={"XXX","YYY","ZZZ"};
  for(int j=0;j<3;++j){if(j){auto *t=new SoTranslation;t->translation.setValue(0,100,0);root->addChild(t);}
   auto *color=new SoBaseColor;color->rgb.setValue(j==0,j==1,j==2);root->addChild(color);
   auto *text=new SoText2;text->string=labels[j];root->addChild(text);
  }
  const int w=640,h=480;SbViewportRegion vp(w,h);SoOffscreenRenderer renderer(vp);renderer.setComponents(SoOffscreenRenderer::RGB);
  camera->position.setValue(0,100,500);camera->nearDistance=1;camera->farDistance=1000;
  if(ortho)static_cast<SoOrthographicCamera *>(camera)->height=500;
  Counts reference=render(root,renderer,w,h,"");
  camera->position.setValue(0,0,1);camera->nearDistance=1;camera->farDistance=10;
  if(ortho)static_cast<SoOrthographicCamera *>(camera)->height=2;
  const std::string tag=(ortho?"ortho":"perspective")+std::string("-size")+std::to_string(fontsize)+"-slack"+(slack==1?"1":"1.1");
  // Keep the culling control away from the exact near-plane boundary.
  camera->position.setValue(0,0,5);
  Counts initial=render(root,renderer,w,h,"");
  SoRayPickAction pick(vp);pick.setRay(SbVec3f(0,200,3),SbVec3f(0,0,-1),0,10);pick.apply(root);
  const bool cullingok=initial.pixel[0]==reference.pixel[0] && initial.pixel[1]==0 && initial.pixel[2]==0 && !pick.getPickedPoint();
  std::printf("%s initial-culling=%s pixels=%d,%d,%d offscreen-pick=%d\n",tag.c_str(),cullingok?"PASS":"FAIL",initial.pixel[0],initial.pixel[1],initial.pixel[2],pick.getPickedPoint()!=NULL);
  if(!cullingok)++failures;
  camera->position.setValue(0,0,1);
  int visible=0,complete=0;
  for(int pass=1;pass<=8;++pass){if(anchorpolicy){SbBox3f anchors;anchors.extendBy(SbVec3f(0,0,0));anchors.extendBy(SbVec3f(0,200,0));camera->viewBoundingBox(anchors,vp.getViewportAspectRatio(),slack);}else camera->viewAll(root,vp,slack);Counts c=render(root,renderer,w,h,prefix.empty()?"":prefix+"-"+tag+"-pass"+std::to_string(pass)+".ppm");visible=complete=0;
   for(int j=0;j<3;++j){visible+=c.pixel[j]>0;complete+=c.pixel[j]==reference.pixel[j] && reference.pixel[j]>0;}
   auto pos=camera->position.getValue();
   std::printf("%s pass=%d visible=%d complete=%d pixels=%d,%d,%d reference=%d,%d,%d camera=(%.9g,%.9g,%.9g)\n",tag.c_str(),pass,visible,complete,c.pixel[0],c.pixel[1],c.pixel[2],reference.pixel[0],reference.pixel[1],reference.pixel[2],pos[0],pos[1],pos[2]);
   ++cases;if(complete!=3)++failures;
  }
  root->unref();
 }
 // A wide label that still fits: compare against a fully visible centered
 // reference, then require LEFT alignment to fit in one public call.
 for(int ortho=0;ortho<2;++ortho){auto *root=new SoSeparator;root->ref();SoCamera *c=ortho?static_cast<SoCamera *>(new SoOrthographicCamera):new SoPerspectiveCamera;root->addChild(c);
  auto *font=new SoFont;font->name="DejaVu Sans";font->size=300;root->addChild(font);auto *color=new SoBaseColor;color->rgb.setValue(1,0,0);root->addChild(color);
  auto *text=new SoText2;text->string="XXX";text->justification=SoText2::CENTER;root->addChild(text);SbViewportRegion vp(640,480);SoOffscreenRenderer r(vp);r.setComponents(SoOffscreenRenderer::RGB);
  c->position.setValue(0,0,500);c->nearDistance=1;c->farDistance=1000;if(ortho)static_cast<SoOrthographicCamera *>(c)->height=500;
  const Counts expected=render(root,r,640,480,"");text->justification=SoText2::LEFT;c->position.setValue(0,0,1);c->nearDistance=1;c->farDistance=10;if(ortho)static_cast<SoOrthographicCamera *>(c)->height=2;
  c->viewAll(root,vp);const Counts actual=render(root,r,640,480,"");++cases;const bool ok=expected.pixel[0]>0 && actual.pixel[0]==expected.pixel[0];if(!ok)++failures;
  std::printf("wide-label ortho=%d reference=%d actual=%d result=%s\n",ortho,expected.pixel[0],actual.pixel[0],ok?"PASS":"FAIL");root->unref();
 }
 SoDB::finish();std::printf("Text framing (first and repeated calls): %d cases, %d failures\n",cases,failures);
 return diagnostic?0:(failures?1:0);
}
