/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSelection.h>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <Inventor/SoPath.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoBoxHighlightRenderAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoBoxHighlightRenderAction )

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
//$ END TEMPLATE ActionClassTypeSource

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
//$ BEGIN TEMPLATE InitActionSource( SoBoxHighlightRenderAction )
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
 
  this->localRoot = new SoSeparator;
  this->localRoot->ref();
 
  this->lightModel = new SoLightModel;
  this->lightModel->model = SoLightModel::BASE_COLOR;
  this->baseColor = new SoBaseColor;
  this->drawStyle = new SoDrawStyle;
  this->drawStyle->style = SoDrawStyle::LINES; 
  this->texture = new SoTexture2; // only there to turn off texture
  this->xlate = new SoTranslation; // transform to bbox center
  this->xform = new SoMatrixTransform; // scale and rotate
  this->cube = new SoCube; // draw actual box

  this->localRoot->addChild(this->lightModel);
  this->localRoot->addChild(this->baseColor);
  this->localRoot->addChild(this->drawStyle);
  this->localRoot->addChild(this->texture);
  this->localRoot->addChild(this->xlate);
  this->localRoot->addChild(this->xform);
  this->localRoot->addChild(this->cube);

  this->localRoot->renderCaching = SoSeparator::OFF;
  this->localRoot->boundingBoxCaching = SoSeparator::OFF;
  this->localRoot->renderCulling = SoSeparator::OFF;
  this->localRoot->pickCulling = SoSeparator::OFF;

  this->hlVisible = TRUE;
  this->selPath = NULL;
  this->searchAction = NULL;
  this->bboxAction = NULL;
}


/*!
  The destructor.
*/

SoBoxHighlightRenderAction::~SoBoxHighlightRenderAction(void)
{
  this->localRoot->unref();
  if (this->selPath) this->selPath->unref();
  delete this->searchAction;
  delete this->bboxAction;
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
      
      int n = selection->getNumSelected();
      for (int i = 0; i < n; i++) {
	this->updateBBox(selection->getPath(i));
	SoGLRenderAction::apply(this->localRoot);
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
  this->baseColor->rgb = color;
}

/*!
  Differs from OIV by returning const.
*/
const SbColor &
SoBoxHighlightRenderAction::getColor()
{
  return this->baseColor->rgb[0];
}

/*!
  Sets the line pattern used for the highlighted box.
*/
void 
SoBoxHighlightRenderAction::setLinePattern(unsigned short pattern)
{
  this->drawStyle->linePattern = pattern;
}

/*!
  Returns line pattern used when drawing box.
*/
unsigned short 
SoBoxHighlightRenderAction::getLinePattern() const
{
  return this->drawStyle->linePattern.getValue();
}

/*!
  Sets the line width used when drawing box.
*/
void 
SoBoxHighlightRenderAction::setLineWidth(const float width)
{
  this->drawStyle->lineWidth = width;
}

/*!
  Returns the line width used when drawing highlight box.
*/
float 
SoBoxHighlightRenderAction::getLineWidth() const
{
  return this->drawStyle->lineWidth.getValue();
}

/*!
  Is called to calculate bbox for a selected object.
*/
void 
SoBoxHighlightRenderAction::updateBBox(SoPath *path)
{
  if (this->bboxAction == NULL) {
    this->bboxAction = new SoGetBoundingBoxAction(this->getViewportRegion());
  }
  this->bboxAction->setViewportRegion(this->getViewportRegion());
  this->bboxAction->apply(path);
  SbBox3f box = this->bboxAction->getBoundingBox();
  this->xlate->translation = box.getCenter();
  float w, h, d;
  box.getSize(w,h,d);
  SbMatrix scale;
  scale.makeIdentity();
  scale.setScale(SbVec3f(w,h,d));
  this->xform->matrix = scale;
}

