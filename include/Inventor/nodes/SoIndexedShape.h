#ifndef COIN_SOINDEXEDSHAPE_H
#define COIN_SOINDEXEDSHAPE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/fields/SoMFInt32.h>

class SoTextureCoordinateElement;
class SoCoordinateElement;

class COIN_DLL_API SoIndexedShape : public SoVertexShape {
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
};

#endif // !COIN_SOINDEXEDSHAPE_H
