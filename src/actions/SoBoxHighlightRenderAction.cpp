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


SO_ACTION_SOURCE(SoBoxHighlightRenderAction);


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

  SoBoxHighlightRenderAction::methods->setUp(); // initialize action methods

  this->hlVisible = TRUE;
  this->color = SbColor(1.0f, 0.0f, 0.0f);
  this->linepattern = 0xffff;
  this->linewidth = 3.0f;
  this->searchaction = NULL;

  // SoBase-derived objects should be dynamically allocated.
  this->postprocpath = new SoTempPath(32);
  this->postprocpath->ref();
}


/*!
  Destructor.
*/
SoBoxHighlightRenderAction::~SoBoxHighlightRenderAction(void)
{
  this->postprocpath->unref();
  delete this->searchaction;
}

/*!
  Overloaded to add highlighting after the "ordinary" rendering.
*/
void
SoBoxHighlightRenderAction::apply(SoNode * node)
{
  SoGLRenderAction::apply(node);
  if (this->hlVisible) {
    if (this->searchaction == NULL) {
      this->searchaction = new SoSearchAction;
      this->searchaction->setType(SoSelection::getClassTypeId());
      this->searchaction->setInterest(SoSearchAction::FIRST);
    }
    this->searchaction->apply(node);
    if (this->searchaction->isFound()) {
      SoSelection * selection =
        (SoSelection *)this->searchaction->getPath()->getTail();
      assert(selection->getTypeId(). isDerivedFrom(SoSelection::getClassTypeId()));

      if (selection->getNumSelected()) {
        this->drawBoxes(this->searchaction->getPath(), selection->getList());
      }
    }
  }
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
  this->color = color;
}

/*!
  Returns rendering color of the highlighted boxes.
*/
const SbColor &
SoBoxHighlightRenderAction::getColor(void)
{
  return this->color;
}

/*!
  Sets the line \a pattern used for the highlighted boxes. Defaults to
  \c 0xffff (i.e. drawn with no stipples).
*/
void
SoBoxHighlightRenderAction::setLinePattern(unsigned short pattern)
{
  this->linepattern = pattern;
}

/*!
  Returns line pattern used when drawing boxes.
*/
unsigned short
SoBoxHighlightRenderAction::getLinePattern(void) const
{
  return this->linepattern;
}

/*!
  Sets the line \a width used when drawing boxes, in screen pixels (as
  for all OpenGL rendering). Defaults to 3.
*/
void
SoBoxHighlightRenderAction::setLineWidth(const float width)
{
  this->linewidth = width;
}

/*!
  Returns the line width used when drawing highlight boxes.
*/
float
SoBoxHighlightRenderAction::getLineWidth(void) const
{
  return this->linewidth;
}


void
SoBoxHighlightRenderAction::drawBoxes(SoPath * pathtothis, const SoPathList * pathlist)
{
  int i;
  int thispos = ((SoFullPath *)pathtothis)->getLength()-1;
  assert(thispos >= 0);
  this->postprocpath->truncate(0); // reset

  for (i = 0; i < thispos; i++)
    this->postprocpath->append(pathtothis->getNode(i));

  SoState * state = this->getState();
  state->push();

  SoLightModelElement::set(state, SoLightModelElement::BASE_COLOR);
  SoDiffuseColorElement::set(state, NULL, 1, &color);
  SoLineWidthElement::set(state, this->linewidth);
  SoLinePatternElement::set(state, this->linepattern);
  SoTextureQualityElement::set(state, 0.0f);
  SoComplexityTypeElement::set(state, SoComplexityTypeElement::BOUNDING_BOX);
  SoDrawStyleElement::set(state, SoDrawStyleElement::LINES);
  SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
  SoOverrideElement::setDiffuseColorOverride(state, NULL, TRUE);
  SoOverrideElement::setLineWidthOverride(state, NULL, TRUE);
  SoOverrideElement::setLinePatternOverride(state, NULL, TRUE);
  SoOverrideElement::setComplexityTypeOverride(state, NULL, TRUE);
  SoTextureOverrideElement::setQualityOverride(state, TRUE);

  for (i = 0; i < pathlist->getLength(); i++) {
    SoFullPath * path = (SoFullPath *)(*pathlist)[i];

    for (int j = 0; j < path->getLength(); j++) {
      this->postprocpath->append(path->getNode(j));
    }

    SoGLRenderAction::apply(this->postprocpath);
    this->postprocpath->truncate(thispos);
  }
  state->pop();
}
