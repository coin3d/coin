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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOLINESET_H__
#define __SOLINESET_H__

#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/fields/SoMFInt32.h>

#if defined(COIN_EXCLUDE_SOLINESET)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOLINESET

// *************************************************************************

class SoLineSet : public SoNonIndexedShape {
  typedef SoNonIndexedShape inherited;

//$ BEGIN TEMPLATE NodeHeader( SoLineSet )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoLineSet(void);
protected:
  virtual ~SoLineSet();
//$ END TEMPLATE NodeHeader

public:
  enum Binding {
    OVERALL = 0,
    PER_LINE,
    PER_SEGMENT,
    PER_VERTEX
  };

public:
  SoMFInt32 numVertices;

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual SoDetail * createLineSegmentDetail(SoRayPickAction * action,
					     const SoPrimitiveVertex * v1,
					     const SoPrimitiveVertex * v2,
					     SoPickedPoint * pp);
#endif // !COIN_EXCLUDE_SORAYPICKACTION

private:
  SbBool generateDefaultNormals(SoState *, SoNormalCache * nc);

  Binding findNormalBinding(SoState * const state) const;
  Binding findMaterialBinding(SoState * const state) const;
};

#endif // !__SOLINESET_H__
