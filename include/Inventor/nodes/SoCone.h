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

#ifndef __SOCONE_H__
#define __SOCONE_H__

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

#if defined(COIN_EXCLUDE_SOCONE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOCONE

// *************************************************************************

class SoCone : public SoShape {
  typedef SoShape inherited;

//$ BEGIN TEMPLATE NodeHeader(SoCone)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoCone(void);
protected:
  virtual ~SoCone();
//$ END TEMPLATE NodeHeader

public:
  enum Part {
    SIDES = 1,
    BOTTOM,
    ALL
  };

  SoSFBitMask parts;
  SoSFFloat bottomRadius;
  SoSFFloat height;

  void addPart(SoCone::Part part);
  void removePart(SoCone::Part part);
  SbBool hasPart(SoCone::Part part) const;

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willSetShapeHints() const;
  virtual SbBool willSetShadeModel() const;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual void rayPick(SoRayPickAction *action);
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction *action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void generatePrimitives(SoAction *action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
};

#endif // !__SOCONE_H__
