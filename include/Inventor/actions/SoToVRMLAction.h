#ifndef COIN_SOTOVRMLACTION_H
#define COIN_SOTOVRMLACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>

// FIXME: the API does not match the one in TGS Inventor, which it
// should. 20020705 mortene.

class COIN_DLL_API SoToVRMLAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoToVRMLAction);

public:
  SoToVRMLAction(void);
  virtual ~SoToVRMLAction();

  static void initClass(void);

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
  class SoToVRMLActionP * pimpl;
  friend class SoToVRMLActionP;
};

#endif // !COIN_SOTOVRMLACTION_H
