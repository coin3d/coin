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


#include <Inventor/misc/SoGenerate.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/details/SoConeDetail.h>

#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>

#include <math.h>

// generate a 3d circle in the x-z plane
static void
generate_3d_circle(SbVec3f *coords, const int num, const float radius, const float y)
{
  float delta = 2*SB_PI/num;
  float angle = 0.0f;
  for (int i = 0; i < num; i++) {
    coords[i][0] = -sin(angle) * radius;
    coords[i][1] = y;
    coords[i][2] = -cos(angle) * radius;
    angle += delta;
  }
}

// generate a 2d circle
static void
generate_2d_circle(SbVec2f *coords, const int num, const float radius)
{
  float delta = 2*SB_PI/num;
  float angle = 0.0f;
  for (int i = 0; i < num; i++) {
    coords[i][0] = -sin(angle) * radius;
    coords[i][1] = -cos(angle) * radius;
    angle += delta;
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

static SbVec2f cube_texcoords[] =
{
  SbVec2f(0.0f, 0.0f),
  SbVec2f(1.0f, 0.0f),
  SbVec2f(1.0f, 1.0f),
  SbVec2f(0.0f, 1.0f)
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

class so_generate_prim_private {
public:
  static void generate_cone(const float radius,
                            const float height,
                            const int numslices,
                            const unsigned int flags,
                            SoShape * const shape,
                            SoAction * const action) {
    int i;
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

    double a = atan(height/radius);
    generate_3d_circle(normals, slices, sin(a), cos(a));
    normals[slices] = normals[0];
    normals[slices+1] = normals[1];

    int matnr = 0;

    SoPrimitiveVertex vertex;
    SoConeDetail sideDetail;
    SoConeDetail bottomDetail;
    sideDetail.setPart(SoCone::SIDES);
    bottomDetail.setPart(SoCone::BOTTOM);

    if (flags & SOGEN_GENERATE_SIDE) {
      vertex.setDetail(&sideDetail);
      vertex.setMaterialIndex(matnr);

      shape->beginShape(action, SoShape::TRIANGLES);
      i = 0;

      float t = 0.0;
      float inc = 1.0f / slices;

      while (i < slices) {
        vertex.setTextureCoords(SbVec2f(t + inc*0.5f, 1.0f));
        vertex.setNormal((normals[i] + normals[i+1])*0.5f);
        vertex.setPoint(SbVec3f(0.0f, h2, 0.0f));
        shape->shapeVertex(&vertex);

        vertex.setTextureCoords(SbVec2f(t, 0.0f));
        vertex.setNormal(normals[i]);
        vertex.setPoint(coords[i]);
        shape->shapeVertex(&vertex);

        vertex.setTextureCoords(SbVec2f(t+inc, 0.0f));
        vertex.setNormal(normals[i+1]);
        vertex.setPoint(coords[i+1]);
        shape->shapeVertex(&vertex);

        i++;
        t += inc;
      }
      if (flags & SOGEN_MATERIAL_PER_PART) matnr++;
      shape->endShape();
    }

    if (flags & SOGEN_GENERATE_BOTTOM) {
      vertex.setDetail(&bottomDetail);
      vertex.setMaterialIndex(matnr);

      generate_2d_circle(texcoords, slices, 0.5f);
      texcoords[slices] = texcoords[0];

      shape->beginShape(action, SoShape::TRIANGLE_FAN);
      vertex.setNormal(SbVec3f(0.0f, -1.0f, 0.0f));
      for (i = slices-1; i >= 0; i--) {
        vertex.setTextureCoords(texcoords[i]+SbVec2f(0.5f, 0.5f));
        vertex.setPoint(coords[i]);
        shape->shapeVertex(&vertex);
      }
      shape->endShape();
    }
  }

  static void generate_cylinder(const float radius,
                                const float height,
                                const int numslices,
                                const unsigned int flags,
                                SoShape * const shape,
                                SoAction * const action) {
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

    generate_3d_circle(normals, slices, 1.0f, 0.0f);
    normals[slices] = normals[0];
    normals[slices+1] = normals[1];

    int matnr = 0;

    SoPrimitiveVertex vertex;
    SoCylinderDetail sideDetail;
    SoCylinderDetail bottomDetail;
    SoCylinderDetail topDetail;
    sideDetail.setPart(SoCylinder::SIDES);
    bottomDetail.setPart(SoCylinder::BOTTOM);
    topDetail.setPart(SoCylinder::TOP);

    if (flags & SOGEN_GENERATE_SIDE) {
      shape->beginShape(action, SoShape::QUAD_STRIP);
      vertex.setDetail(&sideDetail);
      vertex.setMaterialIndex(matnr);
      i = 0;

      float t = 0.0;
      float inc = 1.0f / slices;

      while (i <= slices) {
        vertex.setTextureCoords(SbVec2f(t, 1.0f));
        vertex.setNormal(normals[i]);
        SbVec3f c = coords[i];
        vertex.setPoint(SbVec3f(c[0], h2, c[2]));
        shape->shapeVertex(&vertex);

        vertex.setTextureCoords(SbVec2f(t, 0.0f));
        vertex.setPoint(c);
        i++;
        t += inc;
      }
      if (flags & SOGEN_MATERIAL_PER_PART) matnr++;
      shape->endShape();
    }

    if (flags & (SOGEN_GENERATE_BOTTOM | SOGEN_GENERATE_TOP)) {
      generate_2d_circle(texcoords, slices, 0.5f);
      texcoords[slices] = texcoords[0];
    }

    if (flags & SOGEN_GENERATE_BOTTOM) {
      vertex.setMaterialIndex(matnr);
      vertex.setDetail(&bottomDetail);
      shape->beginShape(action, SoShape::TRIANGLE_FAN);
      vertex.setNormal(SbVec3f(0.0f, -1.0f, 0.0f));

      for (i = slices-1; i >= 0; i--) {
        vertex.setTextureCoords(texcoords[i] + SbVec2f(0.5f, 0.5f));
        vertex.setPoint(coords[i]);
        shape->shapeVertex(&vertex);
      }
      shape->endShape();
      if (flags & SOGEN_MATERIAL_PER_PART) matnr++;
    }
    if (flags & SOGEN_GENERATE_TOP) {
      vertex.setMaterialIndex(matnr);
      vertex.setDetail(&topDetail);
      vertex.setNormal(SbVec3f(0.0f, 1.0f, 0.0f));
      shape->beginShape(action, SoShape::TRIANGLE_FAN);

      for (i = 0; i < slices; i++) {
        vertex.setTextureCoords(texcoords[i] + SbVec2f(0.5f, 0.5f));
        const SbVec3f &c = coords[i];
        vertex.setPoint(SbVec3f(c[0], h2, c[2]));
      }
      shape->endShape();
    }
  }

  static void generate_cube(const float width,
                            const float height,
                            const float depth,
                            const unsigned int flags,
                            SoShape * const shape,
                            SoAction * const action) {
    static int translate[6] = {0, 2, 1, 3, 4, 5};
    SbVec3f varray[8];
    generate_cube_vertices(varray,
                           width * 0.5f,
                           height * 0.5f,
                           depth * 0.5f);


    SoPrimitiveVertex vertex;
    SoCubeDetail cubeDetail;
    vertex.setDetail(&cubeDetail);
    vertex.setMaterialIndex(0);

    shape->beginShape(action, SoShape::QUADS);
    int *iptr = cube_vindices;
    int *tptr = cube_tindices;

    for (int i = 0; i < 6; i++) { // 6 quads
      vertex.setNormal(cube_normals[i]);
      if (flags & SOGEN_MATERIAL_PER_PART) vertex.setMaterialIndex(i);
      for (int j = 0; j < 4; j++) {
        vertex.setTextureCoords(cube_texcoords[*tptr++]);
        vertex.setPoint(varray[*iptr++]);
        shape->shapeVertex(&vertex);
      }
    }
    shape->endShape();
  }

  static void generate_sphere(const float radius,
                              const int numstacks,
                              const int numslices,
                              SoShape * const shape,
                              SoAction * const action) {
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

    SoPrimitiveVertex vertex;
    // no details for sphere

    drho = SB_PI / (float) (stacks-1);
    dtheta = 2.0f * SB_PI / (float) slices;

    i = 0;
    for (j = 0; j <= slices; j++) {
      texcoords[j].setValue((float)j/slices, (float)(stacks-1-i)/(stacks-1));
      normals[j].setValue(0.0f, 1.0f, 0.0f);
      coords[j].setValue(0.0f, radius, 0.0f);
    }

    rho = drho;
    for (i = 1; i < stacks; i++) {
      tc = cos(rho);
      ts = sin(rho);
      shape->beginShape(action, SoShape::QUAD_STRIP);
      theta = 0.0f;
      for (j = 0; j <= slices; j++) {
        vertex.setTextureCoords(texcoords[j]);
        vertex.setNormal(normals[j]);
        vertex.setPoint(coords[j]);
        shape->shapeVertex(&vertex);

        texcoords[j].setValue((float)j/slices, (float)(stacks-1-i)/(stacks-1));
        vertex.setTextureCoords(texcoords[j]);
        tmp.setValue(cos(theta)*ts,
                     tc,
                     -sin(theta)*ts);
        normals[j] = tmp;
        vertex.setNormal(tmp);
        tmp *= radius;
        vertex.setPoint(tmp);
        coords[j] = tmp;
        theta += dtheta;
      }
      shape->endShape();
      rho += drho;
    }
  }
};

void
sogen_generate_cone(const float bottomRadius,
                    const float height,
                    const int numslices,
                    const unsigned int flags,
                    SoShape * const shape,
                    SoAction * const action)
{
  so_generate_prim_private::generate_cone(bottomRadius,
                                          height,
                                          numslices,
                                          flags,
                                          shape,
                                          action);
}

void
sogen_generate_cylinder(const float radius,
                        const float height,
                        const int numslices,
                        const unsigned int flags,
                        SoShape * const shape,
                        SoAction * const action)
{
  so_generate_prim_private::generate_cylinder(radius,
                                              height,
                                              numslices,
                                              flags,
                                              shape,
                                              action);
}

void
sogen_generate_sphere(const float radius,
                      const int numstacks,
                      const int numslices,
                      SoShape * const shape,
                      SoAction * const action)
{
  so_generate_prim_private::generate_sphere(radius,
                                            numstacks,
                                            numslices,
                                            shape,
                                            action);
}

void
sogen_generate_cube(const float width,
                    const float height,
                    const float depth,
                    const unsigned int flags,
                    SoShape * const shape,
                    SoAction * const action)
{
  so_generate_prim_private::generate_cube(width,
                                          height,
                                          depth,
                                          flags,
                                          shape,
                                          action);
}
