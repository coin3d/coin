#ifndef COIN_SOINDEXEDLINESET_H
#define COIN_SOINDEXEDLINESET_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>

#define SO_END_LINE_INDEX (-1)

class COIN_DLL_API SoIndexedLineSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedLineSet);

public:
  static void initClass(void);
  SoIndexedLineSet(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoIndexedLineSet();

private:
  virtual void generatePrimitives(SoAction * action);

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * bundle);
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalCache * nc);

  enum Binding {
    OVERALL = 0,
    PER_SEGMENT,
    PER_SEGMENT_INDEXED,
    PER_LINE,
    PER_LINE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  Binding findNormalBinding(SoState * state);
  Binding findMaterialBinding(SoState * state);
};

#endif // !COIN_SOINDEXEDLINESET_H
