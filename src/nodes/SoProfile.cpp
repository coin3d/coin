/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/nodes/SoSubNodeP.h>

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
  SO_NODE_DEFINE_ENUM_VALUE(Profile, START_FIRST);
  SO_NODE_DEFINE_ENUM_VALUE(Profile, START_NEW);
  SO_NODE_DEFINE_ENUM_VALUE(Profile, ADD_TO_CURRENT);
  SO_NODE_SET_SF_ENUM_TYPE(linkage, Profile);
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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoProfile, SO_FROM_INVENTOR_1);

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
