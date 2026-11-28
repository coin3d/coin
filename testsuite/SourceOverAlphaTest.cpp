// Source-over blending must store alpha as a + dst*(1 - a), not a*a + dst*(1 - a).
// Renders a square with 50% transparency over a cleared (alpha 0) background
// and checks the alpha read back from the offscreen buffer.
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstdio>

static int check(SoGLRenderAction::TransparencyType type, const char * name, SoNode * root)
{
  const int size = 32;
  SbViewportRegion vp(size, size);
  SoOffscreenRenderer r(vp);
  r.setComponents(SoOffscreenRenderer::RGB_TRANSPARENCY);
  r.setBackgroundColor(SbColor(0, 0, 0));
  r.getGLRenderAction()->setTransparencyType(type);
  if (!r.render(root)) { std::printf("SKIP: offscreen render unavailable\n"); return 77; }
  const unsigned char * px = r.getBuffer() + ((size / 2) * size + size / 2) * 4;
  std::printf("%s: rgba %d %d %d %d\n", name, px[0], px[1], px[2], px[3]);
  if (px[0] == 0) { std::printf("SKIP: nothing drawn\n"); return 77; }
  // Alpha 0.5 over alpha 0 must store ~128; squared alpha gives ~64.
  if (px[3] < 118 || px[3] > 138) {
    std::printf("FAIL: %s alpha %d, expected ~128\n", name, px[3]);
    return 1;
  }
  return 0;
}

int main()
{
  SoDB::init();
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoOrthographicCamera * cam = new SoOrthographicCamera;
  cam->height = 2;
  SoLightModel * lm = new SoLightModel;
  lm->model = SoLightModel::BASE_COLOR;
  SoMaterial * mat = new SoMaterial;
  mat->diffuseColor.setValue(1, 0, 0);
  mat->transparency = 0.5f;
  SoCoordinate3 * coords = new SoCoordinate3;
  const float pts[4][3] = {{-0.8f, -0.8f, 0}, {0.8f, -0.8f, 0}, {0.8f, 0.8f, 0}, {-0.8f, 0.8f, 0}};
  coords->point.setValues(0, 4, pts);
  root->addChild(cam);
  root->addChild(lm);
  root->addChild(mat);
  root->addChild(coords);
  root->addChild(new SoFaceSet);

  const struct { SoGLRenderAction::TransparencyType type; const char * name; } cases[] = {
    {SoGLRenderAction::BLEND, "BLEND"},
    {SoGLRenderAction::DELAYED_BLEND, "DELAYED_BLEND"},
    {SoGLRenderAction::SORTED_OBJECT_BLEND, "SORTED_OBJECT_BLEND"}};
  int result = 0;
  for (const auto & c : cases) {
    const int rc = check(c.type, c.name, root);
    if (rc == 77) { root->unref(); return 77; }
    if (rc) result = 1;
  }
  root->unref();
  return result;
}
