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
  \class SoNodekitCatalog SoNodekitCatalog.h Inventor/nodekits/SoNodekitCatalog.h
  \brief The SoNodekitCatalog class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/SbName.h>
#include <assert.h>

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::initClass(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoNodekitCatalog::SoNodekitCatalog(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoNodekitCatalog::~SoNodekitCatalog()
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getNumEntries(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getPartNumber(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
const SbName &
SoNodekitCatalog::getName(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SbName name;
  return name;
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getType(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getType(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getDefaultType(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getDefaultType(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isNullByDefault(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isNullByDefault(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isLeaf(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isLeaf(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
const SbName &
SoNodekitCatalog::getParentName(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SbName name;
  return name;
}

/*!
  FIXME: write function documentation
*/
const SbName &
SoNodekitCatalog::getParentName(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SbName name;
  return name;
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getParentPartNumber(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getParentPartNumber(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
const SbName &
SoNodekitCatalog::getRightSiblingName(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SbName name;
  return name;
}

/*!
  FIXME: write function documentation
*/
const SbName &
SoNodekitCatalog::getRightSiblingName(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SbName name;
  return name;
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getRightSiblingPartNumber(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
int
SoNodekitCatalog::getRightSiblingPartNumber(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isList(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isList(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getListContainerType(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodekitCatalog::getListContainerType(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
const SoTypeList &
SoNodekitCatalog::getListItemTypes(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SoTypeList l;
  return l;
}

/*!
  FIXME: write function documentation
*/
const SoTypeList &
SoNodekitCatalog::getListItemTypes(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  static SoTypeList l;
  return l;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isPublic(int /*part*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::isPublic(const SbName & /*name*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SoNodekitCatalog *
SoNodekitCatalog::clone(SoType /*type*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::addEntry(const SbName & /*name*/, SoType /*type*/,
			   SoType /*defaulttype*/, SbBool /*isdefaultnull*/,
			   const SbName & /*parent*/, const SbName & /*rightsibling*/,
			   SbBool /*islist*/, SoType /*listcontainertype*/,
			   SoType /*listitemtype*/, SbBool /*ispublic*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::addListItemType(int /*part*/, SoType /*type*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::addListItemType(const SbName & /*name*/, SoType /*type*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::narrowTypes(const SbName & /*name*/,
			      SoType /*newtype*/, SoType /*newdefaulttype*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::setNullByDefault(const SbName & /*name*/, SbBool /*nullbydefault*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodekitCatalog::recursiveSearch(int /*part*/, const SbName & /*name*/,
				  SoTypeList * /*checked*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodekitCatalog::printCheck(void) const
{
  assert(0 && "FIXME: not implemented yet");
}
