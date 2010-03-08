#ifndef COIN_SOTOVRML2ACTION_H
#define COIN_SOTOVRML2ACTION_H

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

#include <Inventor/actions/SoToVRMLAction.h>

class SoToVRML2ActionP;

class COIN_DLL_API SoToVRML2Action : public SoToVRMLAction {
  typedef SoToVRMLAction inherited;

  SO_ACTION_HEADER(SoToVRML2Action);

public:
  static void initClass(void);

  SoToVRML2Action(void);
  virtual ~SoToVRML2Action(void);

  virtual void apply(SoNode * node);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);
  
  class SoVRMLGroup * getVRML2SceneGraph(void) const;

  void reuseAppearanceNodes(SbBool appearance);
  SbBool doReuseAppearanceNodes(void) const;

  void reusePropertyNodes(SbBool property);
  SbBool doReusePropertyNodes(void) const;

  void reuseGeometryNodes(SbBool geometry);
  SbBool doReuseGeometryNodes(void) const;

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbPimplPtr<SoToVRML2ActionP> pimpl;
  friend class SoToVRML2ActionP;

  // NOT IMPLEMENTED:
  SoToVRML2Action(const SoToVRML2Action & rhs);
  SoToVRML2Action & operator = (const SoToVRML2Action & rhs);
}; // SoToVRMLAction

#endif // !COIN_SOTOVRML2ACTION_H
