#ifndef COIN_SOGLCACHECONTEXTELEMENT_H
#define COIN_SOGLCACHECONTEXTELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoSubElement.h>

// This shouldn't strictly be necessary, but the OSF1/cxx compiler
// complains if this is left out, while using the "friend class
// SoGLDisplayList" statement in the class definition.
class SoGLDisplayList;


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

private:
  friend class SoGLDisplayList;
  static void scheduleDelete(SoState * state, SoGLDisplayList * dl);

private:
  int context;
  SbBool twopass;
  SbBool remote;
  int autocachebits;
};


// FIXME: keep separate classes in separate header files for
// consistency and clean design -- so split the following into it's
// own SoGLDisplayList.h file (unless there are compelling reasons
// _not_ to do this that have escaped my attention).  20010912 mortene.

// implementation in Coin/src/elements/SoGLDisplayList.cpp
class COIN_DLL_API SoGLDisplayList {
  friend class SoGLCacheContextElement;
  ~SoGLDisplayList();
public:
  enum Type {
    DISPLAY_LIST,
    TEXTURE_OBJECT
  };
  SoGLDisplayList(SoState * state, Type type, int allocnum = 1,
                  SbBool mipmaptexobj = FALSE);
  void ref(void);
  void unref(SoState * state = NULL);

  void open(SoState *state, int index = 0);
  void close(SoState *state);

  void call(SoState * state, int index = 0);
  void addDependency(SoState * state);

  SbBool isMipMapTextureObject(void) const;
  Type getType(void) const;
  int getNumAllocated(void) const;
  // this returns GLuint in Inventor, but we try to avoid including
  // gl.h in the header files so we just return unsigned int.
  unsigned int getFirstIndex(void) const;
  int getContext(void) const;

private:
  Type type;
  int numalloc;
  unsigned int firstindex;
  int context;
  int refcount;
  SbBool mipmap;

  void bindTexture(SoState *state);
};

#endif // !COIN_SOGLCACHECONTEXTELEMENT_H
