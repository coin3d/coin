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
