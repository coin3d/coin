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

#ifndef __SOCUBE_H__
#define __SOCUBE_H__

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

#if defined(COIN_EXCLUDE_SOCUBE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOCUBE

class SoIndexedFaceSet;

// *************************************************************************

class SoCube : public SoShape {
    typedef SoShape inherited;

//$ BEGIN TEMPLATE NodeHeader( SoCube )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoCube(void);
protected:
  virtual ~SoCube();
//$ END TEMPLATE NodeHeader

public:
  SoSFFloat width;
  SoSFFloat height;
  SoSFFloat depth;

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willSetShadeModel() const;
  virtual SbBool willSetShapeHints() const;
  virtual SbBool willSendUnitLengthNormals(SoState *state) const;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual void rayPick(SoRayPickAction * action);
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void generatePrimitives(SoAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
					  const SoPrimitiveVertex * v1,
					  const SoPrimitiveVertex * v2,
					  const SoPrimitiveVertex * v3,
					  SoPickedPoint * pp);
#endif // !COIN_EXCLUDE_SORAYPICKACTION

private:
  void getHalfSize(float & w, float & h, float & d);
  void generateVertices(SbVec3f * const varray);
};

#endif // !__SOCUBE_H__
