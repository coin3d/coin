#ifndef COIN_SOTEXTURECOMBINEELEMENT_H
#define COIN_SOTEXTURECOMBINEELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbColor4f.h>

class SoState;
class SoTextureCombineElementP;

class COIN_DLL_API SoTextureCombineElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoTextureCombineElement);
public:
  static void initClass(void);
protected:
  virtual ~SoTextureCombineElement();

public:

  enum Source {
    PRIMARY_COLOR = 0x8577,
    TEXTURE = 0x1702,
    CONSTANT = 0x8576,
    PREVIOUS =  0x8578
  };
  enum Operand {
    SRC_COLOR = 0x0300,
    ONE_MINUS_SRC_COLOR = 0x0301, 
    SRC_ALPHA = 0x0302,
    ONE_MINUS_SRC_ALPHA = 0x0303
  };
  enum Operation {
    REPLACE = 0x1E01,
    MODULATE = 0x2100,
    ADD = 0x0104,
    ADD_SIGNED = 0x8574,
    SUBTRACT = 0x84E7,
    INTERPOLATE = 0x8575,
    DOT3_RGB = 0x86AE,
    DOT3_RGBA = 0x86AF
  };

  virtual void init(SoState * state);

  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  const Operation rgboperation,
                  const Operation alphaoperation,
                  const Source * rgbsource,
                  const Source * alphasource,
                  const Operand * rgboperand,
                  const Operand * alphaoperand,
                  const SbColor4f & constantcolor,
                  const float rgbscale,
                  const float alphascale);

  static SbBool isDefault(SoState * const state,
                          const int unit);
                       
  static void get(SoState * const state,
                  const int unit,
                  Operation & rgboperation,
                  Operation & alphaoperation,
                  Source * rgbsource,
                  Source * alphasource,
                  Operand * rgboperand,
                  Operand * alphaoperand,
                  SbColor4f & constantcolor,
                  float & rgbscale,
                  float & alphascale);

  virtual void push(SoState * state);
  virtual SbBool matches(const SoElement * elem) const;
  SoElement * copyMatchInfo(void) const;

  virtual void setElt(const int unit,
                      const uint32_t nodeid,
                      const Operation rgboperation,
                      const Operation alphaoperation,
                      const Source * rgbsource,
                      const Source * alphasource,
                      const Operand * rgboperand,
                      const Operand * alphaoperand,
                      const SbColor4f & constantcolor,
                      const float rgbscale,
                      const float alphascale);

  static void apply(SoState * state, const int unit);

  class UnitData {
  public:
    uint32_t nodeid;
    Source rgbsource[3];
    Source alphasource[3];
    Operand rgboperand[3];
    Operand alphaoperand[3];
    Operation rgboperation;
    Operation alphaoperation;
    SbColor4f constantcolor;
    float rgbscale;
    float alphascale;
  };

protected:
  const UnitData & getUnitData(const int unit) const;

private:
  void setDefaultValues(const int unit);
  SoTextureCombineElementP * pimpl;
};

#endif // !COIN_SOTEXTURECOMBINEELEMENT_H
