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
  \class SoBoxHighlightRenderAction Inventor/include/actions/SoBoxHighlightRenderAction.h
  \brief The SoBoxHighlightRenderAction class is a render action class for highlighting selections.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLineWidthElement.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoBoxHighlightRenderAction)

SoType SoBoxHighlightRenderAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoBoxHighlightRenderAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoBoxHighlightRenderAction::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>


// static variables
SoEnabledElementsList * SoBoxHighlightRenderAction::enabledElements;
SoActionMethodList * SoBoxHighlightRenderAction::methods;

/*!
  \fn SoBoxHighlightRenderAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoBoxHighlightRenderAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoBoxHighlightRenderAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoBoxHighlightRenderAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoBoxHighlightRenderAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This method initializes static data for the SoBoxHightlightRenderAction
  class.
*/

void
SoBoxHighlightRenderAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoBoxHighlightRenderAction)
  assert(SoBoxHighlightRenderAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoBoxHighlightRenderAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoBoxHighlightRenderAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

// *************************************************************************

/*!
  A constructor.
*/

SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(void)
  : inherited(SbViewportRegion())
{
  this->init();
}

/*!
  A constructor.
*/

SoBoxHighlightRenderAction::SoBoxHighlightRenderAction(const SbViewportRegion & viewportRegion)
  : inherited(viewportRegion)
{
  this->init();
}

//
// private. called by both constructors
//
void
SoBoxHighlightRenderAction::init()
{
  SO_ACTION_CONSTRUCTOR(SoBoxHighlightRenderAction);

  SoBoxHighlightRenderAction::methods->setUp(); // initialize action methods

  this->hlVisible = TRUE;
  this->color = SbColor(1.0f, 0.0f, 0.0f);
  this->linepattern = 0xffff;
  this->linewidth = 3.0f;
  this->searchAction = NULL;
}


/*!
  The destructor.
*/

SoBoxHighlightRenderAction::~SoBoxHighlightRenderAction(void)
{
  delete this->searchAction;
}

/*!
  Applies this action on scene rooted by \a node.
*/
void
SoBoxHighlightRenderAction::apply(SoNode *node)
{
  SoGLRenderAction::apply(node);
  if (this->hlVisible) {
    if (this->searchAction == NULL) {
      this->searchAction = new SoSearchAction;
      this->searchAction->setType(SoSelection::getClassTypeId());
      this->searchAction->setInterest(SoSearchAction::FIRST);
    }
    this->searchAction->apply(node);
    if (this->searchAction->isFound()) {
      SoSelection *selection = (SoSelection*)
        this->searchAction->getPath()->getTail();
      assert(selection->getTypeId().
             isDerivedFrom(SoSelection::getClassTypeId()));      

      if (selection->getNumSelected()) {
        this->drawBoxes(this->searchAction->getPath(), selection->getList());
      }
    }
  }
}

/*!
  Sets if highlighted box should be visible.
*/
void
SoBoxHighlightRenderAction::setVisible(const SbBool visible)
{
  this->hlVisible = visible;
}

/*!
  Return if selected box should be visible.
*/
SbBool
SoBoxHighlightRenderAction::isVisible() const
{
  return this->hlVisible;
}

/*!
  Sets the color for the highlighted box.
*/
void
SoBoxHighlightRenderAction::setColor(const SbColor &color)
{
  this->color = color;
}

/*!
  Differs from OIV by returning const.
*/
const SbColor &
SoBoxHighlightRenderAction::getColor()
{
  return this->color;
}

/*!
  Sets the line pattern used for the highlighted box.
*/
void
SoBoxHighlightRenderAction::setLinePattern(unsigned short pattern)
{
  this->linepattern = pattern;
}

/*!
  Returns line pattern used when drawing box.
*/
unsigned short
SoBoxHighlightRenderAction::getLinePattern() const
{
  return this->linepattern;
}

/*!
  Sets the line width used when drawing box.
*/
void
SoBoxHighlightRenderAction::setLineWidth(const float width)
{
  this->linewidth = width;
}

/*!
  Returns the line width used when drawing highlight box.
*/
float
SoBoxHighlightRenderAction::getLineWidth() const
{
  return this->linewidth;
}

void 
SoBoxHighlightRenderAction::drawBoxes(SoPath *pathtothis, const SoPathList *pathlist)
{
  int i;
  int thispos = ((SoFullPath*)pathtothis)->getLength()-1;
  assert(thispos >= 0);
  SoTempPath temppath(32);
  temppath.ref(); // to avoid having refcount == 0

  for (i = 0; i < thispos; i++) temppath.append(pathtothis->getNode(i));

  SoState *state = this->getState();
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
    SoFullPath *path = (SoFullPath*)(*pathlist)[i];

    for (int j = 0; j < path->getLength(); j++) {
      temppath.append(path->getNode(j));
    }
    
    SoGLRenderAction::apply(&temppath);
    temppath.truncate(thispos);
  }
  state->pop();
}

