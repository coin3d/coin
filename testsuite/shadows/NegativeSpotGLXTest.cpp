// Reuse S1's public-API GLX fixture; no production source or private shader seam.
#define COIN_SHADOW_GLX_FIXTURE_ONLY
#include "DirectionalGLXTest.cpp"

namespace {
Options negative(float q) {
  Options o(q); o.directional = 0; o.spots = 1; o.drop = -1;
  o.spotIntensity = 0.7f; o.explicitSpotRange = true;
  return o;
}
void nearValue(double value, double expected, double tolerance, const char * message) {
  std::printf("  oracle actual=%.3f expected=%.3f tolerance=%.3f\n",value,expected,tolerance);
  check(std::fabs(value-expected) <= tolerance,message);
}
void stages(const Result & r, float q, bool hasNegative) {
  const bool low = double(q) <= .3;
  // Check the source fetched from attached, compiled GL shader objects, after
  // the fixture has already required an actually linked receiver program.
  check((r.vertex.find("float DirSpotLight(") != std::string::npos) == (low && hasNegative),
        "vertex DirSpotLight definition is registered exactly when selected");
  check((r.fragment.find("float DirSpotLight(") != std::string::npos) == (!low && hasNegative),
        "fragment DirSpotLight definition is registered exactly when selected");
}
void diffuseOracle(float q) {
  Options o = negative(q); o.cutoff = 0.25f;
  label = "negative diffuse outside regular cone"; Result r = render(o); stages(r,q,true);
  // The cone's z=0 radius is 6*tan(.25)=1.532. Each 5x5 patch
  // centered at (+/-1.3,1.3) lies OUTSIDE that circle but INSIDE the
  // perspective map's square [-1.532,+1.532]^2. This isolates absence
  // of cone attenuation without asserting anything about texture borders.
  for (int side = -1; side <= 1; side += 2)
    nearValue(r.patch(side*1.3f,1.3f),255*.7,5,"negative light has no spot cone or positional diffuse falloff");
  label = "negative oblique normal and light direction";
  o.cutoff = 0.75f; o.normal.setValue(.6f,0,.8f); o.spotDirection.setValue(.3f,0,-std::sqrt(.91f));
  r = render(o);
  const double expected = 255*.7*(-.6*.3+.8*std::sqrt(.91));
  nearValue(r.patch(-1.5f),expected,5,"diffuse equals independent geometric dot product");
  nearValue(r.patch(1.5f),expected,5,"diffuse does not point toward positional light origin");
  label = "negative attenuation and world translation";
  o.attenuation.setValue(.2f,.1f,.02f); o.offset = 11; o.cameraDistance = 17;
  r = render(o);
  nearValue(r.patch(1.5f),expected,5,"negative diffuse ignores distance attenuation and translation");
  label = "negative reverse diffuse"; o.normal = -o.normal; r = render(o);
  check(r.patch(1.5f) < 5,"negative light direction sign is geometrically correct");
}
void signs(float q) {
  const float tiny = std::nextafter(0.0f,1.0f);
  const float drops[] = {-1,-tiny,0.0f,-0.0f,tiny,.7f};
  for (int i = 0; i < 6; ++i) {
    label = "drop sign including signed zero and immediate neighbors";
    Options o = negative(q); o.cutoff = .25f; o.drop = drops[i];
    Result r = render(o); stages(r,q,i<2);
    if (i<2) nearValue(r.patch(1.3f,1.3f),255*.7,5,"every finite strictly negative drop selects legacy directional spot");
    else check(r.patch(1.3f,1.3f)<5,"zero, negative zero and positive drop retain the regular spot cone");
  }
  // Same node, same group and action: exercise notify in both directions.
  for (int i = 0; i < 2; ++i) {
    label = "drop notify rebuilds selected shader";
    Options o = negative(q); o.cutoff=.25f; o.notifyDrop=true;
    o.initialDrop = i ? -1 : 0; o.drop = i ? 0 : -1;
    Result r = render(o); stages(r,q,!i);
    if (i) check(r.patch(1.3f,1.3f)<5,"notify negative to zero restores cone");
    else nearValue(r.patch(1.3f,1.3f),255*.7,5,"notify zero to negative removes cone");
  }
}
void radial(float q) {
  Options o = negative(q); o.shallowMap=true; o.cutoff=.5f;
  // Spot VSM uses farDistance/cos(2*cutoff). With .5 radians and
  // farDistance=12 the effective range is ~22.2, so epsilon=1e-5 has
  // distance sigma ~.070. The .27 radial depth separation below is
  // well above that floor. All samples lie inside this spot frustum.
  label = "radial spot receiver with its plane in producer";
  Result clear=render(o);
  nearValue(clear.patch(2.4f),255*.7,8,"off-axis plane must not shadow itself");
  label = "radial spot shallow off-axis blocker";
  o.blocker=true; Result blocked=render(o);
  // Light (0,0,6), caster center (-2.25,0,.2), top z=.25.
  // The ray to receiver x=-2.4 crosses z=.25 at x=-2.3: inside
  // cube [-2.65,-1.85]. Its radial depth is ~6.19; receiver depth
  // sqrt(36+2.4^2)=6.46. A planar receiver depth of 6 is IN FRONT
  // of that radial caster and incorrectly loses this shadow.
  check(blocked.patch(-2.4f)+65 < clear.patch(-2.4f),
        "off-axis shallow occluder is shadowed using radial receiver depth");
  std::printf("  radial clear=%.2f blocked=%.2f lit=%.2f\n",
              clear.patch(-2.4f),blocked.patch(-2.4f),blocked.patch(2.4f));
  nearValue(blocked.patch(2.4f),clear.patch(2.4f),8,
            "geometrically unoccluded point rejects planar producer with radial receiver");
  // Central projection, rather than parallel -direction rays, displaces the
  // blocker's footprint: at z=0, x scales by 6/(6-z). For the ordinary
  // elevated S1 cube, left edge -2.3 at its top z=1.3 projects to -2.936.
  // Thus x=-2.65 is shadowed although outside the cube's parallel footprint.
  label = "perspective spatial shadow coverage";
  o.shallowMap=false; Result perspective=render(o);
  check(perspective.patch(-2.65f)+55 < perspective.patch(1.5f),
        "negative lighting keeps the perspective spot shadow-camera footprint");
  std::printf("  perspective footprint=%.2f lit=%.2f\n",perspective.patch(-2.65f),perspective.patch(1.5f));
  label = "radial spot translated and deep eye";
  o.shallowMap=true; o.offset=11; o.cameraDistance=17; Result moved=render(o);
  check(moved.patch(-2.4f)+65<moved.patch(2.4f),"radial occlusion survives world and eye translation");
  nearValue(moved.patch(2.4f),clear.patch(2.4f),8,"translated radial lit sample is stable");
}
void specularOracle(float q) {
  label="negative specular response"; Options o=negative(q);
  o.diffuse=0; o.specular=.3f; o.shininess=0;
  Result r=render(o);
  // With shininess zero, Blinn-Phong's positive-facing specular factor is
  // exactly one. This oracle is independent of vertex/fragment interpolation.
  nearValue(r.patch(1.5f),255*.7*.3,5,"negative light retains material specular contribution");
  if (double(q)>.3) {
    label="negative oblique Blinn-Phong specular";
    o.shininess=.25f; o.spotDirection.setValue(.4f,0,-std::sqrt(.84f));
    r=render(o);
    // Pixel patch is centered at x=0,y=0. The <=5 intensity tolerance covers
    // its finite 5x5 footprint, 8-bit quantization and raster pixel centers.
    const double hz=std::sqrt((1+std::sqrt(.84))/2);
    nearValue(r.patch(0),255*.7*.3*std::pow(hz,32),5,"specular uses -spotDirection and eye half-vector");
  }
}
void mixed(float q) {
  for (int tex=0;tex<=2;++tex) {
    label="negative regular directional inherited and texture indices";
    Options o=negative(q); o.directional=1; o.regularSpot=true;
    o.inherited=2; o.inheritedSpot=true; o.textures=tex; o.cutoff=.25f;
    o.spotIntensity=.2f;
    Result r=render(o); stages(r,q,true);
    // The regular shadow spot and inherited ordinary spot are outside their
    // cones here. Only directional (.35) plus negative (.2) contribute.
    nearValue(r.patch(1.3f,1.3f),255*(.35+.2),7,"light indices preserve negative and directional contributions only");
  }
  label="ordinary inherited negative spot remains regular";
  Options inherited(q); inherited.directional=0; inherited.spots=1;
  inherited.spotIntensity=0; inherited.inheritedSpot=true;
  inherited.cutoff=.25f;
  Result r=render(inherited); stages(r,q,false);
  check(r.patch(1.3f,1.3f)<5,"ordinary inherited negative spot does not acquire shadow-only extension");
  label="base SoSpotLight subclass path";
  Options base=negative(q); base.baseSpot=true; Result b=render(base);
  nearValue(b.patch(1.5f),255*.7,10,"base SoSpotLight supports negative drop inside shadow group");
}
void negativeBoundaries() {
  const float a=.3f,b=.7f;
  const float qs[]={0,std::nextafter(a,0.0f),a,std::nextafter(a,1.0f),.5f,
    std::nextafter(b,0.0f),b,std::nextafter(b,1.0f),1};
  std::vector<Result> rs;
  for (int i=0;i<9;++i) {
    label="negative effective float quality boundary";
    Options o=negative(qs[i]); o.cutoff=.25f;
    rs.push_back(render(o)); stages(rs.back(),qs[i],true);
    nearValue(rs.back().patch(1.3f,1.3f),255*.7,5,"negative mode remains illuminated at quality boundaries");
  }
  const int branch[]={0,0,4,4,4,4,4,8,8};
  for(int i=0;i<9;++i)
    check(rs[i].vertex==rs[branch[i]].vertex && rs[i].fragment==rs[branch[i]].fragment,
          "float quality retains comparisons against existing double .3 and .7 limits");
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
  if (mode == "all" || mode == "low") {
    diffuseOracle(0); signs(0); radial(0); specularOracle(0); mixed(0);
  }
  if (mode == "all" || mode == "medium") {
    diffuseOracle(.5f); signs(.5f); radial(.5f); specularOracle(.5f); mixed(.5f);
  }
  if (mode == "all" || mode == "high") {
    diffuseOracle(1); signs(1); radial(1); specularOracle(1); mixed(1);
  }
  if (mode == "all" || mode == "boundaries") negativeBoundaries();
  SoDB::finish();
  glXMakeCurrent(display,None,NULL); glXDestroyContext(display,context);
  XDestroyWindow(display,window); XFreeColormap(display,colormap); XFree(visual); XCloseDisplay(display);
  std::printf("Native shadow checks: %d failure(s)\n",failures);
  return failures ? 1 : 0;
}
