/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// This file is used to avoid duplicating sourcecode for the almost
// identical replaceManip() functions over the different manipulator
// classes.

/***************************************************************************/

#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/SoFullPath.h>

/***************************************************************************/

// FIXME: can we use a macro for the documentation aswell? Check to
// see if Doxygen can handle it. 20000427 mortene.

/*!
  \fn SbBool SoTransformManip::replaceManip(SoPath * path, SoTransform * newone) const

  Replaces this manipulator from the position specified by \a path
  with \a newnode. If \a newnode is \c NULL, an SoTransform will be
  created for you.
*/

/*!
  \fn SbBool SoDirectionalLightManip::replaceManip(SoPath * path, SoDirectionalLight * newone) const

  Replaces this manipulator from the position specified by \a path
  with \a newnode. If \a newnode is \c NULL, an SoDirectionalLight
  will be created for you.
*/

/*!
  \fn SbBool SoSpotLightManip::replaceManip(SoPath * path, SoSpotLight * newone) const

  Replaces this manipulator from the position specified by \a path
  with \a newnode. If \a newnode is \c NULL, an SoSpotLight
  will be created for you.
*/

/*!
  \fn SbBool SoPointLightManip::replaceManip(SoPath * path, SoPointLight * newone) const

  Replaces this manipulator from the position specified by \a path
  with \a newnode. If \a newnode is \c NULL, an SoPointLight
  will be created for you.
*/

/*!
  \fn SbBool SoClipPlaneManip::replaceManip(SoPath * path, SoClipPlane * newone) const

  Replaces this manipulator from the position specified by \a path
  with \a newnode. If \a newnode is \c NULL, an SoClipPlane
  will be created for you.
*/

/***************************************************************************/

// FIXME: this should, as far as I can tell, be possible to write as a
// single function without doing it as a macro. 20020805 mortene.

#define SOMANIP_REPLACEMANIPBODY(_class_, _parentclass_) \
SbBool \
_class_::replaceManip(SoPath * path, _parentclass_ * newone) const \
{ \
  SoFullPath * fullpath = (SoFullPath *) path; \
  SoNode * fulltail = fullpath->getTail(); \
 \
  if (fulltail != (SoNode *)this) { \
    SoDebugError::post("_class_::replaceManip", \
                       "child to replace is not this manip (but %s at %p)", \
                       fulltail->getTypeId().getName().getString(), fulltail); \
    return FALSE; \
  } \
 \
  SbBool constructed = FALSE; \
  if (newone == NULL) { \
    newone = new _parentclass_; \
    constructed = TRUE; \
  } \
 \
  this->transferFieldValues(this, newone); \
 \
  if (path->getTail()->isOfType(SoBaseKit::getClassTypeId())) { \
    SoBaseKit * kit = (SoBaseKit *) ((SoNodeKitPath *)path)->getTail(); \
    SbString partname = kit->getPartString(path); \
    if (partname == "" || !kit->setPart(partname, newone)) { \
      SoDebugError::postWarning("_class_::replaceManip", \
                                "failed to replace manip %p with node %p" \
                                "in kit %p (partname='%s')", \
                                this, newone, kit, partname.getString()); \
      if (constructed) { \
        newone->ref(); \
        newone->unref(); \
      } \
      return FALSE; \
    } \
  } \
  else { \
    if (fullpath->getLength() < 2) { \
      SoDebugError::post("_class_::replaceManip", "path is too short"); \
      if (constructed) { \
        newone->ref(); \
        newone->unref(); \
      } \
      return FALSE; \
    } \
 \
    SoNode * parent = fullpath->getNodeFromTail(1); \
 \
    if (!parent->isOfType(SoGroup::getClassTypeId())) { \
      SoDebugError::post("_class_::replaceNode", \
                         "parent node %p is not an SoGroup, but %s", \
                         parent, parent->getTypeId().getName().getString()); \
      if (constructed) { \
        newone->ref(); \
        newone->unref(); \
      } \
      return FALSE; \
    } \
 \
    ((SoGroup*)parent)->replaceChild((SoNode*)this, newone); \
  } \
 \
  return TRUE; \
}

/***************************************************************************/

SOMANIP_REPLACEMANIPBODY(SoTransformManip, SoTransform)
SOMANIP_REPLACEMANIPBODY(SoDirectionalLightManip, SoDirectionalLight)
SOMANIP_REPLACEMANIPBODY(SoSpotLightManip, SoSpotLight)
SOMANIP_REPLACEMANIPBODY(SoPointLightManip, SoPointLight)
SOMANIP_REPLACEMANIPBODY(SoClipPlaneManip, SoClipPlane)
