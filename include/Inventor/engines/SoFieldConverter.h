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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOFIELDCONVERTER_H__
#define __SOFIELDCONVERTER_H__

#include <Inventor/engines/SoSubEngine.h>

class SoEngineOutput;


class SoFieldConverter : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_ABSTRACT_HEADER(SoFieldConverter);

public:

  SoFieldConverter();
  static void initClass();
  static void initConverters();
  virtual SoField * getInput(SoType type) = 0;
  virtual SoEngineOutput * getOutput(SoType type) = 0;
  virtual SoField * getConnectedInput();
  virtual void setConnectedInput(SoField * input);
protected:
  // FIXME: made this protected instead of private to avoid compilation
  // problem (on egcs 1.0.2). Is this correct? 19981102 mortene.
  // It looks correct; the methods are overloaded in subclasses (kintel 19990610)
protected:
  virtual ~SoFieldConverter();
  virtual void evaluate() = 0;
private:
  SoField * connectedInput;
};

#endif // !__SOFIELDCONVERTER_H__
