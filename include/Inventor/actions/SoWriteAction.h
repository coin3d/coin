/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOWRITEACTION_H
#define COIN_SOWRITEACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

class SoVRMLPROTODef;
class SoVRMLEXTERNPROTODef;


class COIN_DLL_EXPORT SoWriteAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoWriteAction);

public:
  SoWriteAction(void);
  SoWriteAction(SoOutput * out);
  virtual ~SoWriteAction();

  static void initClass(void);

  SoOutput * getOutput(void) const;

  void continueToApply(SoNode * node);
  void continueToApply(SoPath * path);

protected:
  virtual void beginTraversal(SoNode * node);
  virtual SbBool shouldCompactPathLists(void) const;

private:
  void commonConstructor(SoOutput * out);

  SoOutput * outobj;
  SbBool localoutputalloc;
};

#endif // !COIN_SOWRITEACTION_H
