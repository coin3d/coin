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
  \class SoLevelOfDetail SoLevelOfDetail.h Inventor/nodes/SoLevelOfDetail.h
  \brief The SoLevelOfDetail class is used to choose a child based on projected size.
  \ingroup nodes

  A level-of-detail mechanism is typically used by application
  programmers to assist the library in speeding up the rendering.

  The way a level-of-detail mechanism works is basically like this:

  Several versions of varying complexity of \e the \e same geometry /
  shape is provided by the application programmer in sorted order from
  "most complex" to "least complex" (where "complex" in this context
  should be taken to mean more or less detailed in the number of
  polygons / shapes used for rendering it).

  The run-time rendering system then, upon scenegraph traversal,
  calculates on-the-fly either the distance from the camera to the
  3D-model in question, or the number of pixels in the screen
  projection of the 3D-model. This value is then used to decide which
  version of the model to actually render: as the model is moved
  farther away from the camera, a less detailed version of the model
  is used. And vice versa, as the model moves closer to the camera,
  more and more detailed versions of it are rendered.

  This is under many different circumstances a very effective way to
  let the application programmer assist to \e profoundly optimize the
  rendering of her 3D-scene.

  There is of course a trade-off with the level-of-detail technique:
  more versions of the same 3D model means the scenegraph will use up
  more application memory resources. Also, generating the set of less
  and less detailed versions of a 3D model from the original is often
  not a trivial task to do properly. The process is often assisted by
  software like what Systems in Motion offers in their <a
  href="http://www.rational-reducer.com>Rational Reducer</a>package.


  The SoLevelOfDetail node implements the "projected size" variety
  level-of-detail technique (as opposed to the "object distance"
  technique, as done by the SoLOD node).

  The node works by comparing the current projected size of the
  smallest rectangle covering the bounding box of it's child geometry.
  

  Along with this set of models of the same shape, a specification of
  when to switch between them is also provided.

  \code
LevelOfDetail {
   screenArea [ 2000, 500, 50 ]

   DEF version-0 Separator {
     # most complex / detailed / heavy version of subgraph
   }
   DEF version-1 Separator {
     # less complex version of subgraph
   }
   DEF version-2 Separator {
     # even less complex version of subgraph
   }
   DEF version-3 Separator {
     # simplest / "lightest" version of subgraph
   }
}
  \endcode

  The way the above sub-scenegraph would work would be the following:
  if the rectangular area around the model's projected bounding box
  covers \e more than 2000 pixels (meaning it will be up pretty close
  to the camera), the most complex version of the model (\c version-0)
  would be traversed (and rendered, of course). If the projected area
  would be \e between 500 and 2000 pixels, the \c version-1 model
  would be used. Ditto if the projected area was between 100 and 500
  pixels, the \c version-2 version of the model would be
  used. Finally, if the projected bounding box area would be \e less
  than 50 square pixels, the presumably least detailed version of the
  modeled would be used.

  (A common "trick" is to let the last of the SoLevelOfDetail node's
  children be just an empty subgraph, so no geometry will be rendered
  at all if the model is sufficiently far away. This will of course
  have a positive effect on the total rendering time for the complete
  scenegraph.)

  Note that the SoLevelOfDetail::screenArea vector will be influenced
  by preceding SoComplexity nodes in the following way: if
  SoComplexity::value is set from 0.0 up to 0.5, lower detail levels
  than normal will be shown. If SoComplexity::value is above 0.5,
  higher level details than normal will be shown. An
  SoComplexity::value equal to 1.0 will cause the first child of
  SoLevelOfDetail to always be shown.


  As mentioned above, there is one other level-of-detail node in the
  Coin library: SoLOD. The difference between that one and this is
  just that instead of projected bounding box area, SoLOD uses the
  distance between the camera and the object to find out when to
  switch between the different model versions.

  Using SoLOD is faster, since figuring out the projected bounding box
  area needs a certain amount of calculations. But using
  SoLevelOfDetail is often "better", in the sense that it's really a
  model's size and visibility in the viewport that determines whether
  we could switch to a less complex version without losing enough
  detail that it gives a noticable visual degradation.

  \sa SoLOD
*/


#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <../tidbits.h> // coin_atexit()
#include <stdlib.h>

// FIXME: not safe in an multithreaded environment. 20020106 mortene.
static SoGetBoundingBoxAction * bboxAction = NULL;

static void
SoLevelOfDetail_cleanup_func(void)
{
  delete bboxAction;
  bboxAction = NULL;
}

/*!
  \var SoMFFloat SoLevelOfDetail::screenArea

  The screen area limits for the children. See usage example in main
  class documentation of SoLevelOfDetail for an explanation of how
  this vector should be set up correctly.

  By default this vector just contains a single value 0.0f.
*/

// *************************************************************************

SO_NODE_SOURCE(SoLevelOfDetail);

/*!
  Default constructor.
*/
SoLevelOfDetail::SoLevelOfDetail(void)
{
  this->commonConstructor();
}

/*!
  Constructor.

  The argument should be the approximate number of children which is
  expected to be inserted below this node. The number need not be
  exact, as it is only used as a hint for better memory resource
  allocation.
*/
SoLevelOfDetail::SoLevelOfDetail(int numchildren)
  : inherited(numchildren)
{
  this->commonConstructor();
}

// private
void
SoLevelOfDetail::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLevelOfDetail);

  SO_NODE_ADD_FIELD(screenArea, (0));
}

/*!
  Destructor.
*/
SoLevelOfDetail::~SoLevelOfDetail()
{
}

// Documented in superclass.
void
SoLevelOfDetail::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLevelOfDetail);
}

// Documented in superclass.
void
SoLevelOfDetail::doAction(SoAction *action)
{
  switch (action->getCurPathCode()) {
  case SoAction::IN_PATH:
    inherited::doAction(action); // normal path traversal
    return;
  case SoAction::OFF_PATH:
    return; // this is a separator node, return.
  case SoAction::BELOW_PATH:
  case SoAction::NO_PATH:
    break; // go on
  default:
    assert(0 && "unknown path code");
    return;
  }

  // for some strange reason, gcc (egcs-2.91.66) won't accept the code
  // below inside a case (yes, I did use brackets).
  // That's the reason for the strange switch/case above. pederb 19991116

  SoState * state = action->getState();
  int n = this->getNumChildren();
  if (n == 0) return;

  SbVec2s size;
  SbBox3f bbox;
  int i;
  int idx = -1;
  float projarea = 0.0f;

  SoComplexityTypeElement::Type complext = SoComplexityTypeElement::get(state);
  float complexity = SbClamp(SoComplexityElement::get(state), 0.0f, 1.0f);

  if (n == 1) { idx = 0; goto traverse; }
  if (complext == SoComplexityTypeElement::BOUNDING_BOX) { idx = n - 1; goto traverse; }
  if (complexity == 0.0f) { idx = n - 1; goto traverse; }
  if (complexity == 1.0f) { idx = 0; goto traverse; }
  if (this->screenArea.getNum() == 0) { idx = 0; goto traverse; }

  if (!bboxAction) {
    // The viewport region will be replaced every time the action is
    // used, so we can just feed it a dummy here.
    bboxAction = new SoGetBoundingBoxAction(SbViewportRegion());
    coin_atexit(SoLevelOfDetail_cleanup_func);
  }

  bboxAction->setViewportRegion(SoViewportRegionElement::get(state));
  bboxAction->apply(this); // find bbox of all children
  bbox = bboxAction->getBoundingBox();
  SoShape::getScreenSize(state, bbox, size);

  // The multiplication factor from the complexity setting is
  // complexity+0.5 because the documented behavior of SoLevelOfDetail
  // is to show lower detail levels than normal when
  // SoComplexity::value < 0.5, and to show higher detail levels when
  // SoComplexity::value > 0.5.
  projarea = float(size[0]) * float(size[1]) * (complexity + 0.5f);

  // In case there are too few or too many screenArea values.
  n = SbMin(n, this->screenArea.getNum());

  for (i = 0; i < n; i++) {
    if (projarea > this->screenArea[i]) { idx = i; goto traverse; }
  }

  // If we get here, projected area was lower than any of the
  // screenArea value, so the last child should be traversed.
  idx = this->getNumChildren() - 1;
  // (fall through to traverse:)

 traverse:
  state->push();
  this->getChildren()->traverse(action, idx);
  state->pop();
  return;
}

// Documented in superclass.
void
SoLevelOfDetail::callback(SoCallbackAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}

// Documented in superclass.
void
SoLevelOfDetail::GLRender(SoGLRenderAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}

// Documented in superclass.
void
SoLevelOfDetail::rayPick(SoRayPickAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}
