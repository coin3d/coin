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

#ifndef COIN_SOVRMLINDEXEDSHAPE_H
#define COIN_SOVRMLINDEXEDSHAPE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLVertexShape.h>
#include <Inventor/fields/SoMFInt32.h>

class COIN_DLL_API SoVRMLIndexedShape : public SoVRMLVertexShape
{
  typedef SoVRMLVertexShape inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLIndexedShape);

public:
  static void initClass(void);

  SoMFInt32 coordIndex;
  SoMFInt32 colorIndex;
  SoMFInt32 normalIndex;
  SoMFInt32 texCoordIndex;

protected:
  SoVRMLIndexedShape(void);
  virtual ~SoVRMLIndexedShape();

  virtual void notify(SoNotList * list);
  virtual void computeBBox(SoAction * action,
                           SbBox3f & box, SbVec3f & center);

  SbBool getVertexData(SoState * state,
                       const SoCoordinateElement *& coords,
                       const SbVec3f *& normals,
                       const int32_t *& cindices,
                       const int32_t *& nindices,
                       const int32_t *& tindices,
                       const int32_t *& mindices,
                       int & numcindices,
                       const SbBool neednormals,
                       SbBool & normalcacheused);

};

#endif // ! COIN_SOVRMLINDEXEDSHAPE_H
