/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOBOXHIGHLIGHTRENDERACTION_H
#define COIN_SOBOXHIGHLIGHTRENDERACTION_H

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>

class SoPathList;


class SoBoxHighlightRenderAction : public SoGLRenderAction {
  typedef SoGLRenderAction inherited;

  SO_ACTION_HEADER(SoBoxHighlightRenderAction);

public:
  SoBoxHighlightRenderAction(void);
  SoBoxHighlightRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoBoxHighlightRenderAction();

  static void initClass(void);

  virtual void apply(SoNode * node);
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
  // action in a differnet manner. But, if you need the protected members
  // provided by OIV here, contact us any we'll consider reimplementing
  // the action to conform more to OIV. pederb, 20000222

private:
  class SoSearchAction * searchaction;
  void init(void);
  void drawBoxes(SoPath * pathtothis, const SoPathList * pathlist);

  SbColor color;
  unsigned short linepattern;
  float linewidth;
  SoTempPath * postprocpath;
};

#endif // !COIN_SOBOXHIGHLIGHTRENDERACTION_H
