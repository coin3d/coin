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

#ifndef __SOTRIANGLESTRIPSET_H__
#define __SOTRIANGLESTRIPSET_H__

#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/fields/SoMFInt32.h>

#if defined(COIN_EXCLUDE_SOTRIANGLESTRIPSET)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOTRIANGLESTRIPSET

// *************************************************************************

class SoTriangleStripSet : public SoNonIndexedShape {
  typedef SoNonIndexedShape inherited;

//$ BEGIN TEMPLATE NodeHeader( SoTriangleStripSet )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoTriangleStripSet(void);
protected:
  virtual ~SoTriangleStripSet();
//$ END TEMPLATE NodeHeader

public:
  enum Binding {
    OVERALL = 0,
    PER_STRIP,
    PER_FACE,
    PER_VERTEX
  };

  SoMFInt32 numVertices;

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

  virtual SbBool generateDefaultNormals(SoState * state, SoNormalBundle * nb);

protected:
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void generatePrimitives(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
					  const SoPrimitiveVertex * v1,
					  const SoPrimitiveVertex * v2,
					  const SoPrimitiveVertex * v3,
					  SoPickedPoint * pp);
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual SbBool generateDefaultNormals(SoState *, SoNormalCache *);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

private:
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  Binding findMaterialBinding(SoState * const state) const;
  Binding findNormalBinding(SoState * const state) const;
#endif

};

#endif // !__SOTRIANGLESTRIPSET_H__
