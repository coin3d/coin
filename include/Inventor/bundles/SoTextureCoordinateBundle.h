/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
