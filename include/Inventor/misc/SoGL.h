#ifndef COIN_SOGL_H
#define COIN_SOGL_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

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
class SbVec3f;
class SbVec2f;

// flags for cone, cylinder and cube

#define SOGL_RENDER_SIDE       0x01
#define SOGL_RENDER_TOP        0x02
#define SOGL_RENDER_BOTTOM     0x04
#define SOGL_MATERIAL_PER_PART 0x08
#define SOGL_NEED_NORMALS      0x10
#define SOGL_NEED_TEXCOORDS    0x20
#define SOGL_NEED_3DTEXCOORDS  0x40

// Convenience function for access to OpenGL wrapper from an SoState
// pointer.
const cc_glglue * sogl_glue_instance(const SoState * state);


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

// nurbs rendering and tessellation
void sogl_render_nurbs_surface(SoAction * action, SoShape * shape,
                               void * nurbsrenderer,
                               const int numuctrlpts, const int numvctrlpts,
                               const float * uknotvec, const float * vknotvec,
                               const int numuknot, const int numvknot,
                               const int numsctrlpts, const int numtctrlpts,
                               const float * sknotvec, const float * tknotvec,
                               const int numsknot, const int numtknot,
                               const SbBool glrender,
                               const int numcoordindex = 0, const int32_t * coordindex = NULL,
                               const int numtexcoordindex = 0, const int32_t * texcoordindex = NULL);

void sogl_render_nurbs_curve(SoAction * action, SoShape * shape,
                             void * nurbsrenderer,
                             const int numctrlpts,
                             const float * knotvec,
                             const int numknots,
                             const SbBool glrender,
                             const SbBool drawaspoints = FALSE,
                             const int numcoordindex = 0, const int32_t * coordindex = NULL);

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

void
sogl_render_pointset(const SoGLCoordinateElement * coords,
                     const SbVec3f * normals,
                     SoMaterialBundle * mb,
                     const SoTextureCoordinateBundle * tb,
                     int32_t numpts,
                     int32_t idx);

SbBool sogl_glerror_debugging(void);
SbString sogl_glerror_string(int err);

int sogl_autocache_get_min_limit(SoState * state);
int sogl_autocache_get_max_limit(SoState * state);

#endif // !COIN_SOGL_H
