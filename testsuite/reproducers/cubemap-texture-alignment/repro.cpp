// Reproducer for a missing glPixelStorei(GL_UNPACK_ALIGNMENT, 1) in
// SoGLCubeMapImage::getGLDisplayList() (src/rendering/SoGLCubeMapImage.cpp):
// unlike SoGLImage.cpp's regular 2D texture upload, the cubemap upload path
// calls glTexImage2D() directly without ever setting the unpack alignment,
// so it's stuck with whatever the context's default (or a previous, unrelated
// draw call's) GL_UNPACK_ALIGNMENT happens to be -- 4 on a fresh context.
//
// A cubemap face image whose row byte-width (width * bytes-per-pixel) isn't
// a multiple of 4 -- e.g. any 3-component (RGB) face with an odd width, or
// this repro's minimal 2x2 RGB case (2*3 = 6 bytes/row) -- then has each row
// read with the wrong stride: the driver rounds 6 up to 8 for a 4-byte
// alignment, so every row after the first is read 2 bytes short of where it
// actually starts, and the final row reads 2 bytes past the end of the
// buffer entirely.
//
// This renders a plain SoCube with an SoTextureCubeMap whose six faces are
// each a solid, distinct 2x2 color (unlit, so the rendered pixel is exactly
// the sampled texture color with no lighting to obscure the effect) and
// checks that a large sample of pixels across the visible faces are *exact*
// matches for one of the six expected colors -- with the bug, misaligned
// reads corrupt a chunk of the uploaded texel data, so sampled colors drift
// away from the expected six.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>

static void setSolid(SoSFImage & field, unsigned char r, unsigned char g, unsigned char b)
{
  // 2x2 RGB = 6 bytes/row: not a multiple of 4, the shape that
  // triggers the bug.
  unsigned char buf[2*2*3];
  for (int i = 0; i < 4; i++) {
    buf[i*3+0] = r; buf[i*3+1] = g; buf[i*3+2] = b;
  }
  field.setValue(SbVec2s(2, 2), 3, buf);
}

int main()
{
  SoDB::init();

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoOrthographicCamera * cam = new SoOrthographicCamera;
  cam->position.setValue(0, 0, 5);
  cam->nearDistance = 1.0f;
  cam->farDistance = 10.0f;
  cam->height = 3.0f;
  root->addChild(cam);

  SoLightModel * lm = new SoLightModel;
  lm->model = SoLightModel::BASE_COLOR;
  root->addChild(lm);

  SoMaterial * mat = new SoMaterial;
  mat->diffuseColor.setValue(1, 1, 1); // avoid MODULATE's default (0.8,0.8,0.8) dimming
  root->addChild(mat);

  struct { unsigned char r, g, b; } colors[6] = {
    {255,0,0}, {0,255,0}, {255,255,0}, {128,0,0}, {0,128,0}, {128,128,0},
  };

  SoTextureCubeMap * cm = new SoTextureCubeMap;
  setSolid(cm->imagePosX, colors[0].r, colors[0].g, colors[0].b);
  setSolid(cm->imageNegX, colors[1].r, colors[1].g, colors[1].b);
  setSolid(cm->imagePosY, colors[2].r, colors[2].g, colors[2].b);
  setSolid(cm->imageNegY, colors[3].r, colors[3].g, colors[3].b);
  setSolid(cm->imagePosZ, colors[4].r, colors[4].g, colors[4].b);
  setSolid(cm->imageNegZ, colors[5].r, colors[5].g, colors[5].b);
  root->addChild(cm);

  root->addChild(new SoCube);

  SbViewportRegion vp(256, 256);
  SoOffscreenRenderer renderer(vp);
  renderer.setBackgroundColor(SbColor(0.2f, 0.2f, 0.2f));
  if (!renderer.render(root)) {
    fprintf(stderr, "[repro] render() failed\n");
    return 2;
  }

  unsigned char * buf = renderer.getBuffer();
  int w = 256, h = 256;
  int total = 0, matched = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      unsigned char * px = buf + (y*w + x)*3;
      // skip background pixels (0.2,0.2,0.2 ~= 51,51,51)
      if (px[0] < 60 && px[1] < 60 && px[2] < 60) continue;
      total++;
      for (int c = 0; c < 6; c++) {
        if (px[0]==colors[c].r && px[1]==colors[c].g && px[2]==colors[c].b) {
          matched++;
          break;
        }
      }
    }
  }

  double frac = total ? (double)matched / total : 0.0;
  fprintf(stderr, "[repro] %d/%d cube pixels are an exact expected color (%.1f%%)\n",
          matched, total, frac * 100.0);

  root->unref();

  // Before the fix: misaligned reads corrupt a large fraction of the
  // uploaded texel data, so well under 100% of pixels match one of the
  // six exact expected colors. After the fix: every cube pixel is an
  // exact match (100%), since each 2x2 face is a single solid color
  // with no lighting or interpolation to blur it -- any exact match
  // failure is memory corruption, not sampling.
  if (frac < 0.999) {
    fprintf(stderr, "[repro] FAIL: expected ~100%% exact matches\n");
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
