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
  \class SoUpgrader SoUpgrader.h
  \brief The SoUpgrader class is used to support Inventor files with version < 2.1.
  \ingroup nodes

  This class is needed since some nodes in earlier versions of 
  OpenInventor had different fields than nodes in Inventor V2.1.

*/

#include "SoUpgrader.h"
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include "SoPackedColorV20.h"
#include <stddef.h> // for NULL

//
// init all upgradable classes. Can be called multiple times.
//
static void 
soupgrader_init_classes(void)
{
  static int first = 1;
  if (first) {
    first = 0;
    SoPackedColorV20::initClass();
  }
}

/*!
  Try creating a node of name \a name with Inventor version \a ivversion.
  
  Returns NULL if no such node exists.
*/
SoBase * 
SoUpgrader::tryCreateNode(const SbName & name, const float ivversion)
{
  if (ivversion == 2.0f) {
    soupgrader_init_classes();
    
    SbString s(name.getString());
    s += "V20";
    SoType type = SoType::fromName(SbName(s.getString()));
    if (type.canCreateInstance()) {
      return (SoBase*) type.createInstance();
    }
  }
  return NULL;
}

/*!
  Upgrade \a base, usually created using SoUpgrader::tryCreateNode(),
  to the latest version of the same node.
*/
SoBase * 
SoUpgrader::createUpgrade(const SoBase * base)
{
  soupgrader_init_classes();

  SoType type = base->getTypeId();

  if (type == SoPackedColorV20::getClassTypeId()) {
    SoPackedColorV20 * pp = (SoPackedColorV20*) base;
    return (SoBase*) pp->createUpgrade();
  }
  else {
    SoDebugError::post("SoUpgrader::createUpgrade",
                       "No upgrade functionality available for %s",
                       type.getName().getString());
  }
  return NULL;
}

