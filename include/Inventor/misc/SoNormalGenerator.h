/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SONORMALGENERATOR_H
#define COIN_SONORMALGENERATOR_H

#include <Inventor/SbVec3f.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/system/inttypes.h>

class COIN_DLL_EXPORT SoNormalGenerator {
public:
  SoNormalGenerator(const SbBool ccw, const int approxVertices = 64);
  ~SoNormalGenerator();

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
  const SbVec3f &getNormal(const int32_t i) const;
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
