/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include "config.h"
#include "glue/glp.h"

#include <Inventor/SoDB.h>
#include <Inventor/C/glue/gl.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

const char * const ENV2 = "COIN_MAXIMUM_TEXTURE2_SIZE";
const char * const ENV3 = "COIN_MAXIMUM_TEXTURE3_SIZE";

int
fail(const char * expression, int line)
{
  std::fprintf(stderr, "FAIL line %d: %s\n", line, expression);
  return 1;
}

#define REQUIRE(expression) do { if (!(expression)) return fail(#expression, __LINE__); } while (0)

struct GLXFixture {
  GLXFixture()
    : display(NULL), visual(NULL), context(NULL), colormap(0), window(0), glue(NULL)
  {
  }

  bool open()
  {
    this->display = XOpenDisplay(NULL);
    if (!this->display) return false;

    int attributes[] = { GLX_RGBA, None };
    this->visual = glXChooseVisual(this->display,
                                   DefaultScreen(this->display), attributes);
    if (!this->visual) return false;

    this->context = glXCreateContext(this->display, this->visual, NULL, True);
    if (!this->context) return false;

    this->colormap = XCreateColormap(this->display,
      RootWindow(this->display, this->visual->screen),
      this->visual->visual, AllocNone);
    XSetWindowAttributes windowattributes = {};
    windowattributes.colormap = this->colormap;
    this->window = XCreateWindow(this->display,
      RootWindow(this->display, this->visual->screen),
      0, 0, 16, 16, 0, this->visual->depth, InputOutput,
      this->visual->visual, CWColormap, &windowattributes);
    if (!this->window) return false;
    if (!glXMakeCurrent(this->display, this->window, this->context)) return false;

    this->glue = cc_glglue_instance(0x544558u);
    return this->glue != NULL;
  }

  ~GLXFixture()
  {
    if (this->glue) coin_glglue_destruct(0x544558u);
    if (this->display && this->context) {
      glXMakeCurrent(this->display, None, NULL);
      glXDestroyContext(this->display, this->context);
    }
    if (this->display && this->window) XDestroyWindow(this->display, this->window);
    if (this->display && this->colormap) XFreeColormap(this->display, this->colormap);
    if (this->visual) XFree(this->visual);
    if (this->display) XCloseDisplay(this->display);
  }

  Display * display;
  XVisualInfo * visual;
  GLXContext context;
  Colormap colormap;
  Window window;
  const cc_glglue * glue;
};

void
configure_environment(const char * value2, const char * value3)
{
  if (value2) setenv(ENV2, value2, 1); else unsetenv(ENV2);
  if (value3) setenv(ENV3, value3, 1); else unsetenv(ENV3);
}

SbBool
legal2(const cc_glglue * glue, int width, int height)
{
  return coin_glglue_is_texture_size_legal(glue, width, height, 0,
    GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, FALSE);
}

SbBool
legal3(const cc_glglue * glue, int width, int height, int depth)
{
  return coin_glglue_is_texture_size_legal(glue, width, height, depth,
    GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, FALSE);
}

SbBool
legal2_mipmap(const cc_glglue * glue, int width, int height)
{
  return coin_glglue_is_texture_size_legal(glue, width, height, 0,
    GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, TRUE);
}

SbBool
legal3_mipmap(const cc_glglue * glue, int width, int height, int depth)
{
  return coin_glglue_is_texture_size_legal(glue, width, height, depth,
    GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, TRUE);
}

void
clear_gl_errors()
{
  while (glGetError() != GL_NO_ERROR) { }
}

int
require_no_gl_error(int line)
{
  if (glGetError() != GL_NO_ERROR) return fail("glGetError() == GL_NO_ERROR", line);
  return 0;
}

#define REQUIRE_NO_GL_ERROR() do { const int result = require_no_gl_error(__LINE__); if (result) return result; } while (0)

int
test_validation_mipmaps(const cc_glglue * glue, GLint physical2)
{
  REQUIRE(physical2 >= 8);
  clear_gl_errors();

  REQUIRE(legal2(glue, 1, 1));
  REQUIRE_NO_GL_ERROR();

  if (cc_glglue_has_2d_proxy_textures(glue)) {
    REQUIRE(!legal2(glue, 0, 1));
    REQUIRE_NO_GL_ERROR();
    REQUIRE(!legal2(glue, 1, 0));
    REQUIRE_NO_GL_ERROR();
    REQUIRE(!legal2(glue, -1, 1));
    REQUIRE_NO_GL_ERROR();
    REQUIRE(!legal2(glue, 1, -1));
    REQUIRE_NO_GL_ERROR();

    REQUIRE(legal2_mipmap(glue, 8, 4));
    REQUIRE_NO_GL_ERROR();
    for (int level = 0, width = 8; level <= 3; ++level, width >>= 1) {
      GLint actual = 0;
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, level,
                               GL_TEXTURE_WIDTH, &actual);
      REQUIRE_NO_GL_ERROR();
      REQUIRE(actual == width);
    }
  }

  cc_glglue noproxy = *glue;
  noproxy.vendor_is_intel = TRUE;
  noproxy.max_texture_size = physical2;
  REQUIRE(!legal2(&noproxy, 0, 1));
  REQUIRE(!legal2(&noproxy, 1, 0));
  REQUIRE(!legal2(&noproxy, -1, 1));
  REQUIRE(!legal2(&noproxy, 1, -1));
  REQUIRE(legal2(&noproxy, 1, 1));
  REQUIRE_NO_GL_ERROR();

  if (cc_glglue_has_3d_textures(glue)) {
    GLint physical3 = 0;
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &physical3);
    REQUIRE_NO_GL_ERROR();
    if (physical3 < 8) return 0;

    REQUIRE(!legal3(glue, 0, 1, 1));
    REQUIRE_NO_GL_ERROR();
    REQUIRE(!legal3(glue, 1, 0, 1));
    REQUIRE_NO_GL_ERROR();
    REQUIRE(!legal3(glue, 1, 1, -1));
    REQUIRE_NO_GL_ERROR();

    REQUIRE(legal3_mipmap(glue, 8, 4, 2));
    REQUIRE_NO_GL_ERROR();
    for (int level = 0, width = 8; level <= 3; ++level, width >>= 1) {
      GLint actual = 0;
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, level,
                               GL_TEXTURE_WIDTH, &actual);
      REQUIRE_NO_GL_ERROR();
      REQUIRE(actual == width);
    }
  }

  return 0;
}

const char *
invalid_value(const char * scenario)
{
  if (std::strncmp(scenario, "empty-", 6) == 0) return "";
  if (std::strncmp(scenario, "zero-", 5) == 0) return "0";
  if (std::strncmp(scenario, "negative-", 9) == 0) return "-16";
  if (std::strncmp(scenario, "junk-", 5) == 0) return "text";
  if (std::strncmp(scenario, "suffix-", 7) == 0) return "16px";
  if (std::strncmp(scenario, "overflow-", 9) == 0) return "2147483648";
  return NULL;
}

int
test2(const char * scenario, const cc_glglue * glue, GLint physical)
{
  if (std::strcmp(scenario, "cap-2d") == 0) {
    REQUIRE(physical >= 17);
    REQUIRE(legal2(glue, 16, 16));
    REQUIRE(!legal2(glue, 17, 16));
    REQUIRE(!legal2(glue, 16, 17));
    REQUIRE(!legal2(glue, 0, 1));
    REQUIRE(!legal2(glue, 1, 0));
    REQUIRE(!legal2(glue, -1, 1));
    REQUIRE(!legal2(glue, 1, -1));
    return 0;
  }
  if (std::strcmp(scenario, "proxy-bypass-2d") == 0) {
    if (!cc_glglue_has_2d_proxy_textures(glue)) return 77;
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GLint width = 0;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    REQUIRE(glGetError() == GL_NO_ERROR);
    REQUIRE(width == 8);
    REQUIRE(legal2(glue, 16, 16));
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    REQUIRE(glGetError() == GL_NO_ERROR);
    REQUIRE(width == 8);
    return 0;
  }
  if (std::strcmp(scenario, "physical-2d") == 0) {
    REQUIRE(physical > 0 && physical < INT_MAX);
    REQUIRE(!legal2(glue, physical + 1, 1));
    REQUIRE(!legal2(glue, 1, physical + 1));
    return 0;
  }
  if (std::strcmp(scenario, "immutable-set-2d") == 0) {
    REQUIRE(legal2(glue, 16, 16));
    setenv(ENV2, "1024", 1);
    REQUIRE(!legal2(glue, 17, 16));
    return 0;
  }
  if (std::strcmp(scenario, "immutable-unset-2d") == 0) {
    REQUIRE(physical >= 16);
    REQUIRE(legal2(glue, 16, 16));
    setenv(ENV2, "1", 1);
    REQUIRE(legal2(glue, 16, 16));
    return 0;
  }

  REQUIRE(physical >= 17);
  REQUIRE(legal2(glue, 17, 17));
  return 0;
}

int
test3(const char * scenario, const cc_glglue * glue, GLint physical)
{
  if (std::strcmp(scenario, "unsupported-3d") == 0) {
    cc_glglue unsupported = *glue;
    unsupported.glTexImage3D = NULL;
    unsupported.glCopyTexSubImage3D = NULL;
    unsupported.glTexSubImage3D = NULL;
    REQUIRE(!legal3(&unsupported, 1, 1, 1));
    return 0;
  }

  if (!cc_glglue_has_3d_textures(glue) || physical < 17) return 77;
  if (std::strcmp(scenario, "cap-3d") == 0) {
    REQUIRE(legal3(glue, 16, 16, 16));
    REQUIRE(!legal3(glue, 17, 16, 16));
    REQUIRE(!legal3(glue, 16, 17, 16));
    REQUIRE(!legal3(glue, 16, 16, 17));
    REQUIRE(!legal3(glue, 0, 1, 1));
    REQUIRE(!legal3(glue, 1, 0, 1));
    REQUIRE(!legal3(glue, 1, 1, -1));
    return 0;
  }
  if (std::strcmp(scenario, "proxy-bypass-3d") == 0) {
    cc_glglue_glTexImage3D(glue, GL_PROXY_TEXTURE_3D, 0, GL_RGBA8,
                          8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GLint width = 0;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
    REQUIRE(glGetError() == GL_NO_ERROR);
    REQUIRE(width == 8);
    REQUIRE(legal3(glue, 16, 16, 16));
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
    REQUIRE(glGetError() == GL_NO_ERROR);
    REQUIRE(width == 8);
    return 0;
  }
  if (std::strcmp(scenario, "physical-3d") == 0) {
    REQUIRE(physical < INT_MAX);
    REQUIRE(!legal3(glue, physical + 1, 1, 1));
    REQUIRE(!legal3(glue, 1, physical + 1, 1));
    REQUIRE(!legal3(glue, 1, 1, physical + 1));
    return 0;
  }

  REQUIRE(legal3(glue, 17, 17, 17));
  return 0;
}

} // namespace

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 2;
  }

  const char * scenario = argv[1];
  const bool is3d = std::strstr(scenario, "-3d") != NULL;
  const char * invalid = invalid_value(scenario);

  if (invalid) configure_environment(is3d ? NULL : invalid, is3d ? invalid : NULL);
  else if (std::strncmp(scenario, "cap-", 4) == 0 ||
           std::strncmp(scenario, "proxy-bypass-", 13) == 0 ||
           std::strncmp(scenario, "immutable-set-", 14) == 0 ||
           std::strcmp(scenario, "unsupported-3d") == 0) {
    configure_environment(is3d ? NULL : "16", is3d ? "16" : NULL);
  }
  else if (std::strncmp(scenario, "physical-", 9) == 0) {
    configure_environment(is3d ? NULL : "2147483647",
                          is3d ? "2147483647" : NULL);
  }
  else configure_environment(NULL, NULL);

  SoDB::init();
  int result = 77;
  {
    GLXFixture fixture;
    if (!fixture.open()) {
      std::puts("SKIP: no usable GLX context");
    }
    else {
      GLint physical = 0;
      glGetIntegerv(is3d ? GL_MAX_3D_TEXTURE_SIZE : GL_MAX_TEXTURE_SIZE,
                    &physical);
      if (glGetError() != GL_NO_ERROR || physical <= 0) result = 77;
      else if (std::strcmp(scenario, "validation-mipmaps") == 0) {
        result = test_validation_mipmaps(fixture.glue, physical);
      }
      else result = is3d ? test3(scenario, fixture.glue, physical)
                         : test2(scenario, fixture.glue, physical);
    }
  }
  SoDB::finish();
  return result;
}
