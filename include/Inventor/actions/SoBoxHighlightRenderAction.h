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

#ifndef __SOBOXHIGHLIGHTRENDERACTION_H__
#define __SOBOXHIGHLIGHTRENDERACTION_H__

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>


class SoBoxHighlightRenderAction : public SoGLRenderAction {
  typedef SoGLRenderAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoBoxHighlightRenderAction)
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

  virtual ~SoBoxHighlightRenderAction();
//$ END TEMPLATE ActionHeader

public:
  SoBoxHighlightRenderAction(void);
  SoBoxHighlightRenderAction(const SbViewportRegion & viewportRegion);

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
  // for some reason all these are protected under OIV.I've kept
  // them protected, using the same names even though I think
  // it would be better to have them under private. pederb, 1999-11-24

  void updateBBox(SoPath *path);
  class SoSeparator *localRoot;
  class SoLightModel *lightModel;
  class SoBaseColor *baseColor;
  class SoDrawStyle *drawStyle;
  class SoTexture2 *texture;
  class SoTranslation *xlate;
  class SoMatrixTransform *xform;
  class SoCube *cube;

  SbBool hlVisible;
  SoPath *selPath;

private:
  class SoSearchAction *searchAction;
  class SoGetBoundingBoxAction *bboxAction;
  void init();
};

#endif // !__SOBOXHIGHLIGHTRENDERACTION_H__
