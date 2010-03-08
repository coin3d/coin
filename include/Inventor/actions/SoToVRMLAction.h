#ifndef COIN_SOTOVRMLACTION_H
#define COIN_SOTOVRMLACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

class SoToVRMLActionP;

class COIN_DLL_API SoToVRMLAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoToVRMLAction);

public:
  static void initClass(void);

  SoToVRMLAction(void);
  virtual ~SoToVRMLAction(void);

  virtual void apply(SoNode * node);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);
  
  SoNode * getVRMLSceneGraph(void) const;
  
  void expandSoFile(SbBool flag);
  SbBool areSoFileExpanded(void) const;
  
  void setUrlName(const SbString name);
  SbString getUrlName(void) const;
  
  void writeTexCoords(SbBool flag);
  SbBool areTexCoordWritten(void) const;
  
  void expandTexture2Node(SbBool flag);
  SbBool areTexture2NodeExpanded(void) const;
  
  void keepUnknownNodes(SbBool flag);
  SbBool areUnknownNodeKept(void) const;
  
  void convertInlineNodes(SbBool flag);
  SbBool doConvertInlineNodes(void) const;
  
  void conditionalConversion(SbBool flag);
  SbBool doConditionalConversion(void) const;
  
  void setVerbosity(SbBool flag);
  SbBool isVerbose(void) const;

protected:
  virtual void beginTraversal(SoNode * node);

private:
  SbPimplPtr<SoToVRMLActionP> pimpl;
  friend class SoToVRMLActionP;

  // NOT IMPLEMENTED:
  SoToVRMLAction(const SoToVRMLAction & rhs);
  SoToVRMLAction & operator = (const SoToVRMLAction & rhs);
}; // SoToVRMLAction

#endif // !COIN_SOTOVRMLACTION_H
