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

#ifndef COIN_SOENGINE_H
#define COIN_SOENGINE_H

#include <Inventor/fields/SoFieldContainer.h>

class SoEngineList;
class SoEngineOutput;
class SoEngineOutputList;
class SoEngineOutputData;

class COIN_DLL_API SoEngine : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  static void initClass(void);
  static void initClasses(void);
  static SoType getClassTypeId(void);

  void evaluateWrapper(void);

  virtual int getOutputs(SoEngineOutputList & l) const;
  SoEngineOutput * getOutput(const SbName & outputname) const;
  SbBool getOutputName(const SoEngineOutput * output, SbName & outputname) const;
  virtual const SoEngineOutputData * getOutputData(void) const = 0;
  static SoEngine * getByName(const SbName & name);
  static int getByName(const SbName & name, SoEngineList & el);

  SbBool isNotifying(void) const;
  virtual void notify(SoNotList * nl);

  SoEngine * copy(void) const;
  virtual SoFieldContainer * copyThroughConnection(void) const;
  SbBool shouldCopy(void) const;

  virtual void writeInstance(SoOutput * out);


protected:
  SoEngine(void);
  virtual ~SoEngine();
  virtual void evaluate(void) = 0;

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void inputChanged(SoField * which);

  static const SoFieldData ** getInputDataPtr(void);
  static const SoEngineOutputData ** getOutputDataPtr(void);

  void writeOutputTypes(SoOutput * out);

private:
  virtual void destroy(void); // overrides SoBase::destroy()

  static SoType classTypeId;
  unsigned int flags;
};

#if !defined(COIN_INTERNAL)
#include <Inventor/engines/SoEngineOutput.h>
#endif // COIN_INTERNAL

#endif // !COIN_SOENGINE_H
