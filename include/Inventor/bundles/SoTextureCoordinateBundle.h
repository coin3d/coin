#ifndef COIN_SOTEXTURECOORDINATEBUNDLE_H
#define COIN_SOTEXTURECOORDINATEBUNDLE_H

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

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/SbVec4f.h>

class SoTextureCoordinateElement;
class SoGLTextureCoordinateElement;
class SoTextureCoordinateCache;
class SoShape;


class COIN_DLL_API SoTextureCoordinateBundle : public SoBundle {
  typedef SoBundle inherited;
public:
  SoTextureCoordinateBundle(SoAction * const action,
                            const SbBool forRendering,
                            const SbBool setUpDefault = TRUE);
  ~SoTextureCoordinateBundle();

  SbBool needCoordinates(void) const;
  SbBool isFunction(void) const;

  const SbVec4f &get(const int index);
  const SbVec4f &get(const SbVec3f &point, const SbVec3f &normal);

  void send(const int index) const {
    glElt->send(index);
  }
  void send(const int index, const SbVec3f &point,
            const SbVec3f &normal) const {
    glElt->send(index, point, normal);
  }

  SbBool needIndices(void) const;

private:
  const SoTextureCoordinateElement *coordElt;
  const SoGLTextureCoordinateElement *glElt;
  unsigned int flags;

  // misc stuff for default texture coordinate mappping
  static const SbVec4f & defaultCB(void * userdata,
                                   const SbVec3f & point,
                                   const SbVec3f & normal);
  static const SbVec4f & defaultCBMulti(void * userdata,
                                        const SbVec3f & point,
                                        const SbVec3f & normal);
  SoShape * shapenode;
  SbVec3f defaultorigo;
  SbVec3f defaultsize;
  SbVec4f dummyInstance;
  int defaultdim0, defaultdim1;
  void initDefaultCallback(SoAction * action);
  void initDefault(SoAction * const action, const SbBool forRendering);
  void initDefaultMulti(SoAction * action, const int unit);
};


#endif // !COIN_SOTEXTURECOORDINATEBUNDLE_H
