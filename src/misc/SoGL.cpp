/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <math.h>

// NOT IN USE?? 19990925 mortene.
#if 0
//
// function used to check if an extension is supported
// based on glut 3.3 function
//
static int
extensionSupported(char *extension)
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
#endif // NOT IN USE??

static SbBool isInitialized = FALSE;
static int maxTextureSize;
static int maxLights;
static int maxClipPlanes;
static int maxModelviewStackDepth;
static int maxProjectionStackDepth;
static int maxTextureStackDepth;

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
  polygonOffsetEXT = extensionSupported("GL_EXT_polygon_offset");
#if 0 // debug
  fprintf(stderr,"polygon offset ext: %d\n", polygonOffsetEXT);
#endif // debug
#endif

#if GL_VERSION_1_1
#elif GL_EXT_texture_object
  textureObjectEXT = extensionSupported("GL_EXT_texture_object");
#if 0 // debug
  fprintf(stderr,"texture object ext: %d\n", textureObjectEXT);
#endif // debug
#endif

#if GL_VERSION_1_1
#elif GL_EXT_vertex_array
  vertexArrayEXT = extensionSupported("GL_EXT_vertex_array");
#if 0 // debug
  fprintf(stderr,"vertex arrayext: %d\n", vertexArrayEXT);
#endif // debug
#endif

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

#if 0 // debug
  fprintf(stderr,"max texure size: %d\n", maxTextureSize);
  fprintf(stderr,"max modelview matrix depth: %d\n", maxModelviewStackDepth);
  fprintf(stderr,"max lights: %d\n", maxLights);
  fprintf(stderr,"max projection matrix depth: %d\n", maxProjectionStackDepth);
  fprintf(stderr,"max texture stack depth: %d\n", maxTextureStackDepth);
  fprintf(stderr,"max clip planes: %d\n", maxClipPlanes);
#endif // debug

}


// generate a 3d circle in the x-z plane
static void
generate_3d_circle(SbVec3f *coords, const int num, const float radius, const float y)
{
  float delta = 2.0f*float(SB_PI)/float(num);
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
generate_2d_circle(SbVec2f *coords, const int num, const float radius)
{
  float delta = 2.0f*float(SB_PI)/float(num);
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

  generate_3d_circle(coords, slices, radius, -h2);
  coords[slices] = coords[0];

  if (flags & SOGL_NEED_NORMALS) {
    double a = atan(height/radius);
    generate_3d_circle(normals, slices, float(sin(a)), float(cos(a)));
    normals[slices] = normals[0];
    normals[slices+1] = normals[1];
  }

  int matnr = 0;

//   if (flags & SOGL_RENDER_SIDE) {
//     glBegin(GL_QUAD_STRIP);
//     i = 0;

//     float t = 0.0;
//     float inc = 1.0f / slices;

//     while (i <= slices) {
//       if (flags & SOGL_NEED_TEXCOORDS) {
//      glTexCoord2f(t, 1.0f);
//       }
//       if (flags & SOGL_NEED_NORMALS) {
//      SbVec3f n = (normals[i] + normals[i+1])*0.5f;
//      glNormal3f(n[0], n[1], n[2]);
//      // FIXME: what is nicest here?
//      //      glNormal3fv((const GLfloat*)&normals[i]);
//      //      glNormal3f(0.0f, 1.0f, 0.0f);
//       }
//       glVertex3f(0.0f, h2, 0.0f);
//       if (flags & SOGL_NEED_TEXCOORDS) {
//      glTexCoord2f(t, 0.0f);
//       }
//       if (flags & SOGL_NEED_NORMALS) {
//      glNormal3fv((const GLfloat*)&normals[i]);
//       }

//       glVertex3fv((const GLfloat*)&coords[i]);
//       i++;
//       t += inc;
//     }

//     matnr++;
//     glEnd();
//   }


  if (flags & SOGL_RENDER_SIDE) {
    glBegin(GL_TRIANGLES);
    i = 0;

    float t = 0.0;
    float inc = 1.0f / slices;

    while (i < slices) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(t + inc*0.5f, 1.0f);
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
        glTexCoord2f(t+inc, 0.0f);
      }
      if (flags & SOGL_NEED_NORMALS) {
        glNormal3fv((const GLfloat*)&normals[i+1]);
      }
      glVertex3fv((const GLfloat*)&coords[i+1]);

      i++;
      t += inc;
    }

    matnr++;
    glEnd();
  }



  if (flags & SOGL_RENDER_BOTTOM) {
    if (flags & SOGL_NEED_TEXCOORDS) {
      generate_2d_circle(texcoords, slices, 0.5f);
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

  generate_3d_circle(coords, slices, radius, -h2);
  coords[slices] = coords[0];

  if (flags & SOGL_NEED_NORMALS) {
    generate_3d_circle(normals, slices, 1.0f, 0.0f);
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
    generate_2d_circle(texcoords, slices, 0.5f);
    texcoords[slices] = texcoords[0];
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
    matnr++;
  }
  if (flags & SOGL_RENDER_TOP) {
    if (flags & SOGL_MATERIAL_PER_PART) {
      material->send(matnr, TRUE);
    }
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (i = 0; i < slices; i++) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2f(texcoords[i][0]+0.5f, texcoords[i][1]+0.5f);
      }
      const SbVec3f &c = coords[i];
      glVertex3f(c[0], h2, c[2]);
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
  SbVec2f texcoords[129];

  int i, j;
  float rho;
  float drho;
  float theta;
  float dtheta;
  float ts, tc;
  SbVec3f tmp;

  drho = float(SB_PI) / (float) (stacks-1);
  dtheta = 2.0f * float(SB_PI) / (float) slices;

  i = 0;
  for (j = 0; j <= slices; j++) {
    texcoords[j].setValue((float)j/slices, (float)(stacks-1-i)/(stacks-1));
    normals[j].setValue(0.0f, 1.0f, 0.0f);
    coords[j].setValue(0.0f, radius, 0.0f);
  }

  rho = drho;
  for (i = 1; i < stacks; i++) {
    tc = (float)cos(rho);
    ts = (float)sin(rho);
    glBegin(GL_QUAD_STRIP);
    theta = 0.0f;
    for (j = 0; j <= slices; j++) {
      glTexCoord2fv((const GLfloat*)&texcoords[j]);
      glNormal3fv((const GLfloat*)&normals[j]);
      glVertex3fv((const GLfloat*)&coords[j]);

      texcoords[j].setValue((float)j/slices, (float)(stacks-1-i)/(stacks-1));
      glTexCoord2f(texcoords[j][0], texcoords[j][1]);

      tmp.setValue(float(cos(theta))*ts,
                   tc,
                   -float(sin(theta))*ts);
      normals[j] = tmp;
      glNormal3f(tmp[0], tmp[1], tmp[2]);
      tmp *= radius;
      glVertex3f(tmp[0], tmp[1], tmp[2]);
      coords[j] = tmp;
      theta += dtheta;
    }
    glEnd();
    rho += drho;
  }
}

//
// the 12 triangles in the cube
//
static int cube_vindices[] =
{
  0, 1, 3, 2,
  1, 5, 7, 3,
  5, 4, 6, 7,
  4, 0, 2, 6,
  4, 5, 1, 0,
  2, 3, 7, 6
};

static int cube_tindices[] =
{
  3, 2, 1, 0,
  3, 2, 1, 0,
  3, 2, 1, 0,
  3, 2, 1, 0,
  3, 2, 1, 0,
  3, 2, 1, 0
};

static float cube_texcoords[] =
{
  0.0f, 0.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 1.0f
};

//
// a cube needs 6 normals
//
static SbVec3f cube_normals[] =
{
  SbVec3f(0.0f, 0.0f, 1.0f),
  SbVec3f(-1.0f, 0.0f, 0.0f),
  SbVec3f(0.0f, 0.0f, -1.0f),
  SbVec3f(1.0f, 0.0f, 0.0f),
  SbVec3f(0.0f, 1.0f, 0.0f),
  SbVec3f(0.0f, -1.0f, 0.0f),
};

static void
generate_cube_vertices(SbVec3f *varray,
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
  generate_cube_vertices(varray,
                         width * 0.5f,
                         height * 0.5f,
                         depth * 0.5f);
  glBegin(GL_QUADS);
  int *iptr = cube_vindices;
  int *tptr = cube_tindices;

  for (int i = 0; i < 6; i++) { // 6 quads
    if (flags & SOGL_NEED_NORMALS)
      glNormal3fv((const GLfloat*)&cube_normals[i]);
    if (flags & SOGL_MATERIAL_PER_PART)
      material->send(i, TRUE);
    for (int j = 0; j < 4; j++) {
      if (flags & SOGL_NEED_TEXCOORDS) {
        glTexCoord2fv(&cube_texcoords[*tptr++ * 2]);
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
  SoDebugError::postInfo("sogl_free_texture", "Free texture: %d\n", index);
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
                      const float quality,
                      const unsigned char * const texture,
                      const int numComponents,
                      const int w, const int h,
                      const SbBool dlist)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  wrapS ? GL_CLAMP : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  wrapT ? GL_CLAMP : GL_REPEAT);

  //
  // for now, if quality >= 0.5, we create mipmaps. The texture quality
  // should probably be based on detected hardware though.
  //
  if (quality < 0.1f) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  else if (quality < 0.5f) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  else if (quality < 0.8f) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  }
  else { // max quality
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    assert(0);
    glformat = GL_RGB; // Unnecessary, but kills a compiler warning.
  }

  if (quality < 0.5f) {
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
                    const float quality,
                    const unsigned char * const texture,
                    const int numComponents,
                    const int w, const int h)
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

  really_create_texture(wrapS, wrapT, quality, texture,
                        numComponents, w, h, dlist);

  if (dlist)
    glEndList();

#if 0 && COIN_DEBUG
  fprintf(stderr,"Creating texture: %d\n", index);
#endif

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

  // fprintf(stderr,"rendering faceset: %d %d %d ==> %d\n",
  //      nbind, mbind, texture, idx);

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
