#ifndef COIN_SOGLPOINTSIZEELEMENT_H
#define COIN_SOGLPOINTSIZEELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/SbVec2f.h>
#include <Inventor/elements/SoPointSizeElement.h>

class COIN_DLL_API SoGLPointSizeElement : public SoPointSizeElement {
  typedef SoPointSizeElement inherited;

  SO_ELEMENT_HEADER(SoGLPointSizeElement);

public:
  static void initClass(void);
protected:
  virtual ~SoGLPointSizeElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

protected:
  virtual void setElt(float size);

private:
  // FIXME: use native types, in this case an SbVec2f. Can't be fixed
  // without breaking ABI compatibility, though, so wait until Coin
  // version 2.  20010823 mortene.
  static float sizerange[2];
  void updategl();
};

#endif // !COIN_SOGLPOINTSIZEELEMENT_H
