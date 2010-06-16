#ifndef COIN_SOREORGANIZEACTION_H
#define COIN_SOREORGANIZEACTION_H

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

#include <Inventor/actions/SoSimplifyAction.h>

class SoSimplifier;
class SoSeparator;
class SoReorganizeActionP;

class COIN_DLL_API SoReorganizeAction : public SoSimplifyAction {
  typedef SoSimplifyAction inherited;

  SO_ACTION_HEADER(SoReorganizeAction);

public: 
  static void initClass(void);

  SoReorganizeAction(SoSimplifier * simplifier = NULL);
  virtual ~SoReorganizeAction(void);

  SoSeparator * getSimplifiedSceneGraph(void) const;
  void generateNormals(SbBool onoff);
  SbBool areNormalGenerated(void) const;
  void generateTriangleStrips(SbBool onoff);
  SbBool areTriangleStripGenerated(void) const;
  void generateTexCoords(SbBool onoff);
  SbBool areTexCoordsGenerated(void) const;
  void generateVPNodes(SbBool onoff); 
  SbBool areVPNodesGenerated(void); 
  void matchIndexArrays(SbBool onoff);
  SbBool areIndexArraysMatched(void) const;
  SoSimplifier * getSimplifier(void) const;

  virtual void apply(SoNode * root);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);

  static void startReport(const char * msg);
  static void finishReport(void);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbPimplPtr<SoReorganizeActionP> pimpl;

  // NOT IMPLEMENTED:
  SoReorganizeAction(const SoReorganizeAction & rhs);
  SoReorganizeAction & operator = (const SoReorganizeAction & rhs);
}; // SoReorganizeAction

#endif // !COIN_SOREORGANIZEACTION_H
