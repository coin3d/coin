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
  \class SoLineHighlightRenderAction SoLineHighlightRenderAction.h Inventor/actions/SoLineHighlightRenderAction.h
  \brief The SoLineHighlightRenderAction class renders selections with line highlighting.
  \ingroup actions

  See the documentation of SoBoxHighlightRenderAction.

  The only difference between SoBoxHighlightRenderAction and this
  action is that this action renders highlights by superposing a
  wireframe image onto each shape instead of the bounding box when
  drawing the highlight.

  \sa SoBoxHighlightRenderAction, SoSelection
*/

#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/SbName.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/elements/SoLineWidthElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSelection.h>
#include <assert.h>

/*!
  \var SoLineHighlightRenderAction::hlVisible

  Boolean which decides whether or not the highlights for selected
  nodes should be visible.
 */


SO_ACTION_SOURCE(SoLineHighlightRenderAction);


// Override from parent class.
void
SoLineHighlightRenderAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoLineHighlightRenderAction, SoGLRenderAction);
}


/*!
  Default constructor. Note: passes a default SbViewportRegion to the
  parent constructor.
 */
SoLineHighlightRenderAction::SoLineHighlightRenderAction(void)
  : inherited(SbViewportRegion())
{
  this->init();
}

/*!
  Constructor, taking an explicit \a viewportregion to render.
*/
SoLineHighlightRenderAction::SoLineHighlightRenderAction(const SbViewportRegion & viewportregion)
  : inherited(viewportregion)
{
  this->init();
}

/*!
  The destructor.
*/
SoLineHighlightRenderAction::~SoLineHighlightRenderAction()
{
  this->postprocpath->unref();
  delete this->searchaction;
}

//
// private. called by both constructors
//
void
SoLineHighlightRenderAction::init(void)
{
  SO_ACTION_CONSTRUCTOR(SoLineHighlightRenderAction);

  SoLineHighlightRenderAction::methods->setUp(); // initialize action methods

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
  Overloaded to add highlighting after the "ordinary" rendering.
*/
void
SoLineHighlightRenderAction::apply(SoNode * node)
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
      SoSelection * selection = (SoSelection *)
        this->searchaction->getPath()->getTail();
      assert(selection->getTypeId().
             isDerivedFrom(SoSelection::getClassTypeId()));

      if (selection->getNumSelected()) {
        this->drawBoxes(this->searchaction->getPath(), selection->getList());
      }
    }
  }
}

/*!
  Sets if highlight wireframes should be \a visible when
  rendering. Defaults to \c TRUE.
*/
void
SoLineHighlightRenderAction::setVisible(const SbBool visible)
{
  this->hlVisible = visible;
}

/*!
  Return if selection wireframes should be visible.
*/
SbBool
SoLineHighlightRenderAction::isVisible(void) const
{
  return this->hlVisible;
}

/*!
  Sets the \a color of the wireframes. Defaults to red.
*/
void
SoLineHighlightRenderAction::setColor(const SbColor & color)
{
  this->color = color;
}

/*!
  Returns color of selection wireframes.
*/
const SbColor &
SoLineHighlightRenderAction::getColor(void)
{
  return this->color;
}

/*!
  Sets the line \a pattern used when drawing wireframes. Defaults to
  \c 0xffff (i.e. full, unstippled lines).
*/
void
SoLineHighlightRenderAction::setLinePattern(unsigned short pattern)
{
  this->linepattern = pattern;
}

/*!
  Returns line pattern used when drawing wireframe.
*/
unsigned short
SoLineHighlightRenderAction::getLinePattern(void) const
{
  return this->linepattern;
}

/*!
  Sets the line \a width used when drawing wireframe. Defaults to 3
  (measured in screen pixels).
*/
void
SoLineHighlightRenderAction::setLineWidth(const float width)
{
  this->linewidth = width;
}

/*!
  Returns the line width used when drawing wireframe.
*/
float
SoLineHighlightRenderAction::getLineWidth(void) const
{
  return this->linewidth;
}

void
SoLineHighlightRenderAction::drawBoxes(SoPath * pathtothis, const SoPathList * pathlist)
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
  SoPolygonOffsetElement::set(state, NULL, 1.0f, 1.0f, SoPolygonOffsetElement::FILLED, TRUE);
  SoDrawStyleElement::set(state, SoDrawStyleElement::LINES);
  SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
  SoOverrideElement::setDiffuseColorOverride(state, NULL, TRUE);
  SoOverrideElement::setLineWidthOverride(state, NULL, TRUE);
  SoOverrideElement::setLinePatternOverride(state, NULL, TRUE);
  SoOverrideElement::setPolygonOffsetOverride(state, NULL, TRUE);
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
