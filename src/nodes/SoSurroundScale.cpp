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
  \class SoSurroundScale SoSurroundScale.h Inventor/nodes/SoSurroundScale.h
  \brief The SoSurroundScale class is used to automatically scale geometry to surround other geometry.
  \ingroup nodes

  This node calculates a transformation (a translation and a scale)
  which will, when the node is traversed, be appended to the current
  model matrix, making a default cube placed directly to the right of
  this node in the graph surround geometry to the right of the
  container branch this node is on. The container is specified by the
  field SoSurroundScale::numNodesUpToContainer.

  When calculating the bounding box to be surrounded, the bounding box
  action will be applied to the container node, and the bounding box
  calculations will be reset after traversing the node specified by
  the field SoSurroundScale::numNodesUpToReset.

  Here's a complete, stand-alone example which sets up am
  SoTrackballDragger (connected to a cube), uses an SoSurroundScale
  node to make it auto-scale to the size of the high cube and
  translates it an offset to one side of the cube:

  \code
  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/draggers/SoTrackballDragger.h>
  #include <Inventor/nodes/SoAntiSquish.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/nodes/SoCube.h>
  #include <Inventor/nodes/SoSurroundScale.h>
  #include <Inventor/nodes/SoRotation.h>
  #include <Inventor/nodes/SoTranslation.h>


  int
  main(int argc, char **argv)
  {
    QWidget * window = SoQt::init(argv[0]);
    if (window == NULL) exit(1);

    SoSeparator * root = new SoSeparator;
    root->ref();

    SoSeparator * surroundsep = new SoSeparator;
    root->addChild(surroundsep);

    SoTranslation * translation = new SoTranslation;
    translation->translation = SbVec3f(12, 0, 0);
    surroundsep->addChild(translation);

    SoSurroundScale * ss = new SoSurroundScale;
    ss->numNodesUpToReset = 1;
    ss->numNodesUpToContainer = 2;
    surroundsep->addChild(ss);

    SoAntiSquish * antisquish = new SoAntiSquish;
    antisquish->sizing = SoAntiSquish::BIGGEST_DIMENSION;
    surroundsep->addChild(antisquish);

    SoTrackballDragger * dragger = new SoTrackballDragger;
    surroundsep->addChild(dragger);

    SoRotation * rotation = new SoRotation;
    rotation->rotation.connectFrom(& dragger->rotation);
    root->addChild(rotation);

    SoCube * cube = new SoCube;
    cube->height = 10.0f;
    root->addChild(cube);

    SoQtExaminerViewer * viewer = new SoQtExaminerViewer(window);
    viewer->setSceneGraph(root);
    viewer->setViewing(FALSE);
    viewer->setDecoration(FALSE);
    viewer->show();

    SoQt::show(window);
    SoQt::mainLoop();

    delete viewer;
    root->unref();

    return 0;
  }
  \endcode

  It might be easier to see how the SoSurroundScale node works in the
  above example by looking at the actual scenegraph:

  \code
  #Inventor V2.1 ascii

  Separator {
     Separator {
        Translation {
           translation 12 0 0
        }
        SurroundScale {
           numNodesUpToContainer 2
           numNodesUpToReset 1
        }
        AntiSquish {
           sizing BIGGEST_DIMENSION
        }
        DEF +0 TrackballDragger {
        }
     }
     Rotation {
        rotation 0 0 1  0 =
        USE +0		. rotation
     }
     Cube {
        height 10
     }
  }
  \endcode

  This node is internally used by manipulators to make it possible for
  them to scale their dragger's geometry to match the scenegraph
  geometry it is modifying, as is demonstrated above (but outside of
  the context of a manipulator).

  It is also generally useful for application programmers who want any
  particular piece of geometry surround other geometry of unknown or
  changing extent.

  SoSurroundScale nodes in the scenegraph is often paired up with
  SoAntiSquish nodes to get uniform scaling along all three principal
  axes.

  \sa SoAntiSquish
*/

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToContainer

  Number of nodes in the path counting from this and "upwards" to the
  container node.
*/
/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToReset

  Number of nodes in the path counting from this and "upwards" to the
  node where we will reset the bounding box value.
*/

/*!
  \var SoSurroundScale::cachedScale
  \COININTERNAL
*/
/*!
  \var SoSurroundScale::cachedInvScale
  \COININTERNAL
*/
/*!
  \var SoSurroundScale::cachedTranslation
  \COININTERNAL
*/
/*!
  \var SoSurroundScale::cacheOK
  \COININTERNAL
*/
/*!
  \var SoSurroundScale::doTranslations
  \COININTERNAL
*/


// *************************************************************************

SO_NODE_SOURCE(SoSurroundScale);

/*!
  Constructor.
*/
SoSurroundScale::SoSurroundScale(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSurroundScale);

  SO_NODE_ADD_FIELD(numNodesUpToContainer, (0));
  SO_NODE_ADD_FIELD(numNodesUpToReset, (0));

  this->cacheOK = FALSE;
  this->ignoreinbbox = FALSE;
  this->doTranslations = TRUE;
}

/*!
  Destructor.
*/
SoSurroundScale::~SoSurroundScale()
{
}

// Doc in superclass.
void
SoSurroundScale::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSurroundScale, SO_FROM_INVENTOR_1);
}


/*!
  Invalidates the cached transformation, forcing a recalculation to be
  done the next time this node is traversed.
*/
void
SoSurroundScale::invalidate(void)
{
  this->cacheOK = FALSE;
}

// Doc in superclass.
void
SoSurroundScale::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (!this->cacheOK) {
    SbMatrix dummy;
    this->updateMySurroundParams(action, dummy);
  }
  if (this->doTranslations &&
      this->cachedTranslation != SbVec3f(0.0f, 0.0f, 0.0f)) {
    SoModelMatrixElement::translateBy(state, this, this->cachedTranslation);
  }
  if (this->cachedScale != SbVec3f(1.0f, 1.0f, 1.0f))
    SoModelMatrixElement::scaleBy(state, this, this->cachedScale);
}

/*!
  Sets whether the translation part of the transformation should be
  ignored or not. Default behavior is to translate.
*/
void
SoSurroundScale::setDoingTranslations(const SbBool val)
{
  this->doTranslations = val;
}

/*!
  Returns whether the translation part of the transformation should be
  ignored or not.

  \sa setDoingTranslations()
*/
SbBool
SoSurroundScale::isDoingTranslations(void)
{
  return this->doTranslations;
}

// Doc in superclass.
void
SoSurroundScale::callback(SoCallbackAction * action)
{
  SoSurroundScale::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoSurroundScale::GLRender(SoGLRenderAction * action)
{
  SoSurroundScale::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoSurroundScale::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (!this->isIgnoreInBbox())
    SoSurroundScale::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoSurroundScale::getMatrix(SoGetMatrixAction * action)
{
  if (!this->cacheOK) {
    this->updateMySurroundParams(action, action->getInverse());
  }

  if (this->doTranslations &&
      this->cachedTranslation != SbVec3f(0.0f, 0.0f, 0.0f)) {
    SbMatrix m;
    m.setTranslate(this->cachedTranslation);
    action->getMatrix().multLeft(m);
    m.setTranslate(- this->cachedTranslation);
    action->getInverse().multRight(m);
  }

  if (this->cachedScale != SbVec3f(1.0f, 1.0f, 1.0f)) {
    SbMatrix m;
    m.setScale(this->cachedScale);
    action->getMatrix().multLeft(m);
    m.setScale(SbVec3f(1.0f / this->cachedScale[0],
                       1.0f / this->cachedScale[1],
                       1.0f / this->cachedScale[2]));
    action->getInverse().multRight(m);
  }
}

// Doc in superclass.
void
SoSurroundScale::pick(SoPickAction * action)
{
  SoSurroundScale::doAction((SoAction *)action);
}

/*!
  Calculates the translation and scale needed to make a default cube
  surround geometry to the right of the branch this node is on.
*/
void
SoSurroundScale::updateMySurroundParams(SoAction * action,
                                        const SbMatrix & /*inv*/)
{
  // I haven't found any use for the inv argument. The function
  // should be kept as is to make this node OIV compatible though.
  // pederb, 20000220

  int numtocontainer = this->numNodesUpToContainer.getValue();
  int numtoreset = this->numNodesUpToReset.getValue();

  if (numtoreset >= numtocontainer) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoSurroundScale::updateMySurroundParams",
                              "illegal field values, numNodesUpToReset (==%d)"
                              "should always be less than "
                              "numNodesUpToContainer (==%d)",
                              numtoreset, numtocontainer);
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

  const SoFullPath * curpath = (const SoFullPath *) action->getCurPath();

  SoNode * applynode = curpath->getNodeFromTail(numtocontainer);

  int start = curpath->getLength() - 1 - numtocontainer;
  int end = curpath->getLength() - 1 - numtoreset;

  if (start < 0 || end < 0) {
    // if values are out of range, just return. This might happen if an
    // SoGetMatrixAction is applied directly on the node..
    this->cachedScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedInvScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedTranslation.setValue(0.0f, 0.0f, 0.0f);
    this->cacheOK = FALSE;
    return;
  }
  assert(start >= 0);
  assert(end >= 0);

  SoTempPath temppath(end-start+1);
  for (int i = start; i <= end; i++) {
    temppath.append(curpath->getNode(i));
  }
  SoGetBoundingBoxAction bboxaction(SoViewportRegionElement::get(action->getState()));

  // reset bbox when returning from surroundscale branch,
  // meaning we'll calculate the bbox of only the geometry
  // to the right of this branch, getting the wanted result.
  bboxaction.setResetPath(&temppath, FALSE, SoGetBoundingBoxAction::ALL);
  bboxaction.apply(applynode);

  SbBox3f box = bboxaction.getBoundingBox();
  if (box.isEmpty()) {
    this->cachedScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedInvScale.setValue(1.0f, 1.0f, 1.0f);
    this->cachedTranslation.setValue(0.0f, 0.0f, 0.0f);
  }
  else {
    box.getSize(this->cachedScale[0], this->cachedScale[1],
                this->cachedScale[2]);

    if (this->cachedScale[0] <= 0.0f ||
        this->cachedScale[1] <= 0.0f ||
        this->cachedScale[2] <= 0.0f) {

      // find the smallest scale not zero
      SbVec3f s = this->cachedScale;
      float min = SbMax(SbMax(s[0], s[1]), s[2]);
      if (s[0] > 0.0f && s[0] < min) min = s[0];
      if (s[1] > 0.0f && s[1] < min) min = s[1];
      if (s[2] > 0.0f && s[2] < min) min = s[2];

      min *= 0.05f; // set empty dimensions to some value
      if (min <= 0.0f) min = 1.0f;
      if (s[0] <= 0.0f) this->cachedScale[0] = min;
      if (s[1] <= 0.0f) this->cachedScale[1] = min;
      if (s[2] <= 0.0f) this->cachedScale[2] = min;
    }

    this->cachedScale *= 0.5f;
    this->cachedInvScale[0] = 1.0f / this->cachedScale[0];
    this->cachedInvScale[1] = 1.0f / this->cachedScale[1];
    this->cachedInvScale[2] = 1.0f / this->cachedScale[2];

    this->cachedTranslation = box.getCenter();
  }

  this->setIgnoreInBbox(storedignore);
  this->cacheOK = TRUE;
}

/*!
  Sets whether bounding box calculations in SoGetBoundingBoxAction
  should be affected by this node.

  Default is to ignore our bounding box calculations.
*/
void
SoSurroundScale::setIgnoreInBbox(const SbBool val)
{
  this->ignoreinbbox = val;
}

/*!
  Returns the value of the flag that decides whether bounding box
  calculations in SoGetBoundingBoxAction should be affected by this
  node.

  \sa setIgnoreInBbox()
*/
SbBool
SoSurroundScale::isIgnoreInBbox(void)
{
  return this->ignoreinbbox;
}
