#ifndef COIN_SOLINEHIGHLIGHTRENDERACTION_H
#define COIN_SOLINEHIGHLIGHTRENDERACTION_H

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

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>

class COIN_DLL_API SoLineHighlightRenderAction : public SoGLRenderAction {
  typedef SoGLRenderAction inherited;

  SO_ACTION_HEADER(SoLineHighlightRenderAction);

public:
  SoLineHighlightRenderAction(void);
  SoLineHighlightRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoLineHighlightRenderAction();

  static void initClass(void);

  virtual void apply(SoNode * node);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);
  void setVisible(const SbBool visible);
  SbBool isVisible(void) const;
  void setColor(const SbColor & color);
  const SbColor & getColor(void);
  void setLinePattern(uint16_t pattern);
  uint16_t getLinePattern(void) const;
  void setLineWidth(const float width);
  float getLineWidth(void) const;

protected:
  SbBool hlVisible;
  // Some protected members are missing compared to OIV here.
  // I doubt that anyone will use them though, since it looked
  // like they should have been private. We chose to implement this
  // action in a different manner. But, if you need the protected members
  // provided by OIV here, contact us any we'll consider reimplementing
  // the action to conform more to OIV. pederb, 20000222

private:
  class SoLineHighlightRenderActionP * pimpl;
  friend class SoLineHighlightRenderActionP;
};

#endif // !COIN_SOLINEHIGHLIGHTRENDERACTION_H
