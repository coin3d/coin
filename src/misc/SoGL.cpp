/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/lists/SbList.h>

#include <assert.h>
#include <stdio.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>

#if !GL_VERSION_1_1 && (GL_EXT_polygon_offset || GL_EXT_texture_object || GL_EXT_vertex_array)
// Function used to check if an extension is supported. (Wrapped in
// #if/#endif to avoid compiler warning.)
static int
extension_supported(char *extension)
{
  static const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;

  if (!extensions)
    extensions = glGetString(GL_EXTENSIONS);
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *)start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return 1;
      }
    }
    start = terminator;
  }
  return 0;
}
#endif // extension support check

static SbBool isInitialized = FALSE;

#if GL_VERSION_1_1
#elif GL_EXT_polygon_offset
static int polygonOffsetEXT;
#endif
#if GL_VERSION_1_1
#elif GL_EXT_texture_object
static int textureObjectEXT;
#endif
#if GL_VERSION_1_1
#elif GL_EXT_vertex_array
static int vertexArrayEXT;
#endif

void
sogl_global_init()
{
  if (isInitialized) return;
  isInitialized = TRUE;

#if GL_VERSION_1_1
#elif GL_EXT_polygon_offset
  polygonOffsetEXT = extension_supported("GL_EXT_polygon_offset");
#endif

#if GL_VERSION_1_1
#elif GL_EXT_texture_object
  textureObjectEXT = extension_supported("GL_EXT_texture_object");
#endif

#if GL_VERSION_1_1
#elif GL_EXT_vertex_array
  vertexArrayEXT = extension_supported("GL_EXT_vertex_array");
#endif

#if 0 // debug
  static int maxTextureSize;
  static int maxLights;
  static int maxClipPlanes;
  static int maxModelviewStackDepth;
  static int maxProjectionStackDepth;
  static int maxTextureStackDepth;

  GLint tmp;

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tmp);
  maxTextureSize = tmp;
  glGetIntegerv(GL_MAX_LIGHTS, &tmp);
  maxLights = tmp;
  glGetIntegerv(GL_MAX_CLIP_PLANES, &tmp);
  maxClipPlanes = tmp;
  glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &tmp);
  maxModelviewStackDepth = tmp;
  glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &tmp);
  maxProjectionStackDepth = tmp;
  glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &tmp);
  maxTextureStackDepth = tmp;

  SoDebugError::postInfo("sogl_global_init", "\n"
                         "max texure size: %d\n"
                         "max modelview matrix depth: %d\n"
                         "max lights: %d\n"
                         "max projection matrix depth: %d\n"
                         "max texture stack depth: %d\n"
                         "max clip planes: %d\n",
                         maxTextureSize,
                         maxModelviewStackDepth,
                         maxLights,
                         maxProjectionStackDepth,
                         maxTextureStackDepth,
                         maxClipPlanes);
#endif // debug

}


// generate a 3d circle in the x-z plane
static void
sogl_generate_3d_circle(SbVec3f *coords, const int num, const float radius, const float y)
{
  float delta = 2.0f*float(M_PI)/float(num);
  float angle = 0.0f;
  for (int i = 0; i < num; i++) {
    coords[i][0] = -float(sin(angle)) * radius;
    coords[i][1] = y;
    coords[i][2] = -float(cos(angle)) * radius;
    angle += delta;
  }
}

// generate a 2d circle
static void
sogl_generate_2d_circle(SbVec2f *coords, const int num, const float radius)
{
  float delta = 2.0f*float(M_PI)/float(num);
  float angle = 0.0f;
  for (int i = 0; i < num; i++) {
    coords[i][0] = -float(sin(angle)) * radius;
    coords[i][1] = -float(cos(angle)) * radius;
    angle += delta;
  }
}

void
sogl_render_cone(const float radius,
                 const float height,
                 const int numslices,
                 SoMaterialBundle * const material,
                 const unsigned int flags)
{
  int i;
  // use a limit of 128 to avoid allocating memory each time
  // a cone is drawn
  int slices = numslices;
  if (slices > 128) slices = 128;
  if (slices < 4) slices = 4;

  float h2 = height * 0.5f;

  // put coordinates on the stack
  SbVec3f coords[129];
  SbVec3f normals[130];
  SbVec2f texcoords[129];

  sogl_generate_3d_circle(coords, slices, radius, -h2);
  coords[slices] = coords[0];

  if (flags & SOGL_NEED_NORMALS) {
    double a = atan(height/radius);
    sogl_generate_3d_circle(normals, slices, float(sin(a)), float(cos(a)));
    normals[slices] = normals[0];
    normals[slices+1] = normals[1];
  }

  int matnr = 0;

  if (flags & SOGL_RENDER_SIDE) {
    glBegin(GL_TRIANGLES);
    i = 0;

    float t = 1.0;
    float delta = 1.0f / slices;

    while (i < slices) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t - delta*0.5f, 1.0f);
      }
      if (flags & SOGL_NEED_NORMALS) {
        SbVec3f n = (normals[i] + normals[i+1])*0.5f;
        glNormal3f(n[0], n[1], n[2]);
      }
      glVertex3f(0.0f, h2, 0.0f);
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t, 0.0f);
      }
      if (flags & SOGL_NEED_NORMALS) {
        glNormal3fv((const GLfloat*)&normals[i]);
      }
      glVertex3fv((const GLfloat*)&coords[i]);

      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t - delta, 0.0f);
      }
      if (flags & SOGL_NEED_NORMALS) {
        glNormal3fv((const GLfloat*)&normals[i+1]);
      }
      glVertex3fv((const GLfloat*)&coords[i+1]);

      i++;
      t -= delta;
    }

    matnr++;
    glEnd();
  }

  if (flags & SOGL_RENDER_BOTTOM) {
    if (flags & SOGL_NEED_TEXCOORDS) {
      sogl_generate_2d_circle(texcoords, slices, 0.5f);
      texcoords[slices] = texcoords[0];
    }

    if (flags & SOGL_MATERIAL_PER_PART) {
      material->send(matnr, TRUE);
    }

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    for (i = slices-1; i >= 0; i--) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(texcoords[i][0]+0.5f, texcoords[i][1]+0.5f);
      }
      glVertex3fv((const GLfloat*)&coords[i]);
    }
    glEnd();
  }
}

void
sogl_render_cylinder(const float radius,
                     const float height,
                     const int numslices,
                     SoMaterialBundle * const material,
                     const unsigned int flags)
{
  int i;
  int slices = numslices;
  if (slices > 128) slices = 128;
  if (slices < 4) slices = 4;

  float h2 = height * 0.5f;

  SbVec3f coords[129];
  SbVec3f normals[130];
  SbVec2f texcoords[129];

  sogl_generate_3d_circle(coords, slices, radius, -h2);
  coords[slices] = coords[0];

  if (flags & SOGL_NEED_NORMALS) {
    sogl_generate_3d_circle(normals, slices, 1.0f, 0.0f);
    normals[slices] = normals[0];
    normals[slices+1] = normals[1];
  }

  int matnr = 0;

  if (flags & SOGL_RENDER_SIDE) {
    glBegin(GL_QUAD_STRIP);
    i = 0;

    float t = 0.0;
    float inc = 1.0f / slices;

    while (i <= slices) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t, 1.0f);
      }
      if (flags & SOGL_NEED_NORMALS) {
        glNormal3fv((const GLfloat*)&normals[i]);
      }
      SbVec3f c = coords[i];
      glVertex3f(c[0], h2, c[2]);
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t, 0.0f);
      }
      glVertex3f(c[0], c[1], c[2]);
      i++;
      t += inc;
    }

    matnr++;
    glEnd();
  }

  if ((flags & SOGL_NEED_TEXCOORDS) &&
      (flags & (SOGL_RENDER_BOTTOM | SOGL_RENDER_TOP))) {
    sogl_generate_2d_circle(texcoords, slices, 0.5f);
    texcoords[slices] = texcoords[0];
  }

  if (flags & SOGL_RENDER_TOP) {
    if (flags & SOGL_MATERIAL_PER_PART) {
      material->send(matnr, TRUE);
    }
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (i = 0; i < slices; i++) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(texcoords[i][0]+0.5f, 1.0f - texcoords[i][1] - 0.5f);
      }
      const SbVec3f &c = coords[i];
      glVertex3f(c[0], h2, c[2]);
    }
    glEnd();
    matnr++;
  }
  if (flags & SOGL_RENDER_BOTTOM) {
    if (flags & SOGL_MATERIAL_PER_PART) {
      material->send(matnr, TRUE);
    }
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);

    for (i = slices-1; i >= 0; i--) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(texcoords[i][0]+0.5f, texcoords[i][1]+0.5f);
      }
      glVertex3fv((const GLfloat*)&coords[i]);
    }
    glEnd();
  }
}

void
sogl_render_sphere(const float radius,
                   const int numstacks,
                   const int numslices,
                   SoMaterialBundle * const /* material */,
                   const unsigned int /* flags */)
{
  int stacks = numstacks;
  int slices = numslices;

  if (stacks < 3) stacks = 3;
  if (slices < 4) slices = 4;

  if (slices > 128) slices = 128;

  // used to cache last stack's data
  SbVec3f coords[129];
  SbVec3f normals[129];
  float S[129];

  int i, j;
  float rho;
  float drho;
  float theta;
  float dtheta;
  float tc, ts;
  SbVec3f tmp;

  drho = float(M_PI) / (float) (stacks-1);
  dtheta = 2.0f * float(M_PI) / (float) slices;

  float currs = 0.0f;
  float incs = 1.0f / (float)slices;
  rho = drho;
  theta = 0.0f;
  tc = (float) cos(rho);
  ts = - (float) sin(rho);
  tmp.setValue(0.0f,
               tc,
               ts);
  normals[0] = tmp;
  tmp *= radius;
  coords[0] = tmp;
  S[0] = currs;
  float dT = 1.0f / (float) (stacks-1);
  float T = 1.0f - dT;

  glBegin(GL_TRIANGLES);

  for (j = 1; j <= slices; j++) {
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(currs + 0.5f * incs, 1.0f);
    glVertex3f(0.0f, radius, 0.0f);

    glNormal3fv((const GLfloat*) &normals[j-1]);
    glTexCoord2f(currs, T);
    glVertex3fv((const GLfloat*) &coords[j-1]);

    currs += incs;
    theta += dtheta;
    S[j] = currs;
    tmp.setValue(float(sin(theta))*ts,
                 tc,
                 float(cos(theta))*ts);

    normals[j] = tmp;
    tmp *= radius;
    coords[j] = tmp;

    glNormal3fv((const GLfloat*)&normals[j]);
    glTexCoord2f(currs, T);
    glVertex3fv((const GLfloat*)&coords[j]);
  }
  glEnd(); // GL_TRIANGLES

  rho += drho;

  for (i = 2; i < stacks-1; i++) {
    tc = (float)cos(rho);
    ts = - (float) sin(rho);
    glBegin(GL_QUAD_STRIP);
    theta = 0.0f;
    for (j = 0; j <= slices; j++) {
      glTexCoord2f(S[j], T);
      glNormal3fv((const GLfloat*)&normals[j]);
      glVertex3fv((const GLfloat*)&coords[j]);

      glTexCoord2f(S[j], T - dT);
      tmp.setValue(float(sin(theta))*ts,
                   tc,
                   float(cos(theta))*ts);
      normals[j] = tmp;
      glNormal3f(tmp[0], tmp[1], tmp[2]);
      tmp *= radius;
      glVertex3f(tmp[0], tmp[1], tmp[2]);
      coords[j] = tmp;
      theta += dtheta;
    }
    glEnd(); // GL_QUAD_STRIP
    rho += drho;
    T -= dT;
  }

  glBegin(GL_TRIANGLES);
  for (j = 0; j < slices; j++) {
    glTexCoord2f(S[j], T);
    glNormal3fv((const GLfloat*)&normals[j]);
    glVertex3fv((const GLfloat*)&coords[j]);

    glTexCoord2f(S[j]+incs*0.5f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -radius, 0.0f);

    glTexCoord2f(S[j+1], T);
    glNormal3fv((const GLfloat*)&normals[j+1]);
    glVertex3fv((const GLfloat*)&coords[j+1]);
  }
  glEnd(); // GL_TRIANGLES
}

//
// the 12 triangles in the cube
//
static int sogl_cube_vindices[] =
{
  0, 1, 3, 2,
  5, 4, 6, 7,
  1, 5, 7, 3,
  4, 0, 2, 6,
  4, 5, 1, 0,
  2, 3, 7, 6
};

static float sogl_cube_texcoords[] =
{
  1.0f, 1.0f,
  0.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f
};

static float sogl_cube_normals[] =
{
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, -1.0f,
  -1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, -1.0f, 0.0f
};

static void
sogl_generate_cube_vertices(SbVec3f *varray,
                       const float w,
                       const float h,
                       const float d)
{
  for (int i = 0; i < 8; i++) {
    varray[i].setValue((i&1) ? -w : w,
                       (i&2) ? -h : h,
                       (i&4) ? -d : d);
  }
}


void
sogl_render_cube(const float width,
                 const float height,
                 const float depth,
                 SoMaterialBundle * const material,
                 const unsigned int flags)
{
  SbVec3f varray[8];
  sogl_generate_cube_vertices(varray,
                         width * 0.5f,
                         height * 0.5f,
                         depth * 0.5f);
  glBegin(GL_QUADS);
  int *iptr = sogl_cube_vindices;

  for (int i = 0; i < 6; i++) { // 6 quads
    if (flags & SOGL_NEED_NORMALS)
      glNormal3fv((const GLfloat*)&sogl_cube_normals[i*3]);
    if (flags & SOGL_MATERIAL_PER_PART)
      material->send(i, TRUE);
    for (int j = 0; j < 4; j++) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2fv(&sogl_cube_texcoords[j<<1]);
      }
      glVertex3fv((const GLfloat*)&varray[*iptr++]);
    }
  }
  glEnd();
}


// limit functions

// FIXME: avoid most of these by using corresponding methods in
// SoGL*Element classes. 19990405 mortene.
int
sogl_max_texture_size()
{
  assert(isInitialized);

  // FIXME: move this code to a static method in
  // SoGLTextureImageElement? Not the case with OIV, but seems to be
  // better design. 19990405 mortene.
  static GLint value = -1;
  if (value == -1) glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
  return value;
}

int
sogl_max_lights()
{
  // TODO: implement.
  assert(isInitialized);
  assert(0);
  return 0;
}

int
sogl_max_clip_planes()
{
  // TODO: implement.
  assert(isInitialized);
  assert(0);
  return 0;
}

int
sogl_max_modelview_stack_depth()
{
  // TODO: implement.
  assert(isInitialized);
  assert(0);
  return 0;
}

int
sogl_max_projection_stack_depth()
{
  // TODO: implement.
  assert(isInitialized);
  assert(0);
  return 0;
}

int
sogl_max_texture_stack_depth()
{
  // TODO: implement.
  assert(isInitialized);
  assert(0);
  return 0;
}

SbBool
sogl_texture_object_ext()
{
#ifdef GL_VERSION_1_1
  return FALSE;
#elif GL_EXT_texture_object
  return (SbBool) textureObjectEXT;
#endif
}

SbBool
sogl_polygon_offset_ext()
{
#ifdef GL_VERSION_1_1
  return FALSE;
#elif GL_EXT_polygon_offset
  return (SbBool) polygonOffsetEXT;
#endif
}

SbBool
sogl_vertex_array_ext()
{
#ifdef GL_VERSION_1_1
  return FALSE;
#elif GL_EXT_vertex_array
  return (SbBool) vertexArrayEXT;
#endif
}

void
sogl_free_texture(unsigned int index)
{
#if COIN_DEBUG && 0
  SoDebugError::postInfo("sogl_free_texture", "Free texture: %d", index);
#endif
#if GL_VERSION_1_1
  glDeleteTextures(1, (const GLuint*)&index);
#elif GL_EXT_texture_object
  if (textureObjectEXT) glDeleteTexturesEXT(1, (const GLuint*)&index);
  else glDeleteLists(index, 1);
#else
  glDeleteLists(index, 1);
#endif
}

//
// local convenience function that creates the texture object/dl
//
static void
really_create_texture(const int wrapS, const int wrapT,
                      const unsigned char * const texture,
                      const int numComponents,
                      const int w, const int h,
                      const SbBool dlist,
                      const SbBool mipmap)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  wrapS ? GL_CLAMP : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  wrapT ? GL_CLAMP : GL_REPEAT);

  // set filtering to legal values for non mipmapped texture
  if (!dlist && !mipmap) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  GLenum glformat;
  switch (numComponents) {
  case 1:
    glformat = GL_LUMINANCE;
    break;
  case 2:
    glformat = GL_LUMINANCE_ALPHA;
    break;
  case 3:
    glformat = GL_RGB;
    break;
  case 4:
    glformat = GL_RGBA;
    break;
  default:
    assert(0 && "illegal numComonents");
    glformat = GL_RGB; // Unnecessary, but kills a compiler warning.
  }

  if (!mipmap) {
    glTexImage2D(GL_TEXTURE_2D, 0, numComponents, w, h,
                 0, glformat, GL_UNSIGNED_BYTE, texture);
  }
  else { // mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, numComponents, w, h,
                      glformat, GL_UNSIGNED_BYTE, texture);
  }
}


unsigned int
sogl_create_texture(const int wrapS, const int wrapT,
                    const unsigned char * const texture,
                    const int numComponents,
                    const int w, const int h,
                    const SbBool mipmap)
{
  GLuint index;
  SbBool dlist = FALSE;

#if GL_VERSION_1_1
  glGenTextures(1, &index);
#elif GL_EXT_texture_object
  if (textureObjectEXT) {
    glGenTexturesEXT(1, &index);
  }
  else {
    dlist = TRUE;
    index = glGenLists(1);
  }
#else
  index = glGenLists(1);
  dlist = TRUE;
#endif
  if (!index) {
    SoDebugError::postInfo("sogl_create_texture",
                           "sogl_create_texture: could not create display list/object\n");
    return 0;
  }
  if (dlist)
    glNewList(index, GL_COMPILE);
  else {
#if GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, index);
#elif GL_EXT_texture_object
    glBindTextureEXT(GL_TEXTURE_2D, index);
#endif
  }

  really_create_texture(wrapS, wrapT, texture,
                        numComponents, w, h, dlist, mipmap);

  if (dlist)
    glEndList();

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("sogl_create_texture",
                         "Creating texture: %d", index);
#endif // debug

  return index;
}

void
sogl_apply_texture(const unsigned int handle)
{
#if GL_VERSION_1_1
  glBindTexture(GL_TEXTURE_2D, handle);
#elif GL_EXT_texture_object
  if (textureObjectEXT) {
    glBindTextureEXT(GL_TEXTURE_2D, handle);
  }
  else glCallList(handle);
#else
  glCallList(handle);
#endif
}

void
sogl_polygon_offset(const int onoff, const float numpixels)
{
  if (onoff) {
#if GL_VERSION_1_1
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(numpixels, 1);
#elif GL_EXT_polygon_offset
    if (polygonOffsetEXT) {
      glPolygonOffsetEXT(numpixels, 0.000001);
      glEnable(GL_POLYGON_OFFSET_EXT);
    }
#endif
  }
  else {
#if GL_VERSION_1_1
    glPolygonOffset(0.0, 0.0);
    glDisable(GL_POLYGON_OFFSET_FILL);
#elif GL_EXT_polygon_offset
    if (polygonOffsetEXT) {
      glPolygonOffsetEXT(0.0, 0.0);
      glDisable(GL_POLYGON_OFFSET_EXT);
    }
#endif
  }
}


// **************************************************************************

#if !defined(NO_LINESET_RENDER)

#define OVERALL             0
#define PER_SEGMENT         1
#define PER_SEGMENT_INDEXED 2
#define PER_LINE            3
#define PER_LINE_INDEXED    4
#define PER_VERTEX          5
#define PER_VERTEX_INDEXED  6

void
sogl_render_lineset(const SoGLCoordinateElement * const coords,
                    const int32_t *indices,
                    int num_vertexindices,
                    const SbVec3f *normals,
                    const int32_t *normindices,
                    SoMaterialBundle *const materials,
                    const int32_t *matindices,
                    const SoTextureCoordinateBundle * const texcoords,
                    const int32_t *texindices,
                    int nbind,
                    int mbind,
                    const int texture,
                    const int drawAsPoints)
{
  // TODO: make several optimized rendering functions
  // I believe this is pretty fast though...

  if (mbind == PER_VERTEX_INDEXED && matindices == NULL) {
    matindices = indices;
  }
  if (nbind == PER_VERTEX_INDEXED && normindices == NULL) {
    normindices = indices;
  }
  if (mbind == PER_VERTEX || mbind == PER_LINE || mbind == PER_SEGMENT) {
    matindices = NULL;
  }
  if (nbind == PER_VERTEX || nbind == PER_LINE || nbind == PER_SEGMENT) {
    normindices = NULL;
  }

  if (nbind == OVERALL) normindices = NULL;
  if (mbind == OVERALL) matindices = NULL;

  int matnr = 0;
  int texidx = 0;
  int32_t i;
  const int32_t *end = indices + num_vertexindices;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;

  if (mbind == PER_SEGMENT || mbind == PER_SEGMENT_INDEXED ||
      nbind == PER_SEGMENT || nbind == PER_SEGMENT_INDEXED) {
    int previ;
    SbBool matPerPolyline = mbind == PER_LINE || mbind == PER_LINE_INDEXED;
    SbBool normPerPolyline = nbind == PER_LINE || nbind == PER_LINE_INDEXED;

    if (drawAsPoints) glBegin(GL_POINTS);
    else glBegin(GL_LINES);

    while (indices < end) {
      previ = *indices++;

      if (matPerPolyline) {
        if (matindices) materials->send(*matindices++, TRUE);
        else materials->send(matnr++, TRUE);
      }
      if (normPerPolyline) {
        if (normindices) currnormal = &normals[*normindices++];
        else currnormal = normals++;
      }

      i = *indices++;
      while (i >= 0) {
        if (!matPerPolyline && mbind != OVERALL) {
          if (matindices) materials->send(*matindices++, TRUE);
          else materials->send(matnr++, TRUE);
        }
        if (!normPerPolyline && nbind != OVERALL) {
          if (normindices) currnormal = &normals[*normindices++];
          else currnormal = normals++;
        }
        glNormal3fv((const GLfloat*)currnormal);
        if (texture) texcoords->send(texindices ? *texindices++ : texidx++,
                                     coords->get3(previ), *currnormal);
        coords->send(previ);

        if (mbind >= PER_VERTEX) {
          if (matindices) materials->send(*matindices++, TRUE);
          else materials->send(matnr++, TRUE);
        }
        if (nbind >= PER_VERTEX) {
          if (normindices) currnormal = &normals[*normindices++];
          else currnormal = normals++;
        }
        glNormal3fv((const GLfloat*)currnormal);
        if (texture) texcoords->send(texindices ? *texindices++ : texidx++,
                                     coords->get3(i), *currnormal);
        coords->send(i);

        previ = i;
        i = *indices++;
      }
      if (mbind == PER_VERTEX_INDEXED) matindices++;
      if (nbind == PER_VERTEX_INDEXED) normindices++;
      if (texture && texindices) texindices++;
    }
    glEnd();
    return;
  }

  if (drawAsPoints) glBegin(GL_POINTS);

  while (indices < end) {
    if (!drawAsPoints) glBegin(GL_LINE_STRIP);
    i = *indices++;
    assert(i >= 0);
    if (matindices) materials->send(*matindices++, TRUE);
    else if (mbind != OVERALL) materials->send(matnr++, TRUE);
    if (normindices) currnormal = &normals[*normindices++];
    else if (nbind != OVERALL) currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
    if (texture) texcoords->send(texindices ? *texindices++ : texidx++,
                                 coords->get3(i), *currnormal);
    coords->send(i);

    i = *indices++;
    assert(i >= 0);
    if (mbind >= PER_VERTEX) {
      if (matindices) materials->send(*matindices++, TRUE);
      else materials->send(matnr++, TRUE);
    }
    if (nbind >= PER_VERTEX) {
      if (normindices) currnormal = &normals[*normindices++];
      else currnormal = normals++;
    }
    glNormal3fv((const GLfloat*)currnormal);
    if (texture) texcoords->send(texindices ? *texindices++ : texidx++,
                                 coords->get3(i), *currnormal);
    coords->send(i);

    i = *indices++;
    while (i >= 0) {
      assert(indices <= end);
      if (mbind >= PER_VERTEX) {
        if (matindices) materials->send(*matindices++, TRUE);
        else materials->send(matnr++, TRUE);
      }
      if (nbind >= PER_VERTEX) {
        if (normindices) currnormal = &normals[*normindices++];
        else currnormal = normals++;
      }
      glNormal3fv((const GLfloat*)currnormal);
      if (texture) texcoords->send(texindices ? *texindices++ : texidx++,
                                   coords->get3(i), *currnormal);
      coords->send(i);
      i = *indices++;
    }
    if (!drawAsPoints) glEnd(); // end of line strip
    if (mbind == PER_VERTEX_INDEXED) matindices++;
    if (nbind == PER_VERTEX_INDEXED) normindices++;
    if (texture && texindices) texindices++;
  }
  if (drawAsPoints) glEnd();
}


#undef OVERALL
#undef PER_SEGMENT
#undef PER_SEGMENT_INDEXED
#undef PER_LINE
#undef PER_LINE_INDEXED
#undef PER_VERTEX
#undef PER_VERTEX_INDEXED

#endif // !NO_LINESET_RENDER


static SbList <float> * tmpcoordlist = NULL;
static SbList <float> * tmptexcoordlist = NULL;

static void nurbs_coord_cleanup(void)
{
  delete tmpcoordlist;
}

static void nurbs_texcoord_cleanup(void)
{
  delete tmptexcoordlist;
}

void
sogl_render_nurbs_surface(SoAction * action, SoShape * shape,
                          void * nurbsrenderer,
                          const int numuctrlpts, const int numvctrlpts,
                          const float * uknotvec, const float * vknotvec,
                          const int numuknot, const int numvknot,
                          const int numsctrlpts, const int numtctrlpts,
                          const float * sknotvec, const float * tknotvec,
                          const int numsknot, const int numtknot,
                          const SbBool glrender,
                          const int numcoordindex, const int32_t * coordindex,
                          const int numtexcoordindex, const int32_t * texcoordindex)
{
#if !GLU_VERSION_1_3
  if (!glrender) {
#if COIN_DEBUG
    static int first = 1;
    if (first) {
      SoDebugError::postInfo("sogl_render_nurbs_surface",
                             "NURBS tessellator requires GLU 1.3.");
      first = 0;
    }
#endif // COIN_DEBUG
    return;
  }
#endif // !GLU_VERSION_1_3

#ifdef HAVE_GLU_NURBSOBJECT
  HAVE_GLU_NURBSOBJECT * nurbsobj = (HAVE_GLU_NURBSOBJECT *) nurbsrenderer;
  SoState * state = action->getState();

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);

  float maxctrl = SbMax(numuctrlpts, numvctrlpts);

  switch (SoComplexityTypeElement::get(state)) {
  case SoComplexityTypeElement::SCREEN_SPACE:
    {
      SbBox3f box;
      SbVec3f center;
      shape->computeBBox(action, box, center);
      SbVec2s size;
      SoShape::getScreenSize(state, box, size);
      float maxpix = (float) SbMax(size[0], size[1]);
      if (maxpix < 1.0f) maxpix = 1.0f;
      float complexity = SoComplexityElement::get(state);
      complexity = SbMin(100.0f, (float)sqrt(complexity * maxpix) * maxctrl);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_U_STEP, complexity);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_V_STEP, complexity);
      break;
    }
  case SoComplexityTypeElement::OBJECT_SPACE:
    {
      float complexity = SoComplexityElement::get(state);
      // add and cube value to give higher complexity values more boost
      complexity += 1.0f;
      complexity = (float) pow(complexity, 3.0);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_U_STEP, complexity * maxctrl);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_V_STEP, complexity * maxctrl);
      break;
    }
  case SoComplexityTypeElement::BOUNDING_BOX:
    assert(0 && "should never get here");
    break;
  default:
    assert(0 && "unknown complexity type");
    break;
  }

#if GLU_VERSION_1_3
  gluNurbsProperty(nurbsobj, (GLenum) GLU_NURBS_MODE,
                   glrender ? GLU_NURBS_RENDERER : GLU_NURBS_TESSELLATOR);
#endif // GLU_VERSION_1_3
  // need to load sampling matrices?
  gluNurbsProperty(nurbsobj, (GLenum) GLU_AUTO_LOAD_MATRIX, glrender);

  if (!glrender) { // supply the sampling matrices
    SbMatrix glmodelmatrix = SoViewingMatrixElement::get(state);
    glmodelmatrix.multLeft(SoModelMatrixElement::get(state));
    SbVec2s origin = SoViewportRegionElement::get(state).getViewportOriginPixels();
    SbVec2s size = SoViewportRegionElement::get(state).getViewportSizePixels();
    GLint viewport[4];
    viewport[0] = origin[0];
    viewport[1] = origin[1];
    viewport[2] = size[0];
    viewport[3] = size[1];
    gluLoadSamplingMatrices(nurbsobj,
                            (float*)glmodelmatrix,
                            SoProjectionMatrixElement::get(state)[0],
                            viewport);
  }

  int dim = coords->is3D() ? 3 : 4;

  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  if (!coords->getNum()) return;

  GLfloat * ptr = coords->is3D() ?
    (GLfloat *)coordelem->getArrayPtr3() :
    (GLfloat *)coordelem->getArrayPtr4();

  // just copy indexed control points into a linear array
  if (numcoordindex && coordindex) {
    if (tmpcoordlist == NULL) {
      tmpcoordlist = new SbList <float>;
      atexit(nurbs_coord_cleanup);
    }
    tmpcoordlist->truncate(0);
    for (int i = 0; i < numcoordindex; i++) {
      for (int j = 0; j < dim; j++) {
        tmpcoordlist->append(ptr[coordindex[i]*dim+j]);
      }
    }
    ptr = (float*) tmpcoordlist->getArrayPtr();
  }

  gluBeginSurface(nurbsobj);
  gluNurbsSurface(nurbsobj,
                  numuknot, (GLfloat*) uknotvec,
                  numvknot, (GLfloat*) vknotvec,
                  dim, dim * numuctrlpts,
                  ptr, numuknot - numuctrlpts, numvknot - numvctrlpts,
                  (dim == 3) ? GL_MAP2_VERTEX_3 : GL_MAP2_VERTEX_4);

  if (!glrender || SoGLTextureEnabledElement::get(state)) {
    const SoTextureCoordinateElement * tc =
      SoTextureCoordinateElement::getInstance(state);
    if (numsctrlpts && numtctrlpts && numsknot && numtknot &&
        (tc->getType() == SoTextureCoordinateElement::EXPLICIT) && tc->getNum()) {
      int texdim = tc->is2D() ? 2 : 4;
      GLfloat * texptr = tc->is2D() ?
        (GLfloat*) tc->getArrayPtr2() :
        (GLfloat*) tc->getArrayPtr4();

      // copy indexed texcoords into temporary array
      if (numtexcoordindex && texcoordindex) {
        if (tmptexcoordlist == NULL) {
          tmptexcoordlist = new SbList <float>;
          atexit(nurbs_texcoord_cleanup);
        }
        tmptexcoordlist->truncate(0);
        for (int i = 0; i < numtexcoordindex; i++) {
          for (int j = 0; j < texdim; j++) {
            tmptexcoordlist->append(texptr[texcoordindex[i]*texdim+j]);
          }
        }
        texptr = (float*) tmptexcoordlist->getArrayPtr();
      }

      gluNurbsSurface(nurbsobj,
                      numsknot, (GLfloat*) sknotvec,
                      numtknot, (GLfloat*) tknotvec,
                      texdim, texdim * numsctrlpts,
                      texptr, numsknot - numsctrlpts, numtknot - numtctrlpts,
                      (texdim == 2) ? GL_MAP2_TEXTURE_COORD_2 : GL_MAP2_TEXTURE_COORD_4);

    }
    else if ((tc->getType() == SoTextureCoordinateElement::DEFAULT) ||
             (tc->getType() == SoTextureCoordinateElement::EXPLICIT)) {
      static float defaulttex[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
      };
      static GLfloat defaulttexknot[] = {0.0f, 0.0f, 1.0f, 1.0f};
      gluNurbsSurface(nurbsobj, 4, defaulttexknot, 4, defaulttexknot,
                      2, 2*2, defaulttex, 4-2, 4-2,
                      GL_MAP2_TEXTURE_COORD_2);
    }
  }
  const SoNodeList & profilelist = SoProfileElement::get(state);
  int i, n = profilelist.getLength();
  SbBool istrimming = FALSE;

  if (n) {
    for (i = 0; i < n; i++) {
      float * points;
      int numpoints;
      int floatspervec;
      int numknots;
      float * knotvector;

      SoProfile * profile = (SoProfile*) profilelist[i];

      if (istrimming && (profile->linkage.getValue() != SoProfileElement::ADD_TO_CURRENT)) {
        istrimming = FALSE;
        gluEndTrim(nurbsobj);
      }
      if (!istrimming) {
        gluBeginTrim(nurbsobj);
        istrimming = TRUE;
      }
      profile->getTrimCurve(state, numpoints,
                            points, floatspervec,
                            numknots, knotvector);

      if (numknots) {
        gluNurbsCurve(nurbsobj, numknots, knotvector, floatspervec,
                      points, numknots-numpoints, floatspervec == 2 ?
                      (GLenum) GLU_MAP1_TRIM_2 : (GLenum) GLU_MAP1_TRIM_3);

      }

      else {
        gluPwlCurve(nurbsobj, numpoints, points, floatspervec,
                    floatspervec == 2 ?
                    (GLenum) GLU_MAP1_TRIM_2 : (GLenum) GLU_MAP1_TRIM_3 );
      }
    }
    if (istrimming) gluEndTrim(nurbsobj);
  }
  gluEndSurface(nurbsobj);

#else // !HAVE_GLU_NURBSOBJECT
  // Should never get here if the NURBS functionality is missing.
  assert(FALSE && "Function sogl_render_nurbs_surface() is de-funct.");
#endif // !HAVE_GLU_NURBSOBJECT
}

void
sogl_render_nurbs_curve(SoAction * action, SoShape * shape,
                        void * nurbsrenderer,
                        const int numctrlpts,
                        const float * knotvec,
                        const int numknots,
                        const SbBool glrender,
                        const SbBool drawaspoints,
                        const int numcoordindex, const int32_t * coordindex)
{
#if !GLU_VERSION_1_3
  if (!glrender) {
#if COIN_DEBUG
    static int first = 1;
    if (first) {
      SoDebugError::postInfo("sogl_render_nurbs_surface",
                             "NURBS tessellator requires GLU 1.3.");
      first = 0;
    }
#endif // COIN_DEBUG
    return;
  }
#endif // !GLU_VERSION_1_3

#ifdef HAVE_GLU_NURBSOBJECT

  HAVE_GLU_NURBSOBJECT * nurbsobj = (HAVE_GLU_NURBSOBJECT *) nurbsrenderer;
  SoState * state = action->getState();

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);

  switch (SoComplexityTypeElement::get(state)) {
  case SoComplexityTypeElement::SCREEN_SPACE:
    {
      SbBox3f box;
      SbVec3f center;
      shape->computeBBox(action, box, center);
      SbVec2s size;
      SoShape::getScreenSize(state, box, size);
      float maxpix = (float) SbMax(size[0], size[1]);
      if (maxpix < 1.0f) maxpix = 1.0f;
      float complexity = SoComplexityElement::get(state);
      complexity = SbMin(100.0f, (float)sqrt(complexity * maxpix) * numctrlpts);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_U_STEP, complexity);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_V_STEP, complexity);
      break;
    }
  case SoComplexityTypeElement::OBJECT_SPACE:
    {
      float complexity = SoComplexityElement::get(state);
      // add and cube value to give higher complexity values more boost
      complexity += 1.0f;
      complexity = (float) pow(complexity, 3.0);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_U_STEP, complexity * numctrlpts);
      gluNurbsProperty(nurbsobj, (GLenum) GLU_V_STEP, complexity * numctrlpts);
      break;
    }
  case SoComplexityTypeElement::BOUNDING_BOX:
    assert(0 && "should never get here");
    break;
  default:
    assert(0 && "unknown complexity type");
    break;
  }

  gluNurbsProperty(nurbsobj, (GLenum) GLU_DISPLAY_MODE, drawaspoints ? GLU_POINT : GLU_LINE);
#if GLU_VERSION_1_3
  gluNurbsProperty(nurbsobj, (GLenum) GLU_NURBS_MODE,
                   glrender ? GLU_NURBS_RENDERER : GLU_NURBS_TESSELLATOR);
#endif // GLU_VERSION_1_3
  // need to load sampling matrices?
  gluNurbsProperty(nurbsobj, (GLenum) GLU_AUTO_LOAD_MATRIX, glrender);

  if (!glrender) { // supply the sampling matrices
    SbMatrix glmodelmatrix = SoViewingMatrixElement::get(state);
    glmodelmatrix.multLeft(SoModelMatrixElement::get(state));
    SbVec2s origin = SoViewportRegionElement::get(state).getViewportOriginPixels();
    SbVec2s size = SoViewportRegionElement::get(state).getViewportSizePixels();
    GLint viewport[4];
    viewport[0] = origin[0];
    viewport[1] = origin[1];
    viewport[2] = size[0];
    viewport[3] = size[1];
    gluLoadSamplingMatrices(nurbsobj,
                            (float*)glmodelmatrix,
                            SoProjectionMatrixElement::get(state)[0],
                            viewport);
  }

  int dim = coords->is3D() ? 3 : 4;

  GLfloat * ptr = coords->is3D() ?
    (GLfloat *)coords->getArrayPtr3() :
    (GLfloat *)coords->getArrayPtr4();

  // just copy indexed control points into a linear array
  if (numcoordindex && coordindex) {
    if (tmpcoordlist == NULL) {
      tmpcoordlist = new SbList <float>;
      atexit(nurbs_coord_cleanup);
    }
    tmpcoordlist->truncate(0);
    for (int i = 0; i < numcoordindex; i++) {
      for (int j = 0; j < dim; j++) {
        tmpcoordlist->append(ptr[coordindex[i]*dim+j]);
      }
    }
    ptr = (float*) tmpcoordlist->getArrayPtr();
  }

  gluBeginCurve(nurbsobj);
  gluNurbsCurve(nurbsobj,
                numknots,
                (float*)knotvec,
                dim,
                ptr,
                numknots - numctrlpts,
                (GLenum)(dim == 3 ? GL_MAP1_VERTEX_3 : GL_MAP1_VERTEX_4));

  gluEndCurve(nurbsobj);

#else // !HAVE_GLU_NURBSOBJECT
  // Should never get here if the NURBS functionality is missing.
  assert(FALSE && "Function sogl_render_nurbs_curve() is de-funct.");
#endif // !HAVE_GLU_NURBSOBJECT

}



// **************************************************************************

#if !defined(NO_FACESET_RENDER)


typedef void sogl_render_faceset_func(const SoGLCoordinateElement * const coords,
                                      const int32_t *vertexindices,
                                      int num_vertexindices,
                                      const SbVec3f *normals,
                                      const int32_t *normindices,
                                      SoMaterialBundle *materials,
                                      const int32_t *matindices,
                                      const SoTextureCoordinateBundle * const texcoords,
                                      const int32_t *texindices);

static sogl_render_faceset_func *render_funcs[74];

//
// optimized faceset rendering functions.
// the functions are generated by including a template file once for
// each function, with different defines.
//

#define OVERALL             0
#define PER_FACE            1
#define PER_FACE_INDEXED    2
#define PER_VERTEX          3
#define PER_VERTEX_INDEXED  4

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_fs_n0_m0
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_fs_n0_m1
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_fs_n0_m2
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_fs_n0_m3
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_fs_n0_m4
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES





#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_fs_n1_m0
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_fs_n1_m1
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_fs_n1_m2
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_fs_n1_m3
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_fs_n1_m4
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES




#define MBINDING OVERALL
#define NBINDING PER_FACE_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n2_m0
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n2_m1
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_FACE_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n2_m2
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n2_m3
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_FACE_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n2_m4
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES





#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_fs_n3_m0
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_fs_n3_m1
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_fs_n3_m2
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_fs_n3_m3
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_fs_n3_m4
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES






#define MBINDING OVERALL
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n4_m0
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n4_m1
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n4_m2
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n4_m3
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_fs_n4_m4
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES



/* Textures ************************************************/


#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_fs_n0_m0_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_fs_n0_m1_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_fs_n0_m2_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_fs_n0_m3_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_fs_n0_m4_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES





#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_fs_n1_m0_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_fs_n1_m1_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_fs_n1_m2_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_fs_n1_m3_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_fs_n1_m4_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES




#define MBINDING OVERALL
#define NBINDING PER_FACE_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n2_m0_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n2_m1_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_FACE_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n2_m2_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n2_m3_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_FACE_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n2_m4_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES





#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_fs_n3_m0_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_fs_n3_m1_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_fs_n3_m2_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_fs_n3_m3_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_fs_n3_m4_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES






#define MBINDING OVERALL
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n4_m0_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n4_m1_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n4_m2_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n4_m3_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES TRUE
static void sogl_fs_n4_m4_tex
#include "SoGLFaceSetTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

void
sogl_render_faceset(const SoGLCoordinateElement * const vertexlist,
                    const int32_t *vertexindices,
                    int num_vertexindices,
                    const SbVec3f *normals,
                    const int32_t *normindices,
                    SoMaterialBundle *const materials,
                    const int32_t *matindices,
                    SoTextureCoordinateBundle * const texcoords,
                    const int32_t *texindices,
                    const int nbind,
                    const int mbind,
                    const int texture)
{
  static int first = 1;
  if (first) {
    first = 0;
    render_funcs[0] = sogl_fs_n0_m0;
    render_funcs[1] = sogl_fs_n0_m0_tex;
    render_funcs[2] = sogl_fs_n0_m1;
    render_funcs[3] = sogl_fs_n0_m1_tex;
    render_funcs[4] = sogl_fs_n0_m2;
    render_funcs[5] = sogl_fs_n0_m2_tex;
    render_funcs[6] = sogl_fs_n0_m3;
    render_funcs[7] = sogl_fs_n0_m3_tex;
    render_funcs[8] = sogl_fs_n0_m4;
    render_funcs[9] = sogl_fs_n0_m4_tex;

    render_funcs[16] = sogl_fs_n1_m0;
    render_funcs[17] = sogl_fs_n1_m0_tex;
    render_funcs[18] = sogl_fs_n1_m1;
    render_funcs[19] = sogl_fs_n1_m1_tex;
    render_funcs[20] = sogl_fs_n1_m2;
    render_funcs[21] = sogl_fs_n1_m2_tex;
    render_funcs[22] = sogl_fs_n1_m3;
    render_funcs[23] = sogl_fs_n1_m3_tex;
    render_funcs[24] = sogl_fs_n1_m4;
    render_funcs[25] = sogl_fs_n1_m4_tex;

    render_funcs[32] = sogl_fs_n2_m0;
    render_funcs[33] = sogl_fs_n2_m0_tex;
    render_funcs[34] = sogl_fs_n2_m1;
    render_funcs[35] = sogl_fs_n2_m1_tex;
    render_funcs[36] = sogl_fs_n2_m2;
    render_funcs[37] = sogl_fs_n2_m2_tex;
    render_funcs[38] = sogl_fs_n2_m3;
    render_funcs[39] = sogl_fs_n2_m3_tex;
    render_funcs[40] = sogl_fs_n2_m4;
    render_funcs[41] = sogl_fs_n2_m4_tex;

    render_funcs[48] = sogl_fs_n3_m0;
    render_funcs[49] = sogl_fs_n3_m0_tex;
    render_funcs[50] = sogl_fs_n3_m1;
    render_funcs[51] = sogl_fs_n3_m1_tex;
    render_funcs[52] = sogl_fs_n3_m2;
    render_funcs[53] = sogl_fs_n3_m2_tex;
    render_funcs[54] = sogl_fs_n3_m3;
    render_funcs[55] = sogl_fs_n3_m3_tex;
    render_funcs[56] = sogl_fs_n3_m4;
    render_funcs[57] = sogl_fs_n3_m4_tex;

    render_funcs[64] = sogl_fs_n4_m0;
    render_funcs[65] = sogl_fs_n4_m0_tex;
    render_funcs[66] = sogl_fs_n4_m1;
    render_funcs[67] = sogl_fs_n4_m1_tex;
    render_funcs[68] = sogl_fs_n4_m2;
    render_funcs[69] = sogl_fs_n4_m2_tex;
    render_funcs[70] = sogl_fs_n4_m3;
    render_funcs[71] = sogl_fs_n4_m3_tex;
    render_funcs[72] = sogl_fs_n4_m4;
    render_funcs[73] = sogl_fs_n4_m4_tex;
  }

  int idx = (nbind << 4) | (mbind << 1) | texture;

  // just in case someone forgot
  if (matindices == NULL) matindices = vertexindices;
  if (normindices == NULL) normindices = vertexindices;

  render_funcs[idx](vertexlist,
                    vertexindices,
                    num_vertexindices,
                    normals,
                    normindices,
                    materials,
                    matindices,
                    texcoords,
                    texindices);
}

#undef OVERALL
#undef PER_FACE
#undef PER_FACE_INDEXED
#undef PER_VERTEX
#undef PER_VERTEX_INDEXED

#endif // !NO_FACESET_RENDER


// **************************************************************************

#if !defined(NO_TRISTRIPSET_RENDER)


#define OVERALL              0
#define PER_STRIP            1
#define PER_STRIP_INDEXED    2
#define PER_TRIANGLE         3
#define PER_TRIANGLE_INDEXED 4
#define PER_VERTEX           5
#define PER_VERTEX_INDEXED   6

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_ts_n0_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES





#define MBINDING OVERALL
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_STRIP
#define TEXTURES FALSE
static void sogl_ts_n1_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES







#define MBINDING OVERALL
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_STRIP_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n2_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES








#define MBINDING OVERALL
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_TRIANGLE
#define TEXTURES FALSE
static void sogl_ts_n3_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES






#define MBINDING OVERALL
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n4_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES







#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_ts_n5_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES








#define MBINDING OVERALL
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m0
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m1
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m2
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m3
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m4
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m5
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX_INDEXED
#define TEXTURES FALSE
static void sogl_ts_n6_m6
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES


/* textures */
#undef TEXTURES
#define TEXTURES TRUE

#define MBINDING OVERALL
#define NBINDING OVERALL
static void sogl_ts_n0_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING OVERALL
static void sogl_ts_n0_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING OVERALL
static void sogl_ts_n0_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING OVERALL
static void sogl_ts_n0_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING OVERALL
static void sogl_ts_n0_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
static void sogl_ts_n0_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING OVERALL
static void sogl_ts_n0_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING





#define MBINDING OVERALL
#define NBINDING PER_STRIP
static void sogl_ts_n1_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP
static void sogl_ts_n1_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_STRIP
static void sogl_ts_n1_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_STRIP
static void sogl_ts_n1_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_STRIP
static void sogl_ts_n1_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP
static void sogl_ts_n1_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_STRIP
static void sogl_ts_n1_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING





#define MBINDING OVERALL
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_STRIP_INDEXED
static void sogl_ts_n2_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING





#define MBINDING OVERALL
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_TRIANGLE
static void sogl_ts_n3_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING




#define MBINDING OVERALL
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_TRIANGLE_INDEXED
static void sogl_ts_n4_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING


#define MBINDING OVERALL
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX
static void sogl_ts_n5_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING






#define MBINDING OVERALL
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m0_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m1_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_STRIP_INDEXED
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m2_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m3_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_TRIANGLE_INDEXED
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m4_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m5_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING

#define MBINDING PER_VERTEX_INDEXED
#define NBINDING PER_VERTEX_INDEXED
static void sogl_ts_n6_m6_tex
#include "SoGLTristripTemplate.cpp"
#undef MBINDING
#undef NBINDING


#undef TEXTURES


static sogl_render_faceset_func *render_ts_funcs[110];


void
sogl_render_tristrip(const SoGLCoordinateElement * const vertexlist,
                     const int32_t *vertexindices,
                     int num_vertexindices,
                     const SbVec3f *normals,
                     const int32_t *normindices,
                     SoMaterialBundle *const materials,
                     const int32_t *matindices,
                     const SoTextureCoordinateBundle * const texcoords,
                     const int32_t *texindices,
                     const int nbind,
                     const int mbind,
                     const int texture)
{
  static int first = 1;
  if (first) {
    first = 0;
    render_ts_funcs[0] = sogl_ts_n0_m0;
    render_ts_funcs[1] = sogl_ts_n0_m0_tex;
    render_ts_funcs[2] = sogl_ts_n0_m1;
    render_ts_funcs[3] = sogl_ts_n0_m1_tex;
    render_ts_funcs[4] = sogl_ts_n0_m2;
    render_ts_funcs[5] = sogl_ts_n0_m2_tex;
    render_ts_funcs[6] = sogl_ts_n0_m3;
    render_ts_funcs[7] = sogl_ts_n0_m3_tex;
    render_ts_funcs[8] = sogl_ts_n0_m4;
    render_ts_funcs[9] = sogl_ts_n0_m4_tex;
    render_ts_funcs[10] = sogl_ts_n0_m5;
    render_ts_funcs[11] = sogl_ts_n0_m5_tex;
    render_ts_funcs[12] = sogl_ts_n0_m6;
    render_ts_funcs[13] = sogl_ts_n0_m6_tex;

    render_ts_funcs[16] = sogl_ts_n1_m0;
    render_ts_funcs[17] = sogl_ts_n1_m0_tex;
    render_ts_funcs[18] = sogl_ts_n1_m1;
    render_ts_funcs[19] = sogl_ts_n1_m1_tex;
    render_ts_funcs[20] = sogl_ts_n1_m2;
    render_ts_funcs[21] = sogl_ts_n1_m2_tex;
    render_ts_funcs[22] = sogl_ts_n1_m3;
    render_ts_funcs[23] = sogl_ts_n1_m3_tex;
    render_ts_funcs[24] = sogl_ts_n1_m4;
    render_ts_funcs[25] = sogl_ts_n1_m4_tex;
    render_ts_funcs[26] = sogl_ts_n1_m5;
    render_ts_funcs[27] = sogl_ts_n1_m5_tex;
    render_ts_funcs[28] = sogl_ts_n1_m6;
    render_ts_funcs[29] = sogl_ts_n1_m6_tex;

    render_ts_funcs[32] = sogl_ts_n2_m0;
    render_ts_funcs[33] = sogl_ts_n2_m0_tex;
    render_ts_funcs[34] = sogl_ts_n2_m1;
    render_ts_funcs[35] = sogl_ts_n2_m1_tex;
    render_ts_funcs[36] = sogl_ts_n2_m2;
    render_ts_funcs[37] = sogl_ts_n2_m2_tex;
    render_ts_funcs[38] = sogl_ts_n2_m3;
    render_ts_funcs[39] = sogl_ts_n2_m3_tex;
    render_ts_funcs[40] = sogl_ts_n2_m4;
    render_ts_funcs[41] = sogl_ts_n2_m4_tex;
    render_ts_funcs[42] = sogl_ts_n2_m5;
    render_ts_funcs[43] = sogl_ts_n2_m5_tex;
    render_ts_funcs[44] = sogl_ts_n2_m6;
    render_ts_funcs[45] = sogl_ts_n2_m6_tex;

    render_ts_funcs[48] = sogl_ts_n3_m0;
    render_ts_funcs[49] = sogl_ts_n3_m0_tex;
    render_ts_funcs[50] = sogl_ts_n3_m1;
    render_ts_funcs[51] = sogl_ts_n3_m1_tex;
    render_ts_funcs[52] = sogl_ts_n3_m2;
    render_ts_funcs[53] = sogl_ts_n3_m2_tex;
    render_ts_funcs[54] = sogl_ts_n3_m3;
    render_ts_funcs[55] = sogl_ts_n3_m3_tex;
    render_ts_funcs[56] = sogl_ts_n3_m4;
    render_ts_funcs[57] = sogl_ts_n3_m4_tex;
    render_ts_funcs[57] = sogl_ts_n3_m5;
    render_ts_funcs[58] = sogl_ts_n3_m5_tex;
    render_ts_funcs[59] = sogl_ts_n3_m6;
    render_ts_funcs[60] = sogl_ts_n3_m6_tex;

    render_ts_funcs[64] = sogl_ts_n4_m0;
    render_ts_funcs[65] = sogl_ts_n4_m0_tex;
    render_ts_funcs[66] = sogl_ts_n4_m1;
    render_ts_funcs[67] = sogl_ts_n4_m1_tex;
    render_ts_funcs[68] = sogl_ts_n4_m2;
    render_ts_funcs[69] = sogl_ts_n4_m2_tex;
    render_ts_funcs[70] = sogl_ts_n4_m3;
    render_ts_funcs[71] = sogl_ts_n4_m3_tex;
    render_ts_funcs[72] = sogl_ts_n4_m4;
    render_ts_funcs[73] = sogl_ts_n4_m4_tex;
    render_ts_funcs[74] = sogl_ts_n4_m5;
    render_ts_funcs[75] = sogl_ts_n4_m5_tex;
    render_ts_funcs[76] = sogl_ts_n4_m6;
    render_ts_funcs[77] = sogl_ts_n4_m6_tex;

    render_ts_funcs[80] = sogl_ts_n5_m0;
    render_ts_funcs[81] = sogl_ts_n5_m0_tex;
    render_ts_funcs[82] = sogl_ts_n5_m1;
    render_ts_funcs[83] = sogl_ts_n5_m1_tex;
    render_ts_funcs[84] = sogl_ts_n5_m2;
    render_ts_funcs[85] = sogl_ts_n5_m2_tex;
    render_ts_funcs[86] = sogl_ts_n5_m3;
    render_ts_funcs[87] = sogl_ts_n5_m3_tex;
    render_ts_funcs[88] = sogl_ts_n5_m4;
    render_ts_funcs[89] = sogl_ts_n5_m4_tex;
    render_ts_funcs[90] = sogl_ts_n5_m5;
    render_ts_funcs[91] = sogl_ts_n5_m5_tex;
    render_ts_funcs[92] = sogl_ts_n5_m6;
    render_ts_funcs[93] = sogl_ts_n5_m6_tex;

    render_ts_funcs[96] = sogl_ts_n6_m0;
    render_ts_funcs[97] = sogl_ts_n6_m0_tex;
    render_ts_funcs[98] = sogl_ts_n6_m1;
    render_ts_funcs[99] = sogl_ts_n6_m1_tex;
    render_ts_funcs[100] = sogl_ts_n6_m2;
    render_ts_funcs[101] = sogl_ts_n6_m2_tex;
    render_ts_funcs[102] = sogl_ts_n6_m3;
    render_ts_funcs[103] = sogl_ts_n6_m3_tex;
    render_ts_funcs[104] = sogl_ts_n6_m4;
    render_ts_funcs[105] = sogl_ts_n6_m4_tex;
    render_ts_funcs[106] = sogl_ts_n6_m5;
    render_ts_funcs[107] = sogl_ts_n6_m5_tex;
    render_ts_funcs[108] = sogl_ts_n6_m6;
    render_ts_funcs[109] = sogl_ts_n6_m6_tex;
  }

  int idx = (nbind << 4) | (mbind << 1) | texture;

  // just in case someone forgot...
  if (matindices == NULL) matindices = vertexindices;
  if (normindices == NULL) normindices = vertexindices;

  render_ts_funcs[idx](vertexlist,
                       vertexindices,
                       num_vertexindices,
                       normals,
                       normindices,
                       materials,
                       matindices,
                       texcoords,
                       texindices);
}

#undef OVERALL
#undef PER_STRIP
#undef PER_STRIP_INDEXED
#undef PER_TRIANGLE
#undef PER_TRIANGLE_INDEXED
#undef PER_VERTEX
#undef PER_VERTEX_INDEXED


#endif // !NO_TRISTRIPSET_RENDER

// **************************************************************************
