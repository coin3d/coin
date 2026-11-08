#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoIndexedNurbsSurface.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoLinearProfile.h>
#include <Inventor/SbColor.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {
// Independent Cox-de Boor reference, intentionally separate from Coin's
// span-based evaluator. Used to compare the framebuffer with the field.
double basis(int i, int degree, double u, const std::vector<float> & knots)
{
  if (!degree) return knots[i] <= u && u < knots[i+1] ? 1.0 : 0.0;
  double value=0.0;
  const double a=knots[i+degree]-knots[i];
  const double b=knots[i+degree+1]-knots[i+1];
  if (a>0) value+=(u-knots[i])/a*basis(i,degree-1,u,knots);
  if (b>0) value+=(knots[i+degree+1]-u)/b*basis(i+1,degree-1,u,knots);
  return value;
}
struct Control { double x,y,z,w; SbColor color; float alpha; };
struct Reference {
  int nu,nv;
  std::vector<float> uk,vk;
  std::vector<Control> points;
  void evaluate(double u,double v,double * position,double * rgba) const {
    double denom=0;
    for (int k=0;k<3;++k) position[k]=0;
    for (int k=0;k<4;++k) rgba[k]=0;
    for (int j=0;j<nv;++j) for (int i=0;i<nu;++i) {
      const Control & p=points[j*nu+i];
      const double weight=basis(i,int(uk.size())-nu-1,u,uk)*
        basis(j,int(vk.size())-nv-1,v,vk)*p.w;
      denom+=weight;
      position[0]+=weight*p.x; position[1]+=weight*p.y; position[2]+=weight*p.z;
      for (int k=0;k<3;++k) rgba[k]+=weight*p.color[k];
      rgba[3]+=weight*p.alpha;
    }
    for (int k=0;k<3;++k) position[k]/=denom;
    for (int k=0;k<4;++k) rgba[k]/=denom;
  }
};
int render(const std::string & name,const char * imagepath)
{
  const bool indexed=name.find("indexed")!=std::string::npos;
  const bool rational=name.find("rational")!=std::string::npos;
  const bool cubic=name.find("cubic")!=std::string::npos;
  const bool alpha=name.find("alpha")!=std::string::npos;
  const bool material=name.find("material")!=std::string::npos;
  const bool texture=name.find("texture")!=std::string::npos;
  const bool trim=name.find("trim")!=std::string::npos;
  const bool nonclamped=name.find("nonclamped")!=std::string::npos;
  const bool overall=name.find("overall")!=std::string::npos;
  const bool low=name.find("low")!=std::string::npos;
  const bool medium=name.find("medium")!=std::string::npos;
  const bool screen=name.find("screen")!=std::string::npos;
  Reference ref;
  ref.nu=ref.nv=cubic?4:2;
  ref.uk=ref.vk=cubic?std::vector<float>{0,0,0,0,1,1,1,1}:
                              std::vector<float>{0,0,1,1};
  if (name.find("nonuniform")!=std::string::npos) {
    ref.nu=6; ref.nv=5;
    ref.uk={0,0,0,0,0.2f,0.7f,1,1,1,1};
    ref.vk={0,0,0,0,0.35f,1,1,1,1};
  }
  if (name.find("repeated")!=std::string::npos) {
    ref.nu=7; ref.nv=4;
    ref.uk={0,0,0,0,0.4f,0.4f,0.75f,1,1,1,1};
    ref.vk={0,0,0,0,1,1,1,1};
  }
  if (nonclamped) {
    ref.nu=ref.nv=4;
    ref.uk=ref.vk={-3,-2,-1,0,1,2,3,4};
  }
  if (name.find("domain")!=std::string::npos) {
    for (size_t i=0;i<ref.uk.size();++i) ref.uk[i]=2+4*ref.uk[i];
    for (size_t i=0;i<ref.vk.size();++i) ref.vk[i]=-3+5*ref.vk[i];
  }
  const int count=ref.nu*ref.nv;
  std::vector<SbVec3f> coords(count);
  std::vector<SbVec4f> coords4(count);
  std::vector<uint32_t> colors(count);
  std::vector<int32_t> indices(count);
  std::vector<float> transparencies(count);
  for (int j=0;j<ref.nv;++j) for (int i=0;i<ref.nu;++i) {
    const int n=j*ref.nu+i;
    const float u=float(i)/(ref.nu-1),v=float(j)/(ref.nv-1);
    Control p;
    p.x=-1+2*u; p.y=-1+2*v;
    p.z=cubic?0.6f*std::sin(3.14159265f*u)*std::sin(3.14159265f*v):0;
    p.w=rational?(n%3==0?4:1):1;
    const SbColor c=cubic?SbColor(0.1f+0.8f*(i%2),v,0.1f+0.8f*((i+j)%2)):
                            SbColor(1-u-v+2*u*v,u,v);
    const uint32_t packed=c.getPackedValue(alpha?0.7f-0.6f*u:0);
    float transparency;
    p.color.setPackedValue(packed,transparency); p.alpha=1-transparency;
    if (material) {
      p.color=SbColor(0.3f,0.6f,0.9f);
      p.alpha=0.3f+0.6f*u;
    }
    ref.points.push_back(p);
    // Reverse the backing arrays so indexed tests cannot pass accidentally
    // by treating the index array as the identity.
    const int dest=indexed?count-1-n:n;
    indices[n]=dest;
    coords[dest]=SbVec3f(p.x,p.y,p.z);
    coords4[dest]=SbVec4f(p.x*p.w,p.y*p.w,p.z*p.w,p.w);
    colors[dest]=packed; transparencies[dest]=1-p.alpha;
  }
  SoSeparator *root=new SoSeparator; root->ref();
  SoOrthographicCamera *camera=new SoOrthographicCamera;
  camera->position.setValue(0,0,3); camera->height=2.2f; root->addChild(camera);
  SoLightModel *light=new SoLightModel;
  light->model=SoLightModel::BASE_COLOR; root->addChild(light);
  SoComplexity *complexity=new SoComplexity;
  complexity->value=low?0.1f:medium?0.7f:1.0f;
  complexity->type=screen?SoComplexity::SCREEN_SPACE:SoComplexity::OBJECT_SPACE;
  root->addChild(complexity);
  SoVertexProperty *vp=new SoVertexProperty;
  if (!rational) vp->vertex.setValues(0,count,coords.data());
  if (!material) vp->orderedRGBA.setValues(0,count,colors.data());
  vp->materialBinding=overall?SoMaterialBinding::OVERALL:
    name.find("binding")!=std::string::npos?SoMaterialBinding::PER_VERTEX_INDEXED:
                                          SoMaterialBinding::PER_VERTEX;
  root->addChild(vp);
  if (material) {
    SoMaterial *m=new SoMaterial;
    m->diffuseColor.setValue(0.3f,0.6f,0.9f);
    m->transparency.setValues(0,count,transparencies.data());root->addChild(m);
    // VertexProperty does not apply its material binding without packed colors.
    SoMaterialBinding *binding=new SoMaterialBinding;
    binding->value=SoMaterialBinding::PER_VERTEX;root->addChild(binding);
  }
  if (rational) {
    SoCoordinate4 *c4=new SoCoordinate4;
    c4->point.setValues(0,count,coords4.data()); root->addChild(c4);
  }
  const float umin=ref.uk[ref.uk.size()-ref.nu-1],umax=ref.uk[ref.nu];
  const float vmin=ref.vk[ref.vk.size()-ref.nv-1],vmax=ref.vk[ref.nv];
  const float textureUk[4]={umin,umin,umax,umax};
  const float textureVk[4]={vmin,vmin,vmax,vmax};
  if (texture) {
    SoTexture2 *tex=new SoTexture2;
    const unsigned char texels[12]={255,0,0, 0,255,0, 0,0,255, 255,255,255};
    tex->image.setValue(SbVec2s(2,2),3,texels);root->addChild(tex);
    SoTextureCoordinate2 *tc=new SoTextureCoordinate2;
    const SbVec2f coords[4]={SbVec2f(0.75f,0.75f),SbVec2f(0.75f,0.75f),
                            SbVec2f(0.75f,0.75f),SbVec2f(0.75f,0.75f)};
    tc->point.setValues(0,4,coords);root->addChild(tc);
  }
  if (trim) {
    SoProfileCoordinate2 *profilecoords=new SoProfileCoordinate2;
    const float a=umin+0.1f*(umax-umin),b=umin+0.9f*(umax-umin);
    const float c=vmin+0.1f*(vmax-vmin),d=vmin+0.9f*(vmax-vmin);
    const SbVec2f coords[5]={SbVec2f(a,c),SbVec2f(b,c),SbVec2f(b,d),
                            SbVec2f(a,d),SbVec2f(a,c)};
    profilecoords->point.setValues(0,5,coords);root->addChild(profilecoords);
    SoLinearProfile *profile=new SoLinearProfile;
    const int32_t index[5]={0,1,2,3,4};
    profile->index.setValues(0,5,index);root->addChild(profile);
  }
  if (indexed) {
    SoIndexedNurbsSurface *surface=new SoIndexedNurbsSurface;
    surface->numUControlPoints=ref.nu; surface->numVControlPoints=ref.nv;
    surface->coordIndex.setValues(0,count,indices.data());
    if (texture) {
      surface->numSControlPoints=surface->numTControlPoints=2;
      surface->sKnotVector.setValues(0,4,textureUk);
      surface->tKnotVector.setValues(0,4,textureVk);
    }
    surface->uKnotVector.setValues(0,ref.uk.size(),ref.uk.data());
    surface->vKnotVector.setValues(0,ref.vk.size(),ref.vk.data()); root->addChild(surface);
  }
  else {
    SoNurbsSurface *surface=new SoNurbsSurface;
    if (texture) {
      surface->numSControlPoints=surface->numTControlPoints=2;
      surface->sKnotVector.setValues(0,4,textureUk);
      surface->tKnotVector.setValues(0,4,textureVk);
    }
    surface->numUControlPoints=ref.nu; surface->numVControlPoints=ref.nv;
    surface->uKnotVector.setValues(0,ref.uk.size(),ref.uk.data());
    surface->vKnotVector.setValues(0,ref.vk.size(),ref.vk.data()); root->addChild(surface);
  }
  glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  double maximumerror=0;
  {
    SoGLRenderAction action(SbViewportRegion(128,128));
    action.setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
    action.setTransparencyType(SoGLRenderAction::BLEND);
    action.apply(root); glFinish();
    const int first=nonclamped?52:24,last=nonclamped?76:104,step=nonclamped?12:20;
    for (int y=first;y<=last;y+=step) for (int x=first;x<=last;x+=step) {
      const double targetx=(x+0.5)/128*2.2-1.1;
      const double targety=(y+0.5)/128*2.2-1.1;
      const double umin=ref.uk[ref.uk.size()-ref.nu-1],umax=ref.uk[ref.nu];
      const double vmin=ref.vk[ref.vk.size()-ref.nv-1],vmax=ref.vk[ref.nv];
      double u=(umin+umax)/2,v=(vmin+vmax)/2;
      double pos[3],rgba[4];
      // Invert only x/y of this graph surface to locate each tested pixel.
      // Finite differences avoid sharing derivative code with production.
      for (int iter=0;iter<80;++iter) {
        ref.evaluate(u,v,pos,rgba);
        const double ex=targetx-pos[0],ey=targety-pos[1];
        if (std::fabs(ex)+std::fabs(ey)<1e-10) break;
        const double du=(umax-umin)*1e-6,dv=(vmax-vmin)*1e-6;
        double pu[3],pv[3],unused[4];
        ref.evaluate(u+du,v,pu,unused); ref.evaluate(u,v+dv,pv,unused);
        const double a=(pu[0]-pos[0])/du,b=(pv[0]-pos[0])/dv;
        const double c=(pu[1]-pos[1])/du,d=(pv[1]-pos[1])/dv;
        const double determinant=a*d-b*c;
        const double stepU=(d*ex-b*ey)/determinant;
        const double stepV=(a*ey-c*ex)/determinant;
        const double scale=std::min(1.0,std::min(
          0.1*(umax-umin)/(std::fabs(stepU)+1e-30),
          0.1*(vmax-vmin)/(std::fabs(stepV)+1e-30)));
        u+=scale*stepU; v+=scale*stepV;
        u=std::max(umin+1e-5,std::min(umax-1e-5,u));
        v=std::max(vmin+1e-5,std::min(vmax-1e-5,v));
      }
      ref.evaluate(u,v,pos,rgba);
      if (std::fabs(pos[0]-targetx)+std::fabs(pos[1]-targety)>1e-5) {
        std::fprintf(stderr,"Reference inversion failed\n"); root->unref(); return 2;
      }
      if (overall) {
        SbColor c;float transparency;c.setPackedValue(colors[0],transparency);
        for (int k=0;k<3;++k) rgba[k]=c[k]; rgba[3]=1-transparency;
      }
      unsigned char pixel[3]; glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
      for (int k=0;k<3;++k) {
        const bool outside=trim && (u<umin+0.1*(umax-umin) ||
          u>umin+0.9*(umax-umin) || v<vmin+0.1*(vmax-vmin) || v>vmin+0.9*(vmax-vmin));
        const double expected=outside?0:255*rgba[k]*(alpha?rgba[3]:1);
        maximumerror=std::max(maximumerror,std::fabs(pixel[k]-expected));
      }
    }
    if (imagepath) {
      std::vector<unsigned char> pixels(128*128*3);
      glPixelStorei(GL_PACK_ALIGNMENT,1);
      glReadPixels(0,0,128,128,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());
      FILE *file=std::fopen(imagepath,"wb");
      if (!file) { root->unref(); return 2; }
      std::fprintf(file,"P6\n128 128\n255\n");
      for (int y=127;y>=0;--y) std::fwrite(pixels.data()+y*128*3,1,128*3,file);
      std::fclose(file);
    }
  }
  root->unref();
  std::printf("%s: maximum RGB error = %.3f / 255\n",name.c_str(),maximumerror);
  return maximumerror<=(low?35:medium?10:5)?0:1;
}
}

// Requires a real GLX display, for example xvfb-run -a.
int main(int argc,char **argv)
{
  Display *dpy=XOpenDisplay(NULL);
  if (!dpy) { std::puts("no X display"); return 2; }
  int attrs[]={GLX_RGBA,GLX_RED_SIZE,8,GLX_GREEN_SIZE,8,GLX_BLUE_SIZE,8,GLX_DEPTH_SIZE,16,None};
  XVisualInfo *vi=glXChooseVisual(dpy,DefaultScreen(dpy),attrs);
  if (!vi) { XCloseDisplay(dpy); return 2; }
  Colormap cmap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone);
  XSetWindowAttributes swa={};swa.colormap=cmap;swa.event_mask=ExposureMask;
  Window win=XCreateWindow(dpy,RootWindow(dpy,vi->screen),0,0,128,128,0,
    vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,&swa);
  XMapWindow(dpy,win);XSync(dpy,False);
  GLXContext ctx=glXCreateContext(dpy,vi,NULL,True);
  if (!ctx||!glXMakeCurrent(dpy,win,ctx)) { std::puts("GLX context creation failed");return 2; }
  glViewport(0,0,128,128);glDisable(GL_DITHER);SoDB::init();
  const int result=render(argc>1?argv[1]:"bilinear",argc>2?argv[2]:NULL);
  glXMakeCurrent(dpy,None,NULL);glXDestroyContext(dpy,ctx);
  XDestroyWindow(dpy,win);XFreeColormap(dpy,cmap);XFree(vi);XCloseDisplay(dpy);
  return result;
}
