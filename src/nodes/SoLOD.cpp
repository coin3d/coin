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
  \class SoLOD SoLOD.h Inventor/nodes/SoLOD.h
  \brief The SoLOD class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLOD.h>

#include <Inventor/misc/SoChildList.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#endif // ! COIN_EXCLUDE_SOGLRENDERACTION

#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>


/*!
  \var SoMFFloat SoLOD::range
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoLOD::center
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLOD);

/*!
  Constructor.
*/
SoLOD::SoLOD()
{
  SO_NODE_CONSTRUCTOR(SoLOD);

  SO_NODE_ADD_FIELD(range, (0.0f));
  // FIXME: is this the correct way of making an empty multifield?
  // 19990301 mortene.
  range.setNum(0);

  SO_NODE_ADD_FIELD(center, (SbVec3f(0, 0, 0)));
}

/*!
  Destructor.
*/
SoLOD::~SoLOD()
{
}

/*!
  Does initialization common for all objects of the
  SoLOD class. This includes setting up the
  type system, among other things.
*/
void
SoLOD::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLOD);
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::GLRender(SoGLRenderAction * action)
{  
  assert(action->getCurPathCode() == SoAction::NO_PATH);
  int idx = this->whichToTraverse(action);;
  if (idx >= 0) this->children->traverse(action, idx);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderBelowPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);
  if (idx >= 0) this->getChildren()->traverse(action, idx);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderInPath(SoGLRenderAction * action)
{
  int numIndices;
  const int *indices; 
  action->getPathCode(numIndices, indices);

  // no need to traverse OFF_PATH children
  for (int i = 0; i < numIndices; i++) {
    this->children->traverse(action, indices[i]);
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderOffPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);;
  if (idx >= 0 && this->getChild(idx)->affectsState())
    this->children->traverse(action, idx);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::rayPick(SoRayPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: what should be done here? Calculating a new bbox each
  // time LOD is changed might hurt bbox cache performance.
  inherited::getBoundingBox(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoLOD::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  Returns the child to traverse based on the ranges in 
  SoLOD::range. Will clamp to index to the number of children.
  This method will return -1 if no child should be traversed.
  This will only happen if the node has no children though.
*/
int
SoLOD::whichToTraverse(SoAction *action)
{
  SoState *state = action->getState();
  const SbMatrix &mat = SoModelMatrixElement::get(state);
  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f worldcenter; 
  mat.multVecMatrix(this->center.getValue(), worldcenter);
  
  float dist = (vv.getProjectionPoint() - worldcenter).length();

  int i;
  int n = this->range.getNum();
  
  for (i = 0; i < n; i++) {
    if (dist < this->range[i]) break;
  }
  if (i >= this->getNumChildren()) i = this->getNumChildren() - 1;  
  return i;
}
#endif // !COIN_EXCLUDE_SOACTION
