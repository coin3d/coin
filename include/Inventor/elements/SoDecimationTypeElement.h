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

#ifndef COIN_SODECIMATIONTYPEELEMENT_H
#define COIN_SODECIMATIONTYPEELEMENT_H

#include <Inventor/elements/SoInt32Element.h>


class COIN_DLL_API SoDecimationTypeElement : public SoInt32Element {
  typedef SoInt32Element inherited;

  SO_ELEMENT_HEADER(SoDecimationTypeElement);
public:
  static void initClass(void);
protected:
  virtual ~SoDecimationTypeElement();

public:
  enum Type { AUTOMATIC, HIGHEST, LOWEST, PERCENTAGE };

  virtual void init(SoState * state);

  static  void set(SoState * const state, const Type type);
  static  void set(SoState * const state, SoNode * const node, const Type type);

  static Type get(SoState * const state);
  static Type getDefault(void);

};

#endif // !COIN_SODECIMATIONTYPEELEMENT_H
