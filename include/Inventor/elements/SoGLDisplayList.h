#ifndef COIN_SOGLDISPLAYLIST_H
#define COIN_SOGLDISPLAYLIST_H

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

#include <Inventor/SbBasic.h>

class SoState;

// *************************************************************************

class COIN_DLL_API SoGLDisplayList {
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
  ~SoGLDisplayList();

  Type type;
  int numalloc;
  unsigned int firstindex;
  int context;
  int refcount;
  SbBool mipmap;

  void bindTexture(SoState *state);

  friend class SoGLCacheContextElement;
};

#endif // !COIN_SOGLDISPLAYLIST_H
