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

#ifndef __SODECIMATIONPERCENTAGEELEMENT_H__
#define __SODECIMATIONPERCENTAGEELEMENT_H__

#include <Inventor/elements/SoFloatElement.h>


class SoDecimationPercentageElement : public SoFloatElement {
  typedef SoFloatElement inherited;

  SO_ELEMENT_HEADER(SoDecimationPercentageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoDecimationPercentageElement();

public:
  virtual void init(SoState * state);
  static void set(SoState * const state, SoNode * const node,
                   const float complexity);
  static void set(SoState * const state, const float complexity);
  static float get(SoState * const state);
  static float getDefault(void);

};

#endif // !__SODECIMATIONPERCENTAGEELEMENT_H__
