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

#ifndef COIN_SOINDEXEDSHAPE_H
#define COIN_SOINDEXEDSHAPE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/fields/SoMFInt32.h>

class SoTextureCoordinateElement;
class SoCoordinateElement;


class SoIndexedShape : public SoVertexShape {
  typedef SoVertexShape inherited;

  SO_NODE_ABSTRACT_HEADER(SoIndexedShape);

public:
  static void initClass(void);

  SoMFInt32 coordIndex;
  SoMFInt32 materialIndex;
  SoMFInt32 normalIndex;
  SoMFInt32 textureCoordIndex;

protected:
  SoIndexedShape(void);
  virtual ~SoIndexedShape();

  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  int getNumVerts(const int startCoord);
  void setupIndices(const int numParts, const int numFaces,
                    const SbBool needNormals, const SbBool needTexCoords);

  const int32_t * getNormalIndices(void);
  const int32_t * getColorIndices(void);
  const int32_t * getTexCoordIndices(void);

  virtual void notify(SoNotList * list);

  static SbBool areTexCoordsIndexed(SoAction * action);

  SbBool getVertexData(SoState * state,
                       const SoCoordinateElement *& coords,
                       const SbVec3f *& normals,
                       const int32_t *& cindices,
                       const int32_t *& nindices,
                       const int32_t *& tindices,
                       const int32_t *& mindices,
                       int & numcindices,
                       const SbBool needNormals,
                       SbBool & normalCacheUsed);

private:
  virtual SbBool generateDefaultNormals(SoState * state,
                                        SoNormalCache * cache);

  int32_t * tIndices;
  int32_t * nIndices;
  int32_t * mIndices;
};


#endif // !COIN_SOINDEXEDSHAPE_H
