#ifndef COIN_SOMULTITEXTUREENABLEDELEMENT_H
#define COIN_SOMULTITEXTUREENABLEDELEMENT_H

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

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>

class SoMultiTextureEnabledElementP;

class COIN_DLL_API SoMultiTextureEnabledElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoMultiTextureEnabledElement);
public:
  static void initClass(void);
protected:
  virtual ~SoMultiTextureEnabledElement();

public:
  enum Mode {
    DISABLED = 0,
    TEXTURE2D,
    RECTANGLE,
    CUBEMAP,
    TEXTURE3D
  };

  virtual void init(SoState * state);
  static void set(SoState * state, 
                  SoNode * node,
                  const int unit,
                  const SbBool enabled);

  static SbBool get(SoState * state, const int unit = 0);
  virtual void setElt(const int unit, const int mode);

  virtual void push(SoState * state);
  virtual SbBool matches(const SoElement * elem) const;
  SoElement * copyMatchInfo(void) const;

  static const SbBool * getEnabledUnits(SoState * state,
                                        int & lastenabled);
  static const Mode * getActiveUnits(SoState * state,
                                     int & lastenabled);

  static void enableRectangle(SoState * state, SoNode * node, const int unit = 0);
  static void enableCubeMap(SoState * state, SoNode * node, const int unit = 0);
  static void enableTexture3(SoState * state, SoNode * node, const int unit = 0);
  static Mode getMode(SoState * state, const int unit = 0);
  static void disableAll(SoState * state);

  // Coin-3 support
  static void set(SoState * state, 
                  const SbBool enabled) {
    set(state, NULL, 0, enabled);
  }

  static void set(SoState * state, SoNode * node,
                  const SbBool enabled) {
    set(state, node, 0, enabled);
  }
  
protected:
  int getMaxUnits() const;
  SbBool isEnabled(const int unit) const;
  Mode getMode(const int unit) const;

private:
  SoMultiTextureEnabledElementP * pimpl;
};

#endif // !COIN_SOMULTITEXTUREENABLEDELEMENT_H
