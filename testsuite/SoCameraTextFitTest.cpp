#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SoPath.h>
#include <Inventor/SbVec3d.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <string>
struct Observation { SbVec3f position; float nearplane,farplane; };
struct Monitor { SoCamera *camera; std::vector<Observation> values; };
static void observed(void *data,SoSensor *){auto &m=*static_cast<Monitor *>(data);m.values.push_back({m.camera->position.getValue(),m.camera->nearDistance.getValue(),m.camera->farDistance.getValue()});}
static int checks,failures;
static void check(const char *label,bool ok){++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL %s\n",label);}}
static bool contained(SoCamera *camera,SoSeparator *root,const SbViewportRegion &vp){
 SoGetBoundingBoxAction b(vp);b.apply(root);const auto box=b.getBoundingBox();
 if(box.isEmpty())return false;SbViewportRegion actual;const auto vv=camera->getViewVolume(vp,actual);
 for(int i=0;i<8;++i){SbVec3d point,screen;for(int a=0;a<3;++a)point[a]=(i&(1<<a))?box.getMax()[a]:box.getMin()[a];
  vv.getDPViewVolume().projectToScreen(point,screen);
  if(!std::isfinite(screen[0]) || !std::isfinite(screen[1]) || screen[0]<0 || screen[0]>1 || screen[1]<0 || screen[1]>1 || !std::isfinite(screen[2]) || screen[2]<-1e-6 || screen[2]>1+1e-6)return false;
 }
 return true;
}
int main(){
 SoDB::init();
 for(int ortho=0;ortho<2;++ortho)for(int external=0;external<2;++external)for(int path=0;path<2;++path)for(int portrait=0;portrait<2;++portrait)for(int rotated=0;rotated<2;++rotated)for(int mapping=0;mapping<=SoCamera::LEAVE_ALONE;++mapping)for(int depth=0;depth<2;++depth){
  auto *root=new SoSeparator;root->ref();SoCamera *c=ortho?static_cast<SoCamera *>(new SoOrthographicCamera):new SoPerspectiveCamera;root->addChild(c);
  auto *scene=new SoSeparator;root->addChild(scene);auto *font=new SoFont;font->name="defaultFont";font->size=24;scene->addChild(font);
  scene->addChild(new SoCube);
  for(int i=0;i<3;++i){if(i){auto *t=new SoTranslation;t->translation.setValue(0,100,depth?100:0);scene->addChild(t);}auto *text=new SoText2;text->string="XXX";text->justification=SoText2::CENTER;scene->addChild(text);}
  if(rotated)c->orientation.setValue(depth?SbVec3f(0,1,0):SbVec3f(0,0,1),.3f);
  c->viewportMapping=mapping;scene->boundingBoxCaching=SoSeparator::ON;
  const auto orientation=c->orientation.getValue();SbViewportRegion vp(portrait?320:640,480);
  Monitor monitor={c,{}};SoFieldSensor sensor(observed,&monitor);sensor.setPriority(0);sensor.attach(&c->position);
  SoNode *target=external?static_cast<SoNode *>(scene):root;
  if(path){auto *p=new SoPath(target);p->ref();c->viewAll(p,vp);p->unref();}else c->viewAll(target,vp);
  check("full final bounds",contained(c,root,vp));check("orientation preserved",c->orientation.getValue()==orientation);
  check("position notified",!monitor.values.empty());
  bool finalonly=true;for(const auto &v:monitor.values)finalonly &= v.position==c->position.getValue() && v.nearplane==c->nearDistance.getValue() && v.farplane==c->farDistance.getValue();
  check("sensor sees final camera only",finalonly);sensor.detach();root->unref();
 }
 // Oversized fixed-pixel text is an impossible fit: keep bounded finite work.
 for(int ortho=0;ortho<2;++ortho){auto *root=new SoSeparator;root->ref();SoCamera *c=ortho?static_cast<SoCamera *>(new SoOrthographicCamera):new SoPerspectiveCamera;root->addChild(c);
  auto *text=new SoText2;const std::string wide(2000,'W');text->string=wide.c_str();root->addChild(text);
  Monitor monitor={c,{}};SoFieldSensor sensor(observed,&monitor);sensor.setPriority(0);sensor.attach(&c->position);
  c->viewAll(root,SbViewportRegion(640,480));bool finite=true;for(int a=0;a<3;++a)finite &= std::isfinite(c->position.getValue()[a]);
  check("oversized text remains finite",finite && std::isfinite(c->nearDistance.getValue()) && std::isfinite(c->farDistance.getValue()));
  check("bounded trials publish once",monitor.values.size()<=1);sensor.detach();
  auto *before=static_cast<SoCamera *>(c->copy());before->ref();c->viewAll(root,SbViewportRegion(0,480));check("empty viewport leaves camera unchanged",c->fieldsAreEqual(before));before->unref();root->unref();
 }
 // The geometry-only path must retain the exact existing viewBoundingBox result.
 for(int ortho=0;ortho<2;++ortho){auto *scene=new SoSeparator;scene->ref();scene->addChild(new SoCube);
  SoCamera *a=ortho?static_cast<SoCamera *>(new SoOrthographicCamera):new SoPerspectiveCamera;a->ref();auto *b=static_cast<SoCamera *>(a->copy());b->ref();SbViewportRegion vp(640,480);
  SoGetBoundingBoxAction bounds(vp);bounds.apply(scene);b->viewBoundingBox(bounds.getBoundingBox(),vp.getViewportAspectRatio(),1);a->viewAll(scene,vp);
  check("ordinary geometry unchanged",a->fieldsAreEqual(b));a->unref();b->unref();scene->unref();
 }
 SoDB::finish();std::printf("Camera text fitting: %d checks, %d failures\n",checks,failures);return failures?1:0;
}
