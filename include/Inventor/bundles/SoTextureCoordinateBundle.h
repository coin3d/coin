/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOTEXTURECOORDINATEBUNDLE_H
#define COIN_SOTEXTURECOORDINATEBUNDLE_H

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec2f.h>

class SoTextureCoordinateElement;
class SoGLTextureCoordinateElement;
class SoTextureCoordinateCache;
class SoVertexShape;

class COIN_DLL_EXPORT SoTextureCoordinateBundle : public SoBundle {
  typedef SoBundle inherited;
public:
  SoTextureCoordinateBundle(SoAction * const action,
                            const SbBool forRendering,
                            const SbBool setUpDefault = TRUE);
  ~SoTextureCoordinateBundle();

  SbBool needCoordinates() const;
  SbBool isFunction() const;

  const SbVec4f &get(const int index);
  const SbVec4f &get(const SbVec3f &point, const SbVec3f &normal);

  void send(const int index) const {
    glElt->send(index);
  }
  void send(const int index, const SbVec3f &point,
            const SbVec3f &normal) const {
    glElt->send(index, point, normal);
  }

private:
  const SoTextureCoordinateElement *coordElt;
  const SoGLTextureCoordinateElement *glElt;
  unsigned int flags;

  // misc stuff for default texture coordinate mappping
  static const SbVec4f &defaultCB(void * userdata,
                                  const SbVec3f & point,
                                  const SbVec3f & normal);
  SoVertexShape *shapenode;
  SbVec2f defaultorigo;
  SbVec2f defaultsize;
  SbVec4f dummyInstance;
  int defaultdim0, defaultdim1;
  void initDefault(SoAction * const action, const SbBool forRendering);
};

#endif // !COIN_SOTEXTURECOORDINATEBUNDLE_H
