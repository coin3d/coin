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

#ifndef __SOVIEWVOLUMEELEMENT_H__
#define __SOVIEWVOLUMEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOVIEWVOLUMEELEMENT

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/SbViewVolume.h>

class SoViewVolumeElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoViewVolumeElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoViewVolumeElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoViewVolumeElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
		  const SbViewVolume & viewVolume);
  static const SbViewVolume &get(SoState * const state);

protected:
  SbViewVolume viewVolume;

}; // class SoViewVolumeElement

#endif // !__SOVIEWVOLUMEELEMENT_H__
