#ifndef COIN_SOGLCOORDINATEELEMENT_H
#define COIN_SOGLCOORDINATEELEMENT_H

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

#include <Inventor/elements/SoCoordinateElement.h>

class COIN_DLL_API SoGLCoordinateElement : public SoCoordinateElement {
  typedef SoCoordinateElement inherited;

  SO_ELEMENT_HEADER(SoGLCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLCoordinateElement();

public:
  const SbVec3f *getPtr3() const;
  const SbVec4f *getPtr4() const;
  void send(const int index) const;

};

#endif // !COIN_SOGLCOORDINATEELEMENT_H
