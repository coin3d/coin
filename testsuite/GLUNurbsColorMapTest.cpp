/**************************************************************************\\
 * Copyright (c) 2026 FreeCAD contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\\**************************************************************************/

#include "CoinTest.h"

#include <cmath>

#define COIN_INTERNAL
#include "config.h"
#include "glue/GLUWrapper.h"

#ifdef HAVE_CGL
#include <OpenGL/OpenGL.h>
#endif

namespace {
#ifdef HAVE_CGL
// Apple's GLU needs a current context even when only tessellation callbacks
// are requested. Keep that requirement local to this test and restore the
// context that the rest of CoinTests had before it ran.
class CurrentCGLContext {
public:
  CurrentCGLContext() : previous(CGLGetCurrentContext()), context(NULL),
                        current(false) {
    const CGLPixelFormatAttribute attributes[] = {
      static_cast<CGLPixelFormatAttribute>(0)
    };
    CGLPixelFormatObj format = NULL;
    GLint count = 0;
    if (CGLChoosePixelFormat(attributes, &format, &count) == kCGLNoError && format) {
      if (CGLCreateContext(format, NULL, &context) == kCGLNoError && context) {
        current = CGLSetCurrentContext(context) == kCGLNoError;
      }
    }
    if (format) CGLDestroyPixelFormat(format);
  }
  ~CurrentCGLContext() {
    if (context) {
      CGLSetCurrentContext(previous);
      CGLDestroyContext(context);
    }
  }
  bool isCurrent() const { return current; }
private:
  CGLContextObj previous;
  CGLContextObj context;
  bool current;
};
#endif

struct ColorSamples {
  unsigned int count;
  bool hasInterpolatedCenter;
};

void APIENTRY colorCallback(float *color, void *data)
{
  ColorSamples *samples = static_cast<ColorSamples *>(data);
  ++samples->count;
  if (std::fabs(color[0] - 0.5f) < 0.02f &&
      std::fabs(color[1] - 0.5f) < 0.02f &&
      std::fabs(color[2] - 0.5f) < 0.02f &&
      std::fabs(color[3] - 1.0f) < 0.02f) {
    samples->hasInterpolatedCenter = true;
  }
}

void APIENTRY beginCallback(GLenum, void *) {}
void APIENTRY vertexCallback(float *, void *) {}
void APIENTRY endCallback(void *) {}
}

BOOST_AUTO_TEST_SUITE(GLUNurbsColorMap_TestSuite)

BOOST_AUTO_TEST_CASE(interpolatesControlColorsWithSurfaceBasis)
{
  const GLUWrapper_t *glu = GLUWrapper();
  BOOST_REQUIRE(glu != NULL);
  if (!glu->available || !glu->versionMatchesAtLeast(1, 3, 0) ||
      !glu->gluNewNurbsRenderer || !glu->gluDeleteNurbsRenderer ||
      !glu->gluNurbsProperty || !glu->gluBeginSurface || !glu->gluEndSurface ||
      !glu->gluNurbsSurface || !glu->gluNurbsCallback || !glu->gluNurbsCallbackData ||
      !glu->gluLoadSamplingMatrices) {
    return;
  }

#ifdef HAVE_CGL
  CurrentCGLContext context;
  BOOST_REQUIRE(context.isCurrent());
#endif

  const GLfloat knots[] = {0.0f, 0.0f, 1.0f, 1.0f};
  const GLfloat points[] = {
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f
  };
  const GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
  };
  ColorSamples samples = {0, false};
  void *renderer = glu->gluNewNurbsRenderer();
  BOOST_REQUIRE(renderer != NULL);

  // Sampling must not depend on the current context's default GL matrices.
  const GLfloat identity[16] = {
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1
  };
  const GLint viewport[4] = {0, 0, 64, 64};
  glu->gluNurbsProperty(renderer, GLU_AUTO_LOAD_MATRIX, GL_FALSE);
  glu->gluLoadSamplingMatrices(renderer, identity, identity, viewport);
  glu->gluNurbsProperty(renderer, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
  glu->gluNurbsProperty(renderer, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
  glu->gluNurbsProperty(renderer, GLU_U_STEP, 8.0f);
  glu->gluNurbsProperty(renderer, GLU_V_STEP, 8.0f);
  glu->gluNurbsCallback(renderer, GLU_NURBS_BEGIN_DATA,
                        reinterpret_cast<gluNurbsCallback_cb_t>(beginCallback));
  glu->gluNurbsCallback(renderer, GLU_NURBS_VERTEX_DATA,
                        reinterpret_cast<gluNurbsCallback_cb_t>(vertexCallback));
  glu->gluNurbsCallback(renderer, GLU_NURBS_COLOR_DATA,
                        reinterpret_cast<gluNurbsCallback_cb_t>(colorCallback));
  glu->gluNurbsCallback(renderer, GLU_NURBS_END_DATA,
                        reinterpret_cast<gluNurbsCallback_cb_t>(endCallback));
  glu->gluNurbsCallbackData(renderer, &samples);
  glu->gluBeginSurface(renderer);
  glu->gluNurbsSurface(renderer, 4, const_cast<GLfloat *>(knots),
                       4, const_cast<GLfloat *>(knots), 3, 6,
                       const_cast<GLfloat *>(points), 2, 2, GL_MAP2_VERTEX_3);
  glu->gluNurbsSurface(renderer, 4, const_cast<GLfloat *>(knots),
                       4, const_cast<GLfloat *>(knots), 4, 8,
                       const_cast<GLfloat *>(colors), 2, 2, GL_MAP2_COLOR_4);
  glu->gluEndSurface(renderer);
  glu->gluDeleteNurbsRenderer(renderer);

  BOOST_CHECK(samples.count > 0u);
  BOOST_CHECK(samples.hasInterpolatedCenter);
}

BOOST_AUTO_TEST_SUITE_END()
