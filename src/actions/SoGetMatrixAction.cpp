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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoGetMatrixAction Inventor/actions/SoGetMatrixAction.h
  \brief The SoGetMatrixAction class is an action for getting the
  transformation matrix for a subgraph.

  FIXME: doc
*/

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/elements/SoViewportRegionElement.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGetMatrixAction )

SoType SoGetMatrixAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGetMatrixAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGetMatrixAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoGetMatrixAction::enabledElements;
SoActionMethodList * SoGetMatrixAction::methods;

/*!
  \fn SoGetMatrixAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGetMatrixAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGetMatrixAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGetMatrixAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGetMatrixAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes all the static data for the
  SoGetMatrixAction class.
*/

void
SoGetMatrixAction::initClass()
{
//$ BEGIN TEMPLATE InitActionSource( SoGetMatrixAction )
  assert(SoGetMatrixAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGetMatrixAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGetMatrixAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  methods->setDefault((void *)SoNode::getMatrixS);
  ENABLE_ELEMENT(SoViewportRegionElement);
  //FIXME: enable other elements (check with Toolmaker p.13->) (kintel 1990616)
}

/*!
  This static method cleans up all the static data for the
  SoGetMatrixAction class.
*/

void
SoGetMatrixAction::cleanClass()
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoGetMatrixAction::SoGetMatrixAction(const SbViewportRegion &newRegion)
{
  this->viewportRegion = newRegion;
}

/*!
  The destructor.
*/

SoGetMatrixAction::~SoGetMatrixAction()
{
}

/*!
  This method sets the viewport region.
*/

void 
SoGetMatrixAction::setViewportRegion(const SbViewportRegion &newRegion)
{
  this->viewportRegion=newRegion;
}

/*!
  This method returns the viewport region.
*/

const SbViewportRegion &
SoGetMatrixAction::getViewportRegion() const
{
  return this->viewportRegion;
}

/*!
  This method returns the transformation matrix.
*/

//$ EXPORT INLINE
SbMatrix &
SoGetMatrixAction::getMatrix()
{
  return this->matrix;
}

/*!
  This method returns the inverse matrix.
*/

//$ EXPORT INLINE
SbMatrix &
SoGetMatrixAction::getInverse()
{
  return this->invMatrix;
}

/*!
  This method returns the texture matrix.
*/

//$ EXPORT INLINE
SbMatrix &
SoGetMatrixAction::getTextureMatrix()
{
  return this->texMatrix;
}

/*!
  This method returns the inverse texture matrix.
*/

//$ EXPORT INLINE
SbMatrix &
SoGetMatrixAction::getTextureInverse()
{
  return this->invTexMatrix;
}

/*!
  This method makes sure the graph is not traversed like it normally would.
 */
void
SoGetMatrixAction::beginTraversal(SoNode *node)
{
  assert(this->traversalMethods);
  
  (*this->traversalMethods)
    [SoNode::getActionMethodIndex(node->getTypeId())](this, node);
}

// the following functions are not in the original Inventor API

/*!
  FIXME: write doc.
*/

void 
SoGetMatrixAction::mult(const SbMatrix &matrix)
{
  this->matrix.multRight(matrix);
  this->invMatrix.multRight(matrix.inverse());
}

/*!
  FIXME: write doc.
*/

void 
SoGetMatrixAction::translateBy(const SbVec3f &vec)
{
  SbMatrix mat;
  mat.setTranslate(vec);
  this->matrix.multRight(mat);
  mat.setTranslate(-vec);
  this->invMatrix.multRight(mat);
}

/*!
  FIXME: write doc.
*/

void 
SoGetMatrixAction::rotateBy(const SbRotation &rot)
{
  SbMatrix mat;
  mat.setRotate(rot);
  this->matrix.multRight(mat);
  mat.setRotate(rot.inverse());
  this->invMatrix.multRight(mat);
}

/*!
  FIXME: write doc.
*/

void 
SoGetMatrixAction::scaleBy(const SbVec3f &scaleFactor)
{
  SbMatrix mat;
  mat.setScale(scaleFactor);
  this->matrix.multRight(mat);
  mat.setScale(-scaleFactor);
  this->invMatrix.multRight(mat);
}

