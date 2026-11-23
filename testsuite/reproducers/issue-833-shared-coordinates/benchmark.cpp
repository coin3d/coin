// PR #833 shared-coordinate audit. The scenario and cost-bound proposal were
// reported by One-Shady-Cat in issuecomment-6565379111. This harness is independent.
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoIndexedPointSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/system/gl.h>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <sys/resource.h>

using Clock=std::chrono::steady_clock;
static int missing=0;
static void diagnostic(const SoError * e, void *) {
  const char * msg=e->getDebugString().getString();
  if(std::strstr(msg,"no coordinate array is available")) ++missing;
  std::fprintf(stderr,"%s\n",msg);
}
static void finish(void *,SoAction * a) {
  if(!a->isOfType(SoGLRenderAction::getClassTypeId())) return;
  static bool first=true;
  if(first) {
    std::fprintf(stderr,"GL_RENDERER=%s\nGL_VERSION=%s\n",glGetString(GL_RENDERER),glGetString(GL_VERSION));
    first=false;
  }
  glFinish();
}
static double rss() {
  std::ifstream f("/proc/self/status"); std::string line;
  while(std::getline(f,line)) if(line.find("VmRSS:")==0) return std::strtod(line.c_str()+6,NULL)/1024;
  return -1;
}
static double peak() { struct rusage r; getrusage(RUSAGE_SELF,&r);return r.ru_maxrss/1024.0; }
static void emit(const char * phase,const std::vector<double>& input,SoOffscreenRenderer& renderer) {
  auto s=input; std::sort(s.begin(),s.end());
  const unsigned char * p=renderer.getBuffer();
  unsigned long long hash=14695981039346656037ULL; int lit=0;
  for(int i=0;i<128*128*3;++i){hash^=p[i];hash*=1099511628211ULL;if(p[i])++lit;}
  std::printf("{\"phase\":\"%s\",\"median_ms\":%.6f,\"rss_mib\":%.3f,\"peak_mib\":%.3f,\"hash\":\"%016llx\",\"nonzero_bytes\":%d,\"missing\":%d,\"samples_ms\":[",phase,s[s.size()/2],rss(),peak(),hash,lit,missing);
  for(size_t i=0;i<input.size();++i)std::printf("%s%.6f",i?",":"",input[i]);
  std::puts("]}");std::fflush(stdout);
}
static std::vector<SbVec3f> points(int count,int columns) {
  std::vector<SbVec3f> p; p.reserve(count);
  const int rows=(count/4+columns-1)/columns;
  const int cx[]={0,1,1,0},cy[]={0,0,1,1};
  for(int i=0;i<count;++i){int q=i/4,c=i%4;float x=float(q%columns+cx[c])/columns,y=float(q/columns+cy[c])/rows;p.emplace_back(x,y,0.1f*x+0.2f*y);}
  return p;
}
int main(int argc,char ** argv) {
  if(argc!=9){std::fprintf(stderr,"usage: shared|own face|ifs explicit|none own|uv|normal|fixed none|2d|3d|multi|mixed|empty  count staticframes coord|shape\n");return 2;}
  const bool shared=!std::strcmp(argv[1],"shared"),ifs=!std::strcmp(argv[2],"ifs"),normals=!std::strcmp(argv[3],"explicit");
  const std::string kind=argv[2],shader=argv[4],tex=argv[5],mutation=argv[8];
  if(kind=="vrml"&&!shared)return 2;
  if(mutation=="shape"&&kind!="face"&&!ifs)return 2;
  const int count=std::atoi(argv[6]),frames=std::atoi(argv[7]); if(count<1||count>400||frames<1)return 2;
  int sharedpoints=410881;
  if(const char * value=std::getenv("COIN_BENCHMARK_SHARED_POINTS")) {
    const long parsed=std::strtol(value,NULL,10);
    if(parsed<4096||parsed>INT_MAX)return 2;
    sharedpoints=static_cast<int>(parsed);
  }
  int verticespershape=4096;
  if(const char * value=std::getenv("COIN_BENCHMARK_VERTICES_PER_SHAPE")) {
    const long parsed=std::strtol(value,NULL,10);
    if(parsed<4||parsed>INT_MAX||parsed%4)return 2;
    verticespershape=static_cast<int>(parsed);
  }
  if(sharedpoints<verticespershape)return 2;
  SoDB::init(); SoDebugError::setHandlerCallback(diagnostic,NULL);
  SoSeparator * root=new SoSeparator;root->ref();root->renderCaching=SoSeparator::OFF;root->boundingBoxCaching=SoSeparator::OFF;
  SoOrthographicCamera * cam=new SoOrthographicCamera;cam->position.setValue(0.5f,0.5f,3);cam->height=1.4f;root->addChild(cam);
  SoLightModel * light=new SoLightModel;light->model=SoLightModel::BASE_COLOR;root->addChild(light);
  if(normals){SoNormal*n=new SoNormal;n->vector.setValue(-0.097590f,-0.195180f,0.975900f);root->addChild(n);SoNormalBinding*b=new SoNormalBinding;b->value=SoNormalBinding::OVERALL;root->addChild(b);}
  if(shader!="fixed") {
    SoShaderProgram * prog=new SoShaderProgram;SoVertexShader * vs=new SoVertexShader;SoFragmentShader * fs=new SoFragmentShader;
    vs->sourceType=fs->sourceType=SoShaderObject::GLSL_PROGRAM;
    vs->sourceProgram="varying vec3 n; void main(){ gl_Position=ftransform(); gl_TexCoord[0]=gl_MultiTexCoord0; gl_TexCoord[1]=gl_MultiTexCoord1; gl_TexCoord[2]=gl_MultiTexCoord2; n=gl_Normal; }";
    fs->sourceProgram=shader=="normal"?"varying vec3 n; void main(){gl_FragColor=vec4(0.5+0.5*normalize(n),1.0);}":
      shader=="own"?"void main(){gl_FragColor=vec4(0.7,0.4,0.2,1.0);}":
      tex=="multi"||tex=="mixed"?"void main(){gl_FragColor=vec4(0.1+0.4*gl_TexCoord[0].s+0.3*gl_TexCoord[1].t,0.1+0.6*gl_TexCoord[0].t,0.1+0.3*gl_TexCoord[1].s+0.3*gl_TexCoord[2].r,1.0);}":
      "void main(){gl_FragColor=vec4(0.1+0.7*gl_TexCoord[0].s,0.1+0.7*gl_TexCoord[0].t,0.1+0.7*gl_TexCoord[0].r,1.0);}";
    prog->shaderObject.set1Value(0,vs);prog->shaderObject.set1Value(1,fs);root->addChild(prog);
  }
  if(tex!="none") {
    const unsigned char px[]={255,0,0,0,255,0,255,0,0,0,255,0,255,0,0,0,255,0,255,0,0,0,255,0};
    int units=tex=="multi"?2:tex=="mixed"?3:1;
    for(int i=0;i<units;++i){SoTextureUnit*u=new SoTextureUnit;u->unit=i;root->addChild(u);
      if(tex=="3d"||(tex=="mixed"&&i==1)){SoTexture3*t=new SoTexture3;t->images.setValue(SbVec3s(2,2,2),3,px);root->addChild(t);}
      else {SoTexture2*t=new SoTexture2;t->image.setValue(SbVec2s(2,2),3,px);root->addChild(t);}
    }
    SoTextureUnit*u=new SoTextureUnit;u->unit=0;root->addChild(u);
    if(tex=="empty"){SoTextureCoordinate2*t=new SoTextureCoordinate2;t->point.setNum(0);root->addChild(t);}
  }
  std::vector<SoCoordinate3*> coordinates;std::vector<SoFaceSet*> faces;std::vector<SoIndexedFaceSet*> indexed;
  if(shared){auto p=points(sharedpoints,320);SoCoordinate3*c=new SoCoordinate3;c->point.setValues(0,p.size(),p.data());root->addChild(c);coordinates.push_back(c);}
  std::vector<int32_t> nv(verticespershape/4,4),idx;for(int q=0;q<verticespershape/4;++q){for(int c=0;c<4;++c)idx.push_back(4*q+c);idx.push_back(-1);}
  SoVRMLCoordinate * vc=NULL;SoVRMLNormal * vn=NULL;
  if(kind=="vrml") {
    vc=new SoVRMLCoordinate;vn=new SoVRMLNormal;
    vc->ref();vn->ref();
    vc->point=coordinates[0]->point;
    std::vector<SbVec3f> nn(vc->point.getNum(),SbVec3f(-0.097590f,-0.195180f,0.975900f));
    vn->vector.setValues(0,nn.size(),nn.data());
  }
  for(int i=0;i<count;++i) {
    SoSeparator* branch=new SoSeparator;branch->renderCaching=SoSeparator::OFF;branch->boundingBoxCaching=SoSeparator::OFF;
    if(!shared){SoTranslation*t=new SoTranslation;t->translation.setValue(float(i%20)/20,float(i/20)/20,0);branch->addChild(t);SoScale*s=new SoScale;s->scaleFactor=SbVec3f(0.049f,0.049f,0.049f);branch->addChild(s);auto p=points(verticespershape,32);SoCoordinate3*c=new SoCoordinate3;c->point.setValues(0,p.size(),p.data());branch->addChild(c);coordinates.push_back(c);}
    const int first=shared?int((long long)i*(sharedpoints-verticespershape)/std::max(1,count-1))/4*4:0;
    if(kind=="point") {SoPointSet*f=new SoPointSet;f->numPoints=verticespershape;f->startIndex=first;branch->addChild(f);}
    else if(kind=="indexed-point"||kind=="line") {SoIndexedShape*f=kind=="line"?static_cast<SoIndexedShape*>(new SoIndexedLineSet):static_cast<SoIndexedShape*>(new SoIndexedPointSet);std::vector<int32_t>ci=idx;for(auto&j:ci)if(j>=0)j+=first;f->coordIndex.setValues(0,ci.size(),ci.data());branch->addChild(f);}
    else if(kind=="vrml") {SoVRMLIndexedFaceSet*f=new SoVRMLIndexedFaceSet;f->coord=vc;f->normal=vn;f->normalPerVertex=TRUE;std::vector<int32_t>ci=idx;for(auto&j:ci)if(j>=0)j+=first;f->coordIndex.setValues(0,ci.size(),ci.data());branch->addChild(f);}
    else if(ifs){SoIndexedFaceSet*f=new SoIndexedFaceSet;std::vector<int32_t> ci=idx;for(auto& j:ci)if(j>=0)j+=first;f->coordIndex.setValues(0,ci.size(),ci.data());indexed.push_back(f);branch->addChild(f);}
    else {SoFaceSet*f=new SoFaceSet;f->numVertices.setValues(0,nv.size(),nv.data());f->startIndex=first;faces.push_back(f);branch->addChild(f);}
    root->addChild(branch);
  }
  if(vc){vc->unref();vn->unref();}
  SoCallback* done=new SoCallback;done->setCallback(finish,NULL);root->addChild(done);
  {SoOffscreenRenderer renderer(SbViewportRegion(128,128));renderer.setComponents(SoOffscreenRenderer::RGB);
    std::printf("{\"phase\":\"scene\",\"rss_mib\":%.3f,\"peak_mib\":%.3f,\"points_per_source\":%d,\"vertices_per_shape\":%d,\"shapes\":%d}\n",rss(),peak(),shared?sharedpoints:verticespershape,verticespershape,count);std::fflush(stdout);
    auto render=[&](){auto b=Clock::now();if(!renderer.render(root))std::exit(3);return std::chrono::duration<double,std::milli>(Clock::now()-b).count();};
    emit("first",{render()},renderer);
    std::vector<double> times;for(int i=0;i<frames;++i)times.push_back(render());emit("static",times,renderer);
    auto edit=Clock::now();
    if(mutation=="coord") {for(auto c:coordinates){SbVec3f* p=c->point.startEditing();for(int i=0;i<c->point.getNum();++i)p[i][0]+=0.1f*std::sin(6.2831853f*p[i][1]);c->point.finishEditing();}if(vc)vc->point=coordinates[0]->point;}
    else if(!ifs){faces[0]->startIndex=faces[0]->startIndex.getValue()+4;faces[0]->numVertices.setNum(1023);}
    else {indexed[0]->coordIndex.set1Value(0,indexed[0]->coordIndex[0]+1);}
    std::printf("{\"phase\":\"edit\",\"edit_ms\":%.6f}\n",std::chrono::duration<double,std::milli>(Clock::now()-edit).count());
    emit("invalidated",{render()},renderer);times.clear();for(int i=0;i<frames;++i)times.push_back(render());emit("restabilized",times,renderer);
  }
  root->unref();SoDebugError::setHandlerCallback(NULL,NULL);SoDB::finish();return missing?4:0;
}
