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

#ifndef COIN_SOLINEHIGHLIGHTRENDERACTION_H
#define COIN_SOLINEHIGHLIGHTRENDERACTION_H

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>

class SoTempPath;
class SoPathList;


class SoLineHighlightRenderAction : public SoGLRenderAction {
  typedef SoGLRenderAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoLineHighlightRenderAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoLineHighlightRenderAction();
//$ END TEMPLATE ActionHeader

public:
  SoLineHighlightRenderAction(void);
  SoLineHighlightRenderAction(const SbViewportRegion & viewportRegion);

  virtual void apply(SoNode *node);
  void setVisible(const SbBool visible);
  SbBool isVisible() const;
  void setColor(const SbColor &color);
  const SbColor &getColor();
  void setLinePattern(unsigned short pattern);
  unsigned short getLinePattern() const;
  void setLineWidth(const float width);
  float getLineWidth() const;

protected:

  SbBool hlVisible;
  // Some protected members are missing compared to OIV here.
  // I doubt that anyone will use them though, since it looked
  // like they should have been private. We chose to implement this 
  // action in a different manner. But, if you need the protected members
  // provided by OIV here, contact us any we'll consider reimplementing
  // the action to conform more to OIV. pederb, 20000222

private:
  class SoSearchAction *searchAction;
  void init();
  void drawBoxes(SoPath *pathtothis, const SoPathList *pathlist);
  SbColor color;
  unsigned short linepattern;
  float linewidth;
};

#endif // !COIN_SOLINEHIGHLIGHTRENDERACTION_H
