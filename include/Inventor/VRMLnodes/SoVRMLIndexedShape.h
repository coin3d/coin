/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
