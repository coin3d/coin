#ifndef COIN_SONORMALGENERATOR_H
#define COIN_SONORMALGENERATOR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbVec3f.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/system/inttypes.h>

class COIN_DLL_API SoNormalGenerator {
public:
  SoNormalGenerator(const SbBool ccw, const int approxVertices = 64);
  ~SoNormalGenerator();

  void reset(const SbBool ccw);
  
  void beginPolygon();
  void polygonVertex(const SbVec3f &v);
  void endPolygon();

  void triangle(const SbVec3f &v0,
                const SbVec3f &v1,
                const SbVec3f &v2);
  void quad(const SbVec3f &v0,
            const SbVec3f &v1,
            const SbVec3f &v2,
            const SbVec3f &v3);

  void generate(const float creaseAngle,
                const int32_t * striplens = NULL,
                const int numstrips = 0);

  void generatePerStrip(const int32_t * striplens,
                        const int numstrips);
  void generatePerFace(void);
  void generateOverall(void);

  // call these only after generate
  int getNumNormals() const;
  void setNumNormals(const int num);
  const SbVec3f *getNormals() const;
  const SbVec3f & getNormal(const int32_t i) const;
  void setNormal(const int32_t index, const SbVec3f &normal);

private:
  SbBSPTree bsp;
  SbList <int> vertexList;
  SbList <int> vertexFace;
  SbList <SbVec3f> faceNormals;
  SbList <SbVec3f> vertexNormals;

  SbBool ccw;
  SbBool perVertex;
  int currFaceStart;

  SbVec3f calcFaceNormal();
};

#endif // !COIN_SONORMALGENERATOR_H
