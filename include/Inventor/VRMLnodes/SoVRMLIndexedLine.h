/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLINDEXEDLINE_H
#define COIN_SOVRMLINDEXEDLINE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLVertexLine.h>
#include <Inventor/fields/SoMFInt32.h>

class COIN_DLL_API SoVRMLIndexedLine : public SoVRMLVertexLine
{
  typedef SoVRMLVertexLine inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLIndexedLine);

public:
  static void initClass(void);

  SoMFInt32 coordIndex;
  SoMFInt32 colorIndex;

protected:
  SoVRMLIndexedLine(void);
  virtual ~SoVRMLIndexedLine();

  virtual void computeBBox(SoAction * action,
                           SbBox3f & box, SbVec3f & center);
  
  int getNumVerts( int startCoord );
  void setupIndices( int numFaces );
  const int32_t * getColorIndices(void);

  virtual void notify( SoNotList * list );

}; // class SoVRMLIndexedLine

#endif // ! COIN_SOVRMLINDEXEDLINE_H
