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

#ifndef __SOINDEXEDLINESET_H__
#define __SOINDEXEDLINESET_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>

#define SO_END_LINE_INDEX (-1)


class SoIndexedLineSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedLineSet);

public:
  static void initClass(void);
  SoIndexedLineSet(void);

  enum Binding {
    OVERALL = 0,
    PER_SEGMENT,
    PER_SEGMENT_INDEXED,
    PER_LINE,
    PER_LINE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };


  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willSetShapeHints() const;
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoIndexedLineSet();

private:
  virtual void generatePrimitives(SoAction * action);

  SbBool generateDefaultNormals(SoState *, SoNormalCache *nc);

  Binding findNormalBinding(SoState *state);
  Binding findMaterialBinding(SoState *state);

  int numLines() const;
  int numLineSegments() const;
};

#endif // !__SOINDEXEDLINESET_H__
