#ifndef COIN_SOGLAMBIENTCOLORELEMENT_H
#define COIN_SOGLAMBIENTCOLORELEMENT_H

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

#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/SbColor.h>

class COIN_DLL_API SoGLAmbientColorElement : public SoAmbientColorElement {
  typedef SoAmbientColorElement inherited;

  SO_ELEMENT_HEADER(SoGLAmbientColorElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLAmbientColorElement();

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index);

protected:
  virtual void setElt(const int32_t numColors, const SbColor * const colors);

private:
  void updategl();
  SbColor current;

};

#endif // !COIN_SOGLAMBIENTCOLORELEMENT_H
