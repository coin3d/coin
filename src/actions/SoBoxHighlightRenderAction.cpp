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

/*!
  \class SoBoxHighlightRenderAction SoBoxHighlightRenderAction.h Inventor/actions/SoBoxHighlightRenderAction.h
  \brief The SoBoxHighlightRenderAction class renders the scene with highlighted boxes around selections.
  \ingroup actions

  This action performs the same tasks as its parent class,
  SoGLRenderAction, with the added ability to render highlighted
  bounding boxes around geometry in selected nodes. This is a simple
  but convenient way of giving feedback to the user upo interaction
  with the scene graph.

  To have the highlighting actually happen (and to be able to
  automatically "select" nodes by picking with the mouse cursor), you
  need to use SoSelection nodes in place of group nodes.

  \sa SoLineHighlightRenderAction, SoSelection
*/

#include <Inventor/actions/SoBoxHighlightRenderAction.h>

#include <Inventor/SbName.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/elements/SoLineWidthElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSelection.h>
#include <assert.h>

/*!
  \var SoBoxHighlightRenderAction::hlVisible

  Boolean which decides whether or not the highlights for selected
  nodes should be visible.
 */

#ifndef DOXYGEN_SKIP_THIS

class SoBoxHighlightRenderActionP {
public:
  SoSearchAction * searchaction;
  SbColor color;
  unsigned short linepattern;
  float linewidth;
  SoTempPath * postprocpath;
};

#endif // DOXYGEN_SKIP_THIS

SO_ACTION_SOURCE(SoBoxHighlightRenderAction);

#undef PRIVATE
#define PRIVATE(p) ((p)->pimpl)

// Overridden from parent class.
void
SoBoxHighlightRenderAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoBoxHighlightRenderAction, SoGLRenderAction);
}


/*!
  Default constructor. Note: passes a default SbViewportRegion to the
  parent constructor.
 */
SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(void)
  : inherited(SbViewportRegion())
{
  this->init();
}

/*!
  Constructor, taking an explicit \a viewportregion to render.
*/
SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(const SbViewportRegion & viewportregion)
  : inherited(viewportregion)
{
  this->init();
}

//
// private. called by both constructors
//
void
SoBoxHighlightRenderAction::init(void)
{
  SO_ACTION_CONSTRUCTOR(SoBoxHighlightRenderAction);

  PRIVATE(this) = new SoBoxHighlightRenderActionP;

  this->hlVisible = TRUE;
  PRIVATE(this)->color = SbColor(1.0f, 0.0f, 0.0f);
  PRIVATE(this)->linepattern = 0xffff;
  PRIVATE(this)->linewidth = 3.0f;
  PRIVATE(this)->searchaction = NULL;

  // SoBase-derived objects should be dynamically allocated.
  PRIVATE(this)->postprocpath = new SoTempPath(32);
  PRIVATE(this)->postprocpath->ref();
}


/*!
  Destructor.
*/
SoBoxHighlightRenderAction::~SoBoxHighlightRenderAction(void)
{
  PRIVATE(this)->postprocpath->unref();
  delete PRIVATE(this)->searchaction;
  delete PRIVATE(this);
}

// Documented in superclass. Overridden to add highlighting after the
// "ordinary" rendering.
void
SoBoxHighlightRenderAction::apply(SoNode * node)
{
  SoGLRenderAction::apply(node);
  if (this->hlVisible) {
    if (PRIVATE(this)->searchaction == NULL) {
      PRIVATE(this)->searchaction = new SoSearchAction;
      PRIVATE(this)->searchaction->setType(SoSelection::getClassTypeId());
      PRIVATE(this)->searchaction->setInterest(SoSearchAction::FIRST);
    }
    PRIVATE(this)->searchaction->apply(node);
    if (PRIVATE(this)->searchaction->isFound()) {
      SoSelection * selection =
        (SoSelection *)PRIVATE(this)->searchaction->getPath()->getTail();
      assert(selection->getTypeId().isDerivedFrom(SoSelection::getClassTypeId()));

      if (selection->getNumSelected()) {
        this->drawBoxes(PRIVATE(this)->searchaction->getPath(), selection->getList());
      }
    }
  }
}

// Documented in superclass. This method will just call the
// SoGLRenderAction::apply() method (so no highlighting will be done).
// 
// It has been overridden to avoid confusing the compiler, which
// typically want to see either all or none of the apply() methods
// overridden.
void
SoBoxHighlightRenderAction::apply(SoPath * path)
{
  SoGLRenderAction::apply(path);
}

// Documented in superclass.  This method will just call the
// SoGLRenderAction::apply() method (so no highlighting will be done).
//
// It has been overridden to avoid confusing the compiler, which
// typically want to see either all or none of the apply() methods
// overridden.
void
SoBoxHighlightRenderAction::apply(const SoPathList & pathlist,
                                  SbBool obeysrules)
{
  SoGLRenderAction::apply(pathlist, obeysrules);
}

/*!
  Sets if highlighted boxes should be \a visible when
  rendering. Defaults to \c TRUE.
*/
void
SoBoxHighlightRenderAction::setVisible(const SbBool visible)
{
  this->hlVisible = visible;
}

/*!
  Return if highlighted boxes are to be visible.
*/
SbBool
SoBoxHighlightRenderAction::isVisible(void) const
{
  return this->hlVisible;
}

/*!
  Sets the \a color for the highlighted boxes. Defaults to completely
  red.
*/
void
SoBoxHighlightRenderAction::setColor(const SbColor & color)
{
  PRIVATE(this)->color = color;
}

/*!
  Returns rendering color of the highlighted boxes.
*/
const SbColor &
SoBoxHighlightRenderAction::getColor(void)
{
  return PRIVATE(this)->color;
}

/*!
  Sets the line \a pattern used for the highlighted boxes. Defaults to
  \c 0xffff (i.e. drawn with no stipples).
*/
void
SoBoxHighlightRenderAction::setLinePattern(unsigned short pattern)
{
  PRIVATE(this)->linepattern = pattern;
}

/*!
  Returns line pattern used when drawing boxes.
*/
unsigned short
SoBoxHighlightRenderAction::getLinePattern(void) const
{
  return PRIVATE(this)->linepattern;
}

/*!
  Sets the line \a width used when drawing boxes, in screen pixels (as
  for all OpenGL rendering). Defaults to 3.
*/
void
SoBoxHighlightRenderAction::setLineWidth(const float width)
{
  PRIVATE(this)->linewidth = width;
}

/*!
  Returns the line width used when drawing highlight boxes.
*/
float
SoBoxHighlightRenderAction::getLineWidth(void) const
{
  return PRIVATE(this)->linewidth;
}

void
SoBoxHighlightRenderAction::drawBoxes(SoPath * pathtothis, const SoPathList * pathlist)
{
  int i;
  int thispos = ((SoFullPath *)pathtothis)->getLength()-1;
  assert(thispos >= 0);
  PRIVATE(this)->postprocpath->truncate(0); // reset
  
  for (i = 0; i < thispos; i++)
    PRIVATE(this)->postprocpath->append(pathtothis->getNode(i));
  
  SoState * state = this->getState();
  state->push();

  SoLightModelElement::set(state, SoLightModelElement::BASE_COLOR);
  SoDiffuseColorElement::set(state, NULL, 1, &PRIVATE(this)->color);
  SoLineWidthElement::set(state, PRIVATE(this)->linewidth);
  SoLinePatternElement::set(state, PRIVATE(this)->linepattern);
  SoTextureQualityElement::set(state, 0.0f);
  SoComplexityTypeElement::set(state, SoComplexityTypeElement::BOUNDING_BOX);
  SoDrawStyleElement::set(state, SoDrawStyleElement::LINES);
  SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
  SoOverrideElement::setDiffuseColorOverride(state, NULL, TRUE);
  SoOverrideElement::setLineWidthOverride(state, NULL, TRUE);
  SoOverrideElement::setLinePatternOverride(state, NULL, TRUE);
  SoOverrideElement::setComplexityTypeOverride(state, NULL, TRUE);
  SoOverrideElement::setDrawStyleOverride(state, NULL, TRUE);
  SoTextureOverrideElement::setQualityOverride(state, TRUE);

  for (i = 0; i < pathlist->getLength(); i++) {
    SoFullPath * path = (SoFullPath *)(*pathlist)[i];

    for (int j = 0; j < path->getLength(); j++) {
      PRIVATE(this)->postprocpath->append(path->getNode(j));
    }

    SoGLRenderAction::apply(PRIVATE(this)->postprocpath);
    PRIVATE(this)->postprocpath->truncate(thispos);
  }
  state->pop();
}

#undef PRIVATE

