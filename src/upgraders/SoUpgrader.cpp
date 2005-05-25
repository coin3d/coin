/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoUpgrader SoUpgrader.h
  \brief The SoUpgrader class is used to support Inventor files with version < 2.1.
  \ingroup nodes

  This class is needed since some nodes in earlier versions of 
  OpenInventor had different fields than nodes in Inventor V2.1.

*/

// *************************************************************************

#include "SoUpgrader.h"

#include <stddef.h> // for NULL
#include <assert.h>

#include "SoPackedColorV20.h"
#include "SoShapeHintsV10.h"
#include <Inventor/C/tidbitsp.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SbHash.h>

// *************************************************************************

// an upgrader lookup dict.
//
// FIXME: add a new method to SoType (other than SoType::fromName),
// that checks if a type with a name is registered _without_
// attempting to load the type from a shared object/dll.  pederb.
//
// FIXME: replace this with a real set datatype abstraction. 20050524 mortene.
typedef SbHash<void *, const char *> NameSet;
static NameSet * soupgrader_namedict = NULL;

static void
soupgrader_cleanup(void)
{
  delete soupgrader_namedict;
  soupgrader_namedict = NULL;
}

// add a name to the upgrader lookup dict.
static void 
soupgrader_add_to_namedict(const SbString & name)
{
  assert(soupgrader_namedict);
  soupgrader_namedict->put(name.getString(), NULL);

  // create lookup both with and without the "So" prefix
  SbString tmp;
  if (name.compareSubString("So") == 0) {
    tmp = name.getSubString(2);
  }
  else {
    tmp = "So";
    tmp += name;
  }
  soupgrader_namedict->put(tmp.getString(), NULL);
}

static SbBool 
soupgrader_exists(const SbString & name) 
{
  assert(soupgrader_namedict);
  void * dummy;
  return soupgrader_namedict->get(name.getString(), dummy);
}

#define SOUPGRADER_ADD_TYPE(_class_) \
  _class_::initClass(); \
  soupgrader_add_to_namedict(SO__QUOTE(_class_))

//
// init all upgradable classes. Can be called multiple times.
//
static void 
soupgrader_init_classes(void)
{
  static int first = 1;
  if (first) {
    first = 0;
    soupgrader_namedict = new NameSet;
    
    coin_atexit((coin_atexit_f*) soupgrader_cleanup, 0); 
    
    SOUPGRADER_ADD_TYPE(SoPackedColorV20);
    SOUPGRADER_ADD_TYPE(SoShapeHintsV10);
  }
}

/*!
  Try creating a node of name \a name with Inventor version \a ivversion.
  
  Returns NULL if no such node exists.
*/
SoBase * 
SoUpgrader::tryCreateNode(const SbName & name, const float ivversion)
{
  if (ivversion == 1.0f) {
    soupgrader_init_classes();
    
    SbString s(name.getString());
    s += "V10";

    if (soupgrader_exists(s.getString())) {
      SoType type = SoType::fromName(SbName(s.getString()));
      if (type.canCreateInstance()) {
        return (SoBase*) type.createInstance();
      }
    }
  }
  else if (ivversion == 2.0f) {
    soupgrader_init_classes();
    
    SbString s(name.getString());
    s += "V20";

    if (soupgrader_exists(s.getString())) {
      SoType type = SoType::fromName(SbName(s.getString()));
      if (type.canCreateInstance()) {
        return (SoBase*) type.createInstance();
      }
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
  else if (type == SoShapeHintsV10::getClassTypeId()) {
    SoShapeHintsV10 * pp = (SoShapeHintsV10*) base;
    return (SoBase*) pp->createUpgrade();
  }
  else {
    SoDebugError::post("SoUpgrader::createUpgrade",
                       "No upgrade functionality available for %s",
                       type.getName().getString());
  }
  return NULL;
}

#undef SOUPGRADER_ADD_TYPE

