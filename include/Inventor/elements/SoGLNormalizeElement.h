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

#ifndef __SOGLNORMALIZEELEMENT_H__
#define __SOGLNORMALIZEELEMENT_H__

#include <Inventor/elements/SoElement.h>

#if defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif /// defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)


class SoGLNormalizeElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoGLNormalizeElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLNormalizeElement(void);

  static void initClass(void);

protected:
  SoGLNormalizeElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo() const;

  static void setMatrixState(SoState * const state,
                             const SbBool valid);
  static void setUnitNormals(SoState * const state,
                             const SbBool unitNormals);

  void evaluate() const;
  void forceSend(const SbBool normalsAreUnitLength) const;

private:

  SbBool okMatrix;
  SbBool unitNormals;
  SbBool glnormalize;

  void updategl(const SbBool normalize);
};

#endif // !__SOGLNORMALIZEELEMENT_H__
