#ifndef COIN_SOGLCACHECONTEXTELEMENT_H
#define COIN_SOGLCACHECONTEXTELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/elements/SoSubElement.h>

// This shouldn't strictly be necessary, but the OSF1/cxx compiler
// complains if this is left out, while using the "friend class
// SoGLDisplayList" statement in the class definition.
class SoGLDisplayList;

typedef void SoScheduleDeleteCB(void * closure, uint32_t contextid);

// *************************************************************************

class COIN_DLL_API SoGLCacheContextElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLCacheContextElement);

public:
  static void initClass(void);
protected:
  virtual ~SoGLCacheContextElement();

public:
  virtual void init(SoState * state);

  virtual SbBool matches(const SoElement * elt) const;
  virtual SoElement * copyMatchInfo(void) const;
  static void set(SoState * state, int context,
                  SbBool twopasstransparency,
                  SbBool remoterendering);
  static int get(SoState * state);
  static int getExtID(const char * str);
  static void getOpenGLVersion(SoState * state, int & major, int & minor);
  static SbBool extSupported(SoState * state, int extid);
  static SbBool areMipMapsFast(SoState * state);
  enum {
    DO_AUTO_CACHE = 1,
    DONT_AUTO_CACHE
  };
  static void shouldAutoCache(SoState * state, int bits);
  static void setAutoCacheBits(SoState * state, int bits);
  static int resetAutoCacheBits(SoState * state);
  static SbBool getIsRemoteRendering(SoState * state);

  static uint32_t getUniqueCacheContext(void);

  static void scheduleDeleteCallback(const uint32_t contextid,
                                     SoScheduleDeleteCB * cb,
                                     void * closure);

private:
  friend class SoGLDisplayList;
  static void scheduleDelete(SoState * state, SoGLDisplayList * dl);
  static void cleanupContext(uint32_t contextid, void * closure);

private:
  int context;
  SbBool twopass;
  int autocachebits;

  enum { RENDERING_UNSET, RENDERING_SET_DIRECT, RENDERING_SET_INDIRECT };
  int rendering;
  SbBool isDirectRendering(SoState * state) const;
};

// *************************************************************************

// For compatibility with client code originally written with SGI/TGS
// Inventor:
#ifndef COIN_INTERNAL
#include <Inventor/elements/SoGLDisplayList.h>
#endif // ! COIN_INTERNAL

#endif // !COIN_SOGLCACHECONTEXTELEMENT_H
