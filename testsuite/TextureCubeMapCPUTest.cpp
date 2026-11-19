// CPU traversal coverage: these tests create no GL context and never render.
#include <Inventor/SoDB.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <cmath>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <string>

namespace {
int failures = 0, checks = 0;
std::string label, action;
void check(bool ok, const char * what) {
  ++checks;
  if (!ok) { ++failures; std::fprintf(stderr,"FAIL %s %s: %s\n",action.c_str(),label.c_str(),what); }
}
void equal(const SbVec4f & got, const SbVec4f & want, const char * what) {
  bool ok = true;
  for (int i=0;i<4;++i) ok = ok && std::isfinite(got[i]) && std::fabs(got[i]-want[i]) < 3e-5f;
  check(ok,what);
  if (!ok) std::fprintf(stderr,"  got (%g,%g,%g,%g), expected (%g,%g,%g,%g)\n",
    got[0],got[1],got[2],got[3],want[0],want[1],want[2],want[3]);
}
SoSFImage & face(SoTextureCubeMap * n,int i) {
  SoSFImage * fields[] = {&n->imagePosX,&n->imageNegX,&n->imagePosY,&n->imageNegY,&n->imagePosZ,&n->imageNegZ};
  return *fields[i];
}
void pixels(SoSFImage & f,int w=2,int h=2,int nc=4) {
  static const unsigned char data[64] = {17,29,43,255,59,71,83,255};
  f.setValue(SbVec2s(w,h),nc,data);
}
SoTextureCubeMap * cube(int i=0) {
  SoTextureCubeMap * n = new SoTextureCubeMap; pixels(face(n,i)); return n;
}
void unit(SoGroup * root,int u) {
  SoTextureUnit * n = new SoTextureUnit; n->unit=u; root->addChild(n);
}
void texture2(SoGroup * root) {
  SoTexture2 * n = new SoTexture2; pixels(n->image,1,2,3); root->addChild(n);
}
class Probe : public SoCube {
public:
  std::function<void(SoState *)> inspect;
  void callback(SoCallbackAction * a) override { inspect(a->getState()); }
  void rayPick(SoRayPickAction * a) override { inspect(a->getState()); }
};
void probe(SoGroup * root, const std::function<void(SoState *)> & f) {
  Probe * p = new Probe; p->inspect=f; root->addChild(p);
}
void traverse(SoNode * root) {
  if (action=="callback") { SoCallbackAction a; a.apply(root); }
  else { SoRayPickAction a(SbViewportRegion(32,32)); a.enableCulling(FALSE);
    a.setRay(SbVec3f(0,0,5),SbVec3f(0,0,-1)); a.apply(root); }
}
struct Image {
  SbVec3s size;
  int nc;
  const unsigned char * bytes;
  SoMultiTextureImageElement::Wrap s,t,r;
  SoMultiTextureImageElement::Model model;
  SbColor blend;
  void read(SoState * state,int u) {
    bytes=SoMultiTextureImageElement::get(state,u,size,nc,s,t,r,model,blend);
  }
  void unchanged(SoState * state,int u) const {
    Image now; now.read(state,u);
    check(now.bytes==bytes && now.size==size && now.nc==nc && now.s==s && now.t==t &&
          now.r==r && now.model==model && now.blend==blend,"cubemap leaves image element unchanged");
  }
};
void mode(SoState * s,int u,SoMultiTextureEnabledElement::Mode want) {
  check(SoMultiTextureEnabledElement::getMode(s,u)==want,"expected CPU texture mode");
}
void stateCase(SoTextureCubeMap * n,int selected,bool seed,bool overridden,bool effective) {
  SoGroup * root=new SoGroup; root->ref();
  unit(root,1); texture2(root); unit(root,0); if(seed) texture2(root);
  Image images[2];
  probe(root,[&](SoState * s) {
    images[0].read(s,0); images[1].read(s,1);
    SoTextureOverrideElement::setImageOverride(s,overridden);
  });
  unit(root,selected); root->addChild(n);
  probe(root,[&](SoState * s) {
    const bool changed=effective && selected>=0 && !(selected==0 && overridden);
    mode(s,0, changed && selected==0 ? SoMultiTextureEnabledElement::CUBEMAP :
      seed ? SoMultiTextureEnabledElement::TEXTURE2D : SoMultiTextureEnabledElement::DISABLED);
    mode(s,1, changed && selected==1 ? SoMultiTextureEnabledElement::CUBEMAP : SoMultiTextureEnabledElement::TEXTURE2D);
    mode(s,3, changed && selected==3 ? SoMultiTextureEnabledElement::CUBEMAP : SoMultiTextureEnabledElement::DISABLED);
    images[0].unchanged(s,0); images[1].unchanged(s,1);
    check(bool(SoTextureOverrideElement::getImageOverride(s))==
      (overridden || (changed && selected==0 && n->isOverride())),"image override follows unit zero convention");
  });
  traverse(root); traverse(root); root->unref();
}
void stateCases() {
  for(int i=0;i<6;++i) for(int u : {0,1,3}) {
    label="single face="+std::to_string(i)+" unit="+std::to_string(u);
    stateCase(cube(i),u,false,false,true);
  }
  for(int nc=1;nc<=4;++nc) {
    label="nonsquare partial face components="+std::to_string(nc);
    SoTextureCubeMap * n=new SoTextureCubeMap; pixels(face(n,5),1,3,nc);
    stateCase(n,0,false,false,true);
  }
  label="mismatched partial faces";
  SoTextureCubeMap * mismatch=cube(); pixels(face(mismatch,4),3,1,1);
  stateCase(mismatch,0,false,false,true);
  for(bool seed : {false,true}) {
    label="empty cube"; stateCase(new SoTextureCubeMap,0,seed,false,false);
    label="ignored pixels";
    SoTextureCubeMap * ignored=cube(); face(ignored,0).setIgnored(TRUE);
    stateCase(ignored,0,seed,false,false);
    label="zero extent with components";
    SoTextureCubeMap * empty=new SoTextureCubeMap; pixels(face(empty,3),0,2,4);
    stateCase(empty,0,seed,false,false);
    label="invalid five components";
    SoTextureCubeMap * invalid=new SoTextureCubeMap; pixels(face(invalid,0),1,1,5);
    stateCase(invalid,0,seed,false,false);
    label="zero components";
    SoTextureCubeMap * zero=new SoTextureCubeMap; pixels(face(zero,0),1,1,0);
    stateCase(zero,0,seed,false,false);
    label="positive extent and components without pixel storage";
    SoTextureCubeMap * nullpixels=new SoTextureCubeMap;
    face(nullpixels,2).setValue(SbVec2s(2,2),4,NULL,SoSFImage::NO_COPY);
    stateCase(nullpixels,0,seed,false,false);
    label="negative extent without allocation";
    SoTextureCubeMap * negative=new SoTextureCubeMap;
    face(negative,2).setValue(SbVec2s(-1,2),4,NULL,SoSFImage::NO_COPY);
    stateCase(negative,0,seed,false,false);
  }
  for(int u : {-1,0,1,3}) {
    label="existing override unit="+std::to_string(u);
    stateCase(cube(),u,true,true,true);
    label="own override unit="+std::to_string(u);
    SoTextureCubeMap * n=cube(); n->setOverride(TRUE);
    stateCase(n,u,true,false,true);
  }
  label="empty override is no-op";
  SoTextureCubeMap * n=new SoTextureCubeMap; n->setOverride(TRUE);
  stateCase(n,0,true,false,false);
  label="filenames without loaded pixels";
  n=new SoTextureCubeMap; n->filenames.set1Value(0,"coin-cubemap-cpu-do-not-load-this-file.png");
  stateCase(n,0,true,false,false);
  label="ignored first face does not hide later valid face";
  n=cube(); face(n,0).setIgnored(TRUE); pixels(face(n,5),1,3,2);
  stateCase(n,0,false,false,true);
}
void reuseCases() {
  SoGroup * root=new SoGroup; root->ref();
  SoTextureCubeMap * shared=cube(5); shared->ref();
  unit(root,0); texture2(root);
  SoSeparator * sep=new SoSeparator; sep->pickCulling=SoSeparator::OFF; root->addChild(sep);
  sep->addChild(shared);
  probe(sep,[](SoState * s){ mode(s,0,SoMultiTextureEnabledElement::CUBEMAP); });
  unit(sep,1); sep->addChild(shared);
  probe(sep,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::CUBEMAP);mode(s,1,SoMultiTextureEnabledElement::CUBEMAP);});
  probe(root,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::TEXTURE2D);mode(s,1,SoMultiTextureEnabledElement::DISABLED);});
  label="shared node and Separator restore"; traverse(root); traverse(root); root->unref();
  for(int iteration=0;iteration<4;++iteration) {
    root=new SoGroup; root->ref(); unit(root,0); texture2(root); root->addChild(shared);
    if(iteration==1) face(shared,5).setIgnored(TRUE);
    if(iteration==2) {face(shared,5).setIgnored(FALSE); face(shared,5).setValue(SbVec2s(0,0),0,NULL);}
    if(iteration==3) pixels(face(shared,2),3,1,1);
    probe(root,[iteration](SoState * s){ mode(s,0,iteration==0 || iteration==3 ?
      SoMultiTextureEnabledElement::CUBEMAP : SoMultiTextureEnabledElement::TEXTURE2D); });
    label="face edits iteration="+std::to_string(iteration); traverse(root); root->unref();
  }
  shared->unref();
  root=new SoGroup; root->ref(); root->addChild(cube());
  probe(root,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::CUBEMAP);});
  texture2(root);
  probe(root,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::TEXTURE2D);});
  root->addChild(cube(4));
  probe(root,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::CUBEMAP);});
  root->addChild(new SoTexture2);
  probe(root,[](SoState * s){mode(s,0,SoMultiTextureEnabledElement::DISABLED);});
  label="cube to 2D to cube to empty 2D"; traverse(root); root->unref();
  root=new SoGroup; root->ref(); texture2(root);
  sep=new SoSeparator; sep->pickCulling=SoSeparator::OFF; root->addChild(sep);
  SoTextureCubeMap * overriding=cube(); overriding->setOverride(TRUE); sep->addChild(overriding);
  texture2(sep);
  probe(sep,[](SoState * s){
    mode(s,0,SoMultiTextureEnabledElement::CUBEMAP);
    check(SoTextureOverrideElement::getImageOverride(s),"cube override blocks later 2D texture");
  });
  probe(root,[](SoState * s){
    mode(s,0,SoMultiTextureEnabledElement::TEXTURE2D);
    check(!SoTextureOverrideElement::getImageOverride(s),"Separator restores image override");
  });
  label="Separator restores override and texture mode"; traverse(root); root->unref();
}
const SbVec3f points[] = {SbVec3f(-2,-1,0),SbVec3f(2,-1,0),SbVec3f(0,2,0)};
const SbVec3f explicitcoords[] = {SbVec3f(.2f,.3f,.7f),SbVec3f(.6f,.1f,.9f),SbVec3f(.4f,.8f,.5f)};
SbVec4f expected(const SbVec3f & p,int generator) {
  if(generator==0) return SbVec4f(0,.6f,.8f,1);
  if(generator==2) {
    for(int i=0;i<3;++i) if((p-points[i]).length()<1e-5f)
      return SbVec4f(explicitcoords[i][0],explicitcoords[i][1],explicitcoords[i][2],1);
    check(false,"explicit callback vertex matches mesh"); return SbVec4f(0,0,0,1);
  }
  double x=p[0],y=p[1],z=double(p[2])-5;
  const double length=std::sqrt(x*x+y*y+z*z), dot=.6*y+.8*z;
  return SbVec4f(float(x/length),float((y-1.2*dot)/length),float((z-1.6*dot)/length),1);
}
struct Samples {int generator; int triangles;};
void triangle(void * data,SoCallbackAction *,const SoPrimitiveVertex * a,
              const SoPrimitiveVertex * b,const SoPrimitiveVertex * c) {
  Samples * samples=static_cast<Samples *>(data); ++samples->triangles;
  const SoPrimitiveVertex * vertices[]={a,b,c};
  for(int i=0;i<3;++i) equal(vertices[i]->getTextureCoords(),expected(vertices[i]->getPoint(),samples->generator),"real mesh callback coordinate");
}
void meshCases() {
  for(bool indexed : {false,true}) for(int generator=0;generator<3;++generator)
    for(int imagecase=0;imagecase<7;++imagecase) {
    label=std::string(indexed?"IndexedFaceSet":"FaceSet")+" generator="+std::to_string(generator)+" face="+std::to_string(imagecase);
    SoSeparator * root=new SoSeparator; root->ref(); root->pickCulling=SoSeparator::OFF;
    SoOrthographicCamera * camera=new SoOrthographicCamera; camera->position.setValue(0,0,5); root->addChild(camera);
    SoTextureCubeMap * n=cube(imagecase%6);
    if(imagecase==6) for(int i=0;i<6;++i) pixels(face(n,i));
    root->addChild(n);
    if(generator==0) root->addChild(new SoTextureCoordinateNormalMap);
    else if(generator==1) root->addChild(new SoTextureCoordinateReflectionMap);
    else {SoTextureCoordinate3 * tc=new SoTextureCoordinate3; tc->point.setValues(0,3,explicitcoords); root->addChild(tc);}
    SoCoordinate3 * coords=new SoCoordinate3; coords->point.setValues(0,3,points); root->addChild(coords);
    SoNormal * normal=new SoNormal; normal->vector.set1Value(0,SbVec3f(0,.6f,.8f)); root->addChild(normal);
    SoNormalBinding * binding=new SoNormalBinding; binding->value=SoNormalBinding::OVERALL; root->addChild(binding);
    if(indexed) {SoIndexedFaceSet * shape=new SoIndexedFaceSet; const int32_t indices[]={0,1,2,-1};shape->coordIndex.setValues(0,4,indices);root->addChild(shape);}
    else {SoFaceSet * shape=new SoFaceSet;shape->numVertices.set1Value(0,3);root->addChild(shape);}
    for(int repeat=0;repeat<2;++repeat) {
      if(action=="callback") {
        Samples samples={generator,0}; SoCallbackAction a;
        a.addTriangleCallback(SoFaceSet::getClassTypeId(),triangle,&samples);
        a.addTriangleCallback(SoIndexedFaceSet::getClassTypeId(),triangle,&samples); a.apply(root);
        check(samples.triangles==1,"real mesh generated one triangle");
      } else {
        SoRayPickAction a(SbViewportRegion(32,32)); a.enableCulling(FALSE);
        a.setRay(SbVec3f(0,0,5),SbVec3f(0,0,-1)); a.apply(root);
        const SoPickedPoint * picked=a.getPickedPoint(); check(picked!=NULL,"real mesh ray intersection exists");
        if(picked) {
          SbVec4f want(0,0,0,0);
          for(int i=0;i<3;++i) want+=expected(points[i],generator)*(1.0f/3.0f);
          equal(picked->getTextureCoords(),want,"real mesh picked interpolated coordinate");
        }
      }
    }
    root->unref();
  }
}
}
int main(int argc,char ** argv) {
  if(argc!=3) return 2; action=argv[1]; const std::string which=argv[2];
  if(action!="callback" && action!="pick") return 2;
  SoDB::init();
  if(which=="mesh") meshCases();
  else if(which=="state") stateCases();
  else if(which=="reuse") reuseCases();
  else return 2;
  std::printf("%d checks, %d failures\n",checks,failures); return failures?1:0;
}
