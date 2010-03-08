#ifndef COIN_SOBOXHIGHLIGHTRENDERACTION_H
#define COIN_SOBOXHIGHLIGHTRENDERACTION_H

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

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>

class SoBoxHighlightRenderActionP;

class COIN_DLL_API SoBoxHighlightRenderAction : public SoGLRenderAction {
  typedef SoGLRenderAction inherited;

  SO_ACTION_HEADER(SoBoxHighlightRenderAction);

public:
  static void initClass(void);

  SoBoxHighlightRenderAction(void);
  SoBoxHighlightRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoBoxHighlightRenderAction(void);

  virtual void apply(SoNode * node);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);
  void setVisible(const SbBool visible);
  SbBool isVisible(void) const;
  void setColor(const SbColor & color);
  const SbColor & getColor(void);
  void setLinePattern(unsigned short pattern);
  unsigned short getLinePattern(void) const;
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
  void init(void);
  void drawBoxes(SoPath * pathtothis, const SoPathList * pathlist);

private:
  SbPimplPtr<SoBoxHighlightRenderActionP> pimpl;

  // NOT IMPLEMENTED:
  SoBoxHighlightRenderAction(const SoBoxHighlightRenderAction & rhs);
  SoBoxHighlightRenderAction & operator = (const SoBoxHighlightRenderAction & rhs);
}; // SoBoxHighlightRenderAction

#endif // !COIN_SOBOXHIGHLIGHTRENDERACTION_H
