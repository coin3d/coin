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
  \class SoSurroundScale SoSurroundScale.h Inventor/nodes/SoSurroundScale.h
  \brief The SoSurroundScale class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/misc/SoTempPath.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToContainer
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToReset
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoSurroundScale);

/*!
  Constructor.
*/
SoSurroundScale::SoSurroundScale()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSurroundScale);

  SO_NODE_ADD_FIELD(numNodesUpToContainer, (0));
  SO_NODE_ADD_FIELD(numNodesUpToReset, (0));

  this->cacheOK = FALSE;
  this->ignoreInBBox = FALSE;

  // FIXME: test how to support this, pederb, 20000215
  this->doTranslations = TRUE;
}

/*!
  Destructor.
*/
SoSurroundScale::~SoSurroundScale()
{
}

/*!
  Does initialization common for all objects of the
  SoSurroundScale class. This includes setting up the
  type system, among other things.
*/
void
SoSurroundScale::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSurroundScale);
}


/*!
  FIXME: write doc
 */
void
SoSurroundScale::invalidate(void)
{
  this->cacheOK = FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::doAction(SoAction *action)
{
  SoState *state = action->getState();
  if (!this->cacheOK) {
    SbMatrix inv = SoModelMatrixElement::get(state).inverse();
    this->updateMySurroundParams(action, inv);
  }
  if (this->cachedTranslation != SbVec3f(0.0f, 0.0f, 0.0f))
    SoModelMatrixElement::translateBy(state, this, this->cachedTranslation);
  if (this->cachedScale != SbVec3f(1.0f, 1.0f, 1.0f)) 
    SoModelMatrixElement::scaleBy(state, this, this->cachedScale);
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::setDoingTranslations(const SbBool val)
{
  this->doTranslations = val;
}

/*!
  FIXME: write doc
*/
SbBool
SoSurroundScale::isDoingTranslations(void)
{
  return this->doTranslations;
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::callback(SoCallbackAction *action)
{
  SoSurroundScale::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::GLRender(SoGLRenderAction *action)
{
  SoSurroundScale::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::getBoundingBox(SoGetBoundingBoxAction *action)
{
  if (!this->isIgnoreInBbox())
    SoSurroundScale::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::getMatrix(SoGetMatrixAction *action)
{
  if (!this->cacheOK) {
    this->updateMySurroundParams(action, action->getInverse());
  }
  if (this->cachedTranslation != SbVec3f(0.0f, 0.0f, 0.0f))
    action->translateBy(this->cachedTranslation);
  if (this->cachedScale != SbVec3f(1.0f, 1.0f, 1.0f))
    action->scaleBy(this->cachedScale);
}

/*!
  FIXME: write doc
*/
void
SoSurroundScale::pick(SoPickAction *action)
{
  SoSurroundScale::doAction((SoAction*)action);
}

/*!
  Calculates the translation and scale needed to make a 
  default cube surround geometry to the right of the
  branch this node is on.
*/
void
SoSurroundScale::updateMySurroundParams(SoAction *action,
                                        const SbMatrix &invmodelmatrix)
{
  this->cacheOK = TRUE;
  int numtocontainer = this->numNodesUpToContainer.getValue();
  int numtoreset = this->numNodesUpToReset.getValue();

  if (numtoreset >= numtocontainer) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoSurroundScale::updateMySurroundParams",
                              "illegal field values");
#endif // debug
    this->cachedScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedInvScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedTranslation.setValue(0.0f, 0.0f, 0.0f);
    this->cacheOK = TRUE;
    return;
  }

  // make sure we don't get here when calculating the bbox
  SbBool storedignore = this->isIgnoreInBbox();
  this->setIgnoreInBbox(TRUE);

  const SoFullPath *curpath = (const SoFullPath*) action->getCurPath(); 
  
  SoNode *applynode = curpath->getNodeFromTail(numtocontainer);

  SoTempPath temppath(4);
  int start = curpath->getLength() - 1 - numtocontainer;
  int end = curpath->getLength() - 1 - numtoreset;
  
  assert(start >= 0);
  assert(end >= 0);
  
  for (int i = start; i <= end; i++) {
    temppath.append(curpath->getNode(i));
  } 
  SoGetBoundingBoxAction bboxaction(SoViewportRegionElement::get(action->getState()));

  // reset bbox when returning from surroundscale branch,
  // meaning we'll calculate the bbox of only the geometry
  // to the right of this branch, getting the wanted result.
  bboxaction.setResetPath(&temppath, FALSE, SoGetBoundingBoxAction::ALL);
  bboxaction.apply(applynode);
  
  bboxaction.getXfBoundingBox().transform(invmodelmatrix);
  SbBox3f box = bboxaction.getBoundingBox();
  
  box.getSize(this->cachedScale[0], this->cachedScale[1],
              this->cachedScale[2]);
  this->cachedScale *= 0.5f;
  this->cachedInvScale[0] = 1.0f / this->cachedScale[0];
  this->cachedInvScale[1] = 1.0f / this->cachedScale[1];
  this->cachedInvScale[2] = 1.0f / this->cachedScale[2];

  // FIXME: investigate what to do when a translation is needed.
  // pederb, 20000216
  this->cachedTranslation = SbVec3f(0,0,0);

  this->setIgnoreInBbox(storedignore);
  this->cacheOK = TRUE;
}

/*!
  Sets whether bounding box in SoGetBoundingBoxAction should
  be affected by this node.
*/
void
SoSurroundScale::setIgnoreInBbox(const SbBool val)
{
  this->ignoreInBBox = val;
}

/*!
  Returns whether bounding box in SoGetBoundingBoxAction should
  be affected by this node.
*/
SbBool
SoSurroundScale::isIgnoreInBbox(void)
{
  return this->ignoreInBBox;
}
