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

#ifndef COIN_SOVRMLINDEXEDLINESET_H
#define COIN_SOVRMLINDEXEDLINESET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedLine.h>

#undef SO_END_LINE_INDEX
#define SO_END_LINE_INDEX -1

class COIN_DLL_API SoVRMLIndexedLineSet : public SoVRMLIndexedLine
{
  typedef SoVRMLIndexedLine inherited;
  SO_NODE_HEADER(SoVRMLIndexedLineSet);

public:
  static void initClass(void);
  SoVRMLIndexedLineSet(void);

  virtual void GLRender( SoGLRenderAction * action );
  virtual void getPrimitiveCount( SoGetPrimitiveCountAction * action );
  virtual void getBoundingBox( SoGetBoundingBoxAction * action );

protected:
  virtual ~SoVRMLIndexedLineSet();
  virtual void generatePrimitives( SoAction * action );

private:
  enum Binding {
    // Needs to be these specific values to match the rendering code
    // in SoGL.cpp.  FIXME: bad dependency. 20020805 mortene.
    OVERALL = 0,
    PER_LINE = 3,
    PER_LINE_INDEXED = 4,
    PER_VERTEX = 5 ,
    PER_VERTEX_INDEXED = 6
  };

  Binding findMaterialBinding(void) const;

}; // class SoVRMLIndexedLineSet

#endif // ! COIN_SOVRMLINDEXEDLINESET_H
