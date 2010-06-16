#ifndef COIN_SOGLMULTITEXTUREENABLEDELEMENT_H
#define COIN_SOGLMULTITEXTUREENABLEDELEMENT_H

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

#include <Inventor/elements/SoMultiTextureEnabledElement.h>

typedef struct cc_glglue cc_glglue;

class COIN_DLL_API SoGLMultiTextureEnabledElement : public SoMultiTextureEnabledElement {
  typedef SoMultiTextureEnabledElement inherited;

  SO_ELEMENT_HEADER(SoGLMultiTextureEnabledElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLMultiTextureEnabledElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);
  virtual void setElt(const int unit, const int mode);

private:
  void updategl(const int unit);
  void updategl(const int unit, const Mode oldvalue, const Mode newvalue);

  uint32_t cachecontext;
};

#endif // !COIN_SOGLMULTITEXTUREENABLEDELEMENT_H
