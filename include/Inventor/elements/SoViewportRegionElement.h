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

#ifndef __SOVIEWPORTREGIONELEMENT_H__
#define __SOVIEWPORTREGIONELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT

#include <Inventor/elements/SoElement.h>
#include <Inventor/SbViewportRegion.h>

class SoViewportRegionElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoViewportRegionElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoViewportRegionElement(void);

  static void initClass(void);

protected:
  SoViewportRegionElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement *copyMatchInfo() const;

  static void set(SoState * const state,
                  const SbViewportRegion & viewportRegion);
  static const SbViewportRegion &get(SoState * const state);

  virtual void print(FILE * file) const;

protected:
  virtual void setElt(const SbViewportRegion & viewportRegion);
  SbViewportRegion viewportRegion;

private:
  static  SbViewportRegion *emptyViewportRegion;

};

#endif // !__SOVIEWPORTREGIONELEMENT_H__
