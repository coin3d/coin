/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOCONCATENATE_H__
#define __SOCONCATENATE_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMField.h>

#if defined(COIN_EXCLUDE_SOCONCATENATE)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOCONCATENATE

class SoConcatenate : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_ABSTRACT_HEADER(SoConcatenate);

public:
  SoMField * input[10];

  SoEngineOutput * output;

  SoConcatenate(SoType inputType);

  static void initClass();

private:
  virtual ~SoConcatenate();
  virtual void evaluate();
};

#endif // !__SOCONCATENATE_H__
