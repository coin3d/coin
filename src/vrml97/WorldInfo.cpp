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

/*!
  \class SoVRMLWorldInfo SoVRMLWorldInfo.h Inventor/VRMLnodes/SoVRMLWorldInfo.h
  \brief The SoVRMLWorldInfo class contains information about the VRML scene.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  WorldInfo {
    field MFString info  []
    field SFString title ""
  }
  \endverbatim
 
  The WorldInfo node contains information about the world. This node
  is strictly for documentation purposes and has no effect on the
  visual appearance or behaviour of the world. The \e title field is
  intended to store the name or title of the world so that browsers
  can present this to the user (perhaps in the window border). Any
  other information about the world can be stored in the \e info
  field, such as author information, copyright, and usage
  instructions.

*/

/*!
  \var SoSFString SoVRMLWorldInfo::title
  World title strings.
*/

/*!
  \var SoMFString SoVRMLWorldInfo::info
  Info strings.
*/

#include <Inventor/VRMLnodes/SoVRMLWorldInfo.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

SO_NODE_SOURCE(SoVRMLWorldInfo);

// Doc in parent
void
SoVRMLWorldInfo::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLWorldInfo, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLWorldInfo::SoVRMLWorldInfo(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLWorldInfo);

  SO_VRMLNODE_ADD_FIELD(title, (""));
  SO_VRMLNODE_ADD_EMPTY_MFIELD(info);
}

/*!
  Destructor.
*/
SoVRMLWorldInfo::~SoVRMLWorldInfo()
{
}
