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
  \class SoProfile SoProfile.h Inventor/nodes/SoProfile.h
  \brief The SoProfile class is the abstract superclass for profile definitions.
  \ingroup nodes

  Node subclasses of SoProfile specifies profiles for extruded 3D text
  and nurbs surface data.
*/

#include <Inventor/nodes/SoProfile.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/elements/SoProfileElement.h>

/*!
  \enum SoProfile::Profile

  Enumeration of various choices of how to link together multiple
  profiles.
*/
/*!
  \var SoProfile::Profile SoProfile::START_FIRST
  Replace the current profile state set with this profile alone.
*/
/*!
  \var SoProfile::Profile SoProfile::START_NEW
  Append this profile to the state as a new profile.
*/
/*!
  \var SoProfile::Profile SoProfile::ADD_TO_CURRENT
  Append indices of this node to the last profile.
*/


/*!
  \var SoMFInt32 SoProfile::index

  Profile coordinate indices.

  These must match what is available from previous
  SoProfileCoordinate2 and SoProfileCoordinate3 nodes in the
  traversal.
*/
/*!
  \var SoSFEnum SoProfile::linkage

  How the indices of this profile node should be combined with the
  current profile index set of the traversal state.
*/


/*!
  \fn void SoProfile::getTrimCurve(SoState * state, int32_t & numpoints, float *& points, int & floatspervec, int32_t & numknots, float *& knotvector)
  Return \a points and \a knotvector of the \a state.
*/

/*!
  \fn void SoProfile::getVertices(SoState * state, int32_t & numvertices, SbVec2f *& vertices)
  Return vertex set of \a state.
*/


// *************************************************************************

SO_NODE_ABSTRACT_SOURCE(SoProfile);

/*!
  Constructor.
*/
SoProfile::SoProfile(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoProfile);

  SO_NODE_ADD_FIELD(index, (0));
  SO_NODE_ADD_FIELD(linkage, (START_FIRST));
}

/*!
  Destructor.
*/
SoProfile::~SoProfile()
{
}

// Doc from superclass.
void
SoProfile::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoProfile);

  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoProfileCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoProfileElement);
  SO_ENABLE(SoGLRenderAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoProfileElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileCoordinateElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProfileElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProfileElement);
  SO_ENABLE(SoPickAction, SoProfileCoordinateElement);
  SO_ENABLE(SoPickAction, SoProfileElement);
}

// Doc from superclass.
void
SoProfile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoProfile::doAction(action);
}

// Doc from superclass.
void
SoProfile::doAction(SoAction * action)
{
  SoProfileElement::add(action->getState(), this);
}

// Doc from superclass.
void
SoProfile::callback(SoCallbackAction * action)
{
  SoProfile::doAction(action);
}

// Doc from superclass.
void
SoProfile::GLRender(SoGLRenderAction * action)
{
  SoProfile::doAction(action);
}

// Doc from superclass.
void
SoProfile::pick(SoPickAction * action)
{
  SoProfile::doAction(action);
}

// Doc from superclass.
void
SoProfile::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoProfile::doAction(action);
}
