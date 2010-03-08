#ifndef COIN_SOGL_H
#define COIN_SOGL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

// This file contains GL code which is shared internally. It contains
// mostly rendering code shared between several node types.

#include <Inventor/SbString.h>
#include <Inventor/C/glue/gl.h>

class SoShape;
class SoState;
class SoAction;
class SoMaterialBundle;
class SoGLCoordinateElement;
class SoTextureCoordinateBundle;
class SoVertexAttributeBundle;
class SbVec3f;
class SbVec2f;

// flags for cone, cylinder and cube

#define SOGL_RENDER_SIDE         0x01
#define SOGL_RENDER_TOP          0x02
#define SOGL_RENDER_BOTTOM       0x04
#define SOGL_MATERIAL_PER_PART   0x08
#define SOGL_NEED_NORMALS        0x10
#define SOGL_NEED_TEXCOORDS      0x20
#define SOGL_NEED_3DTEXCOORDS    0x40
#define SOGL_NEED_MULTITEXCOORDS 0x80 // internal

// Convenience function for access to OpenGL wrapper from an SoState
// pointer.
const cc_glglue * sogl_glue_instance(const SoState * state);


// render
void sogl_render_cone(const float bottomRadius,
                      const float height,
                      const int numslices,
                      SoMaterialBundle * const material,
                      const unsigned int flags,
                      SoState * state);


void sogl_render_cylinder(const float radius,
                          const float height,
                          const int numslices,
                          SoMaterialBundle * const material,
                          const unsigned int flags,
                          SoState * state);

void sogl_render_sphere(const float radius,
                        const int numstacks,
                        const int numslices,
                        SoMaterialBundle * const material,
                        const unsigned int flags,
                        SoState * state);

void sogl_render_cube(const float width,
                      const float height,
                      const float depth,
                      SoMaterialBundle * const material,
                      const unsigned int flags,
                      SoState * state);

// FIXME: must be kept around due to ABI & API compatibility reasons
// for now, but should consider taking it out for the next major Coin
// release. 20030519 mortene.
void sogl_offscreencontext_callback(void (*cb)(void *, SoAction*),
                                    void * closure);

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
                         SoVertexAttributeBundle * const attribs,
                         const int nbind,
                         const int mbind,
                         const int attribbind,
                         const int dotexture,
                         const int doattribs);

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

void
sogl_render_pointset(const SoGLCoordinateElement * coords,
                     const SbVec3f * normals,
                     SoMaterialBundle * mb,
                     const SoTextureCoordinateBundle * tb,
                     int32_t numpts,
                     int32_t idx);

SbBool sogl_glerror_debugging(void);

void sogl_autocache_update(SoState * state, const int numprimitives, 
                           SbBool didusevbo);

#endif // !COIN_SOGL_H
