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

#ifndef _sogl_h_
#define _sogl_h_

//
// this file containes some useful (and necessary) GL wraparounds.
// The sogl_global_init() function _must_ be called once before
// any OpenGL rendering can occur.
//
// Mainly this file has functions for checking limits and which
// extensions are available for the GL implementation.
//

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoMaterialBundle;
class SoGLCoordinateElement;
class SoTextureCoordinateBundle;
class SbVec3f;
class SbVec2f;

void sogl_global_init();

// limit functions
int sogl_max_texture_size();
int sogl_max_lights();
int sogl_max_clip_planes();
int sogl_max_modelview_stack_depth();
int sogl_max_projection_stack_depth();
int sogl_max_texture_stack_depth();

// extension queries
SbBool sogl_texture_object_ext();
SbBool sogl_polygon_offset_ext();
SbBool sogl_vertex_array_ext();

// flags for cone, cylinder and cube

#define SOGL_RENDER_SIDE       0x01
#define SOGL_RENDER_TOP        0x02
#define SOGL_RENDER_BOTTOM     0x04
#define SOGL_MATERIAL_PER_PART 0x08
#define SOGL_NEED_NORMALS      0x10
#define SOGL_NEED_TEXCOORDS    0x20

// render
void sogl_render_cone(const float bottomRadius,
                      const float height,
                      const int numslices,
                      SoMaterialBundle * const material,
                      const unsigned int flags);


void sogl_render_cylinder(const float radius,
                          const float height,
                          const int numslices,
                          SoMaterialBundle * const material,
                          const unsigned int flags);

void sogl_render_sphere(const float radius,
                        const int numstacks,
                        const int numslices,
                        SoMaterialBundle * const material,
                        const unsigned int flags);

void sogl_render_cube(const float width,
                      const float height,
                      const float depth,
                      SoMaterialBundle * const material,
                      const unsigned int flags);

// convenience functions
void sogl_polygon_offset(const int onoff, const float numpixels = 1.0f);
unsigned int sogl_create_texture(const int wrapS, const int wrapT,
                                 const float quality,
                                 const unsigned char * const texture,
                                 const int numComponents,
                                 const int w, const int h);
void sogl_apply_texture(const unsigned int handle);
void sogl_free_texture(unsigned int handle);

//
// optimized faceset rendering functions.
// the functions are automagically generated based on a template function.
// in addition to these 50 different functions, the template also generates
// different versions based on the OpenGL version, and if the vertex array
// extension is available for OpenGL 1.0 implementations.
// OpenGL 1.2 features is not currently used :(
//
//
//

void sogl_render_faceset(const SoGLCoordinateElement * const coords,
                         const int32_t *vertexindices,
                         int num_vertexindices,
                         const SbVec3f *normals,
                         const int32_t *normindices,
                         SoMaterialBundle * const materials,
                         const int32_t *matindices,
                         SoTextureCoordinateBundle * const texcoords,
                         const int32_t *texindices,
                         const int nbind,
                         const int mbind,
                         const int texture);

void
sogl_render_tristrip(const SoGLCoordinateElement * const coords,
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
                     const int texture);

void
sogl_render_lineset(const SoGLCoordinateElement * const coords,
                    const int32_t *vertexindices,
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
                    const int drawAsPoints);

#endif // _sogl_h_
