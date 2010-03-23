#ifndef COIN_SOTEXTURECOORDINATEBUNDLE_H
#define COIN_SOTEXTURECOORDINATEBUNDLE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>

#include <Inventor/SbVec4f.h>

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
  const SoMultiTextureCoordinateElement *coordElt;
  const SoGLMultiTextureCoordinateElement *glElt;
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
  void initDefault(SoAction * action, const int unit);
};


#endif // !COIN_SOTEXTURECOORDINATEBUNDLE_H
