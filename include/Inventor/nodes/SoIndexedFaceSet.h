/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOINDEXEDFACESET_H__
#define __SOINDEXEDFACESET_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoIndexedShape.h>

#if defined(COIN_EXCLUDE_SOINDEXEDFACESET)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOINDEXEDFACESET

#define SO_END_FACE_INDEX (-1)

// *************************************************************************

class SoConvexDataCache;

class SoIndexedFaceSet : public SoIndexedShape {
  typedef SoIndexedShape inherited;

  SO_NODE_HEADER(SoIndexedFaceSet);

public:
  static void initClass(void);
  SoIndexedFaceSet(void);

  enum Binding {
    OVERALL = 0, 
    PER_FACE, 
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED,
    NONE = OVERALL, 
  };

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

  SbBool generateDefaultNormals(SoState *state, SoNormalBundle *nb);

protected:
  virtual ~SoIndexedFaceSet();

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void generatePrimitives(SoAction *action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual SoDetail *createTriangleDetail(SoRayPickAction *action,
					 const SoPrimitiveVertex *v1,
					 const SoPrimitiveVertex *v2,
					 const SoPrimitiveVertex *v3,
					 SoPickedPoint *pp);
#endif // !COIN_EXCLUDE_SORAYPICKACTION


private:
  int findNumFaces() const;
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
  virtual void notify(SoNotList *list);

  SbBool countPrimitives();

  int numTriangles;
  int numQuads;
  int numPolygons;
  
  SoConvexDataCache *convexCache;

};

#endif // !__SOINDEXEDFACESET_H__
