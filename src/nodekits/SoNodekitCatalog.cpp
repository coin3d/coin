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
  \brief The SoNodekitCatalog class is a container for nodekit layouts.
  \ingroup nodekits

  Nodekits store all their hierarchical layout information and part
  information in instances of this class.

  \sa SoNodeKit, SoBaseKit
*/

#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/SbName.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <stdio.h> // fprintf()


// Private container class.
class CatalogItem {
public:
  CatalogItem(void) { }
  ~CatalogItem() { }

  SbName name, parentname, siblingname;
  SoType type, defaulttype, containertype;
  SbBool isdefaultnull, islist, ispublic;
  SoTypeList itemtypeslist;
};

/*!
  Initialization of static variables.
*/
void
SoNodekitCatalog::initClass(void)
{
}

/*!
  Constructor.
*/
SoNodekitCatalog::SoNodekitCatalog(void)
{
}

/*!
  Destructor.
*/
SoNodekitCatalog::~SoNodekitCatalog()
{
  for (int i=0; i < this->items.getLength(); i++) delete this->items[i];
}

/*!
  Returns total number of entries in the catalog.
*/
int
SoNodekitCatalog::getNumEntries(void) const
{
  return this->items.getLength();
}

/*!
  Returns part number in catalog with \a name. If part exists with that name,
  returns SO_CATALOG_NAME_NOT_FOUND.
*/
int
SoNodekitCatalog::getPartNumber(const SbName & name) const
{
  int nritems = this->getNumEntries();
  for (int i=0; i < nritems; i++) {
    if (name == this->items[i]->name) return i;
  }
  return SO_CATALOG_NAME_NOT_FOUND;
}

/*!
  Given the \a part number, return name of that part.
*/
const SbName &
SoNodekitCatalog::getName(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getName",
		       "invalid part number, %d", part);
    static SbName name;
    return name;
  }
#endif // debug
  
  return this->items[part]->name;
}

/*!
  Given the \a part number, return type.
*/
SoType
SoNodekitCatalog::getType(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getType",
		       "invalid part number, %d", part);
    return SoType::badType();
  }
#endif // debug
  
  return this->items[part]->type;
}

/*!
  Given the part \a name, return type.
*/
SoType
SoNodekitCatalog::getType(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getType",
		       "invalid part name, \"%s\"", name.getString());
    return SoType::badType();
  }
#endif // debug
  return this->items[part]->type;
}

/*!
  Given \a part number, return default type of part.
*/
SoType
SoNodekitCatalog::getDefaultType(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getDefaultType",
		       "invalid part number, %d", part);
    return SoType::badType();
  }
#endif // debug
  
  return this->items[part]->defaulttype;
}

/*!
  Given part \a name, return default type of part.
*/
SoType
SoNodekitCatalog::getDefaultType(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getDefaultType",
		       "invalid part name, \"%s\"", name.getString());
    return SoType::badType();
  }
#endif // debug
  return this->items[part]->defaulttype;
}

/*!
  Returns \c TRUE if the \a part is empty by default, otherwise \c FALSE.
*/
SbBool
SoNodekitCatalog::isNullByDefault(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::isNullByDefault",
		       "invalid part number, %d", part);
    return TRUE;
  }
#endif // debug
  
  return this->items[part]->isdefaultnull;
}

/*!
  Returns \c TRUE if part \name is empty by default, otherwise \c FALSE.
*/
SbBool
SoNodekitCatalog::isNullByDefault(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::isNullByDefault",
		       "invalid part name, \"%s\"", name.getString());
    return TRUE;
  }
#endif // debug
  return this->items[part]->isdefaultnull;
}

/*!
  Returns \c TRUE if the \a part is \e not a parent for any
  other parts in the nodekit catalog.
*/
SbBool
SoNodekitCatalog::isLeaf(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::isLeaf",
		       "invalid part number, %d", part);
    return TRUE;
  }
#endif // debug
  for (int i=0; i < this->items.getLength(); i++) {
    if ((i != part) && (this->items[part]->name == this->items[i]->name))
      return FALSE;
  }
  return TRUE;
}

/*!
  Returns \c TRUE if the part \a name is \e not a parent for any
  other parts in the nodekit catalog.
*/
SbBool
SoNodekitCatalog::isLeaf(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::isLeaf",
		       "invalid part name, \"%s\"", name.getString());
    return TRUE;
  }
#endif // debug
  return this->isLeaf(part);
}

/*!
  Returns name of parent of \a part. If \a part doesn't have a parent,
  the empty string is returned.
*/
const SbName &
SoNodekitCatalog::getParentName(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getParentName",
		       "invalid part number, %d", part);
    static SbName name;
    return name;
  }
#endif // debug
  return this->items[part]->parentname;
}

/*!
  Returns name of parent of the part. If \a name doesn't have a parent,
  the empty string is returned.
*/
const SbName &
SoNodekitCatalog::getParentName(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getParentName",
		       "invalid part name, \"%s\"", name.getString());
    static SbName name;
    return name;
  }
#endif // debug
  return this->getParentName(part);
}

/*!
  Returns part number of given part's parent. If \a part doesn't have a parent,
  SO_CATALOG_NAME_NOT_FOUND is returned.
*/
int
SoNodekitCatalog::getParentPartNumber(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getParentPartNumber",
		       "invalid part number, %d", part);
    return SO_CATALOG_NAME_NOT_FOUND;
  }
#endif // debug
  return this->getPartNumber(this->items[part]->parentname);
}

/*!
  Returns part number of given part's parent. If \a name doesn't have a parent,
  SO_CATALOG_NAME_NOT_FOUND is returned.
*/
int
SoNodekitCatalog::getParentPartNumber(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getParentPartNumber",
		       "invalid part name, \"%s\"", name.getString());
    return part;
  }
#endif // debug
  return this->getParentPartNumber(part);
}

/*!
  Returns name of right sibling of \a part. Returns the empty string if
  \a part doesn't have a right sibling.
*/
const SbName &
SoNodekitCatalog::getRightSiblingName(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getRightSiblingName",
		       "invalid part number, %d", part);
    static SbName name;
    return name;
  }
#endif // debug
  return this->items[part]->siblingname;
}

/*!
  Returns name of sibling of the part. Returns the empty string if
  \a name doesn't have a right sibling.
*/
const SbName &
SoNodekitCatalog::getRightSiblingName(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getRightSiblingName",
		       "invalid part name, \"%s\"", name.getString());
    static SbName name;
    return name;
  }
#endif // debug
  return this->getRightSiblingName(part);
}

/*!
  Returns part number of given part's sibling. Returns 
  SO_CATALOG_NAME_NOT_FOUND if \a part doesn't have a right sibling.
*/
int
SoNodekitCatalog::getRightSiblingPartNumber(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getRightSiblingPartNumber",
		       "invalid part number, %d", part);
    return SO_CATALOG_NAME_NOT_FOUND;
  }
#endif // debug
  return this->getPartNumber(this->items[part]->siblingname);
}

/*!
  Returns part number of given part's right sibling. Returns 
  SO_CATALOG_NAME_NOT_FOUND if part doesn't have a right sibling.
*/
int
SoNodekitCatalog::getRightSiblingPartNumber(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getRightSiblingPartNumber",
		       "invalid part name, \"%s\"", name.getString());
    return part;
  }
#endif // debug
  return this->getRightSiblingPartNumber(part);
}

/*!
  Returns \c TRUE if the given \a part is a list container.
*/
SbBool
SoNodekitCatalog::isList(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::isList",
		       "invalid part number, %d", part);
    return FALSE;
  }
#endif // debug
  return this->items[part]->islist;
}

/*!
  Returns \c TRUE if the given part is a list container.
*/
SbBool
SoNodekitCatalog::isList(const SbName & name) const
{ 
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::isList",
		       "invalid part name, \"%s\"", name.getString());
    return FALSE;
  }
#endif // debug
  return this->isList(part);
}

/*!
  Returns type of list container (SoGroup, SoSeparator, SoSwitch, etc)
  which \a part is.
*/
SoType
SoNodekitCatalog::getListContainerType(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getListContainerType",
		       "invalid part number, %d", part);
    return SoType::badType();
  }
  else if (!this->items[part]->islist) {
    SoDebugError::post("SoNodekitCatalog::getListContainerType",
		       "part %d is not a list container", part);
    return SoType::badType();
  }
#endif // debug
  return this->items[part]->containertype;
}

/*!
  Returns type of list container (SoGroup, SoSeparator, SoSwitch, etc)
  which the named part is.
*/
SoType
SoNodekitCatalog::getListContainerType(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getListContainerType",
		       "invalid part name, \"%s\"", name.getString());
    return SoType::badType();
  }
#endif // debug
  return this->getListContainerType(part);
}

/*!
  Returns list of node types which are allowed to be children of the
  list container \a part.
*/
const SoTypeList &
SoNodekitCatalog::getListItemTypes(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::getListItemTypes",
		       "invalid part number, %d", part);
    static SoTypeList l;
    return l;
  }
  else if (!this->items[part]->islist) {
    SoDebugError::post("SoNodekitCatalog::getListItemTypes",
		       "part %d is not a list container", part);
    static SoTypeList l;
    return l;
  }
#endif // debug
  return this->items[part]->itemtypeslist;
}

/*!
  Returns list of node types which are allowed to be children of the
  named list container part.
*/
const SoTypeList &
SoNodekitCatalog::getListItemTypes(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::getListItemTypes",
		       "invalid part name, \"%s\"", name.getString());
    static SoTypeList l;
    return l;
  }
#endif // debug
  return this->getListItemTypes(part);
}

/*!
  Returns \c TRUE if \a part is visible and publicly available for
  queries and modifications, \c FALSE if \a part is hidden.
*/
SbBool
SoNodekitCatalog::isPublic(int part) const
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::isPublic",
		       "invalid part number, %d", part);
    return FALSE;
  }
#endif // debug
  return this->items[part]->ispublic;
}

/*!
  Returns \c TRUE if the part is visible and publicly available for
  queries and modifications, \c FALSE if it is hidden.
*/
SbBool
SoNodekitCatalog::isPublic(const SbName & name) const
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::isPublic",
		       "invalid part name, \"%s\"", name.getString());
    return FALSE;
  }
#endif // debug
  return this->isPublic(part);
}

/*!
  Return a clone of this catalog. \a type will be used to set the type
  and defaulttype values of the toplevel \c this entry.
*/
SoNodekitCatalog *
SoNodekitCatalog::clone(SoType type) const
{
  SoNodekitCatalog * newcat = new SoNodekitCatalog;
  for (int i=0; i < this->items.getLength(); i++) {
    CatalogItem * olditem = this->items[i];
    CatalogItem * newitem = new CatalogItem(*olditem);
    if (i == 0) {
      newitem->type = type;
      newitem->defaulttype = type;
    }
    // This is the only element in CatalogItem which can't be bitwise
    // copied.
    newitem->itemtypeslist = olditem->itemtypeslist;
    newcat->items.append(newitem);
  }
  return newcat;
}

/*!
  Add a new entry to the catalog. Returns \c TRUE if add was ok.
*/
SbBool
SoNodekitCatalog::addEntry(const SbName & name, SoType type,
			   SoType defaulttype, SbBool isdefaultnull,
			   const SbName & parent, const SbName & rightsibling,
			   SbBool islist, SoType listcontainertype,
			   SoType listitemtype, SbBool ispublic)
{

  // The elements of a nodekit catalog is conceptually ordered like a
  // tree, but implementation-wise we stuff them inside a list. This
  // will make it speedier to access the elements through preset part
  // number indices.
  //
  // The list is in the same order which we would get by continually
  // appending elements through a prefix traversal of the tree.

#if COIN_DEBUG // debug
  if (name == "") {
    SoDebugError::post("SoNodekitCatalog::addEntry", "empty name not allowed");
    return FALSE;
  }
  else if (this->getPartNumber(name) != SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::addEntry",
		       "partname \"%s\" already in use", name.getString());
    return FALSE;
  }
  else if (rightsibling != "" &&
	   this->getPartNumber(rightsibling) == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::addEntry",
		       "right sibling \"%s\" not found", name.getString());
    return FALSE;
  }
  else if (parent != "" &&
	   this->getPartNumber(parent) == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::addEntry",
		       "parent \"%s\" not found", name.getString());
    return FALSE;
  }
  else if (parent == "" && this->getNumEntries() > 0) {
    SoDebugError::post("SoNodekitCatalog::addEntry", "we need a parent name");
    return FALSE;
  }
  else if ((type == SoType::badType()) || (defaulttype == SoType::badType()) ||
	   (islist && ((listcontainertype == SoType::badType()) ||
		       (listitemtype == SoType::badType())))) {
    SoDebugError::post("SoNodekitCatalog::addEntry", "bad type");
    return FALSE;
  }
#endif // debug

#if 0 // debug
  SoDebugError::postInfo("SoNodekitCatalog::addEntry",
			 "new entry: ``%s''", name.getString());
#endif // debug

  CatalogItem * newitem = new CatalogItem;
  newitem->name = name;
  newitem->type = type;
  newitem->defaulttype = defaulttype;
  newitem->isdefaultnull = isdefaultnull;
  newitem->parentname = parent;
  newitem->siblingname = rightsibling;
  newitem->islist = islist;
  newitem->containertype = listcontainertype;
  newitem->itemtypeslist.append(listitemtype);
  newitem->ispublic = ispublic;

  // First item in catalog?
  if (this->items.getLength() == 0) {
#if COIN_DEBUG
    if (newitem->parentname != "" || newitem->siblingname != "") {
      SoDebugError::post("SoNodekitCatalog::addEntry",
			 "first item must be the toplevel parent");
      return FALSE;
    }
#endif // COIN_DEBUG
    this->items.append(newitem);
    return TRUE;
  }

  int position = 0;

  // First find parent.
  while (this->items[position]->name != newitem->parentname) position++;

#if 0 // debug
  SoDebugError::postInfo("SoNodekitCatalog::addEntry",
			 "parent position: %d", position);
#endif // debug

  // See if we're only child so far..
  position++;
  if (position == this->items.getLength()) {
    this->items.append(newitem);
    return TRUE;
  }
  else {
    CatalogItem * next = this->items[position];
    if (next->parentname != newitem->parentname) {
#if COIN_DEBUG
      if (newitem->siblingname != "") {
	SoDebugError::post("SoNodekitCatalog::addEntry",
			   "%s is not a child of %s",
			   newitem->siblingname.getString(),
			   newitem->parentname.getString());
	return FALSE;
      }
#endif // COIN_DEBUG
      this->items.insert(newitem, position);
      return TRUE;
    }
  }

  // Find correct position among the siblings.
  while (position < this->items.getLength() &&
	 this->items[position]->name != newitem->siblingname &&
	 this->items[position]->parentname == newitem->parentname) position++;

  // Then insert in catalog.

  CatalogItem * prev = this->items[position-1];
  if (prev->parentname == newitem->parentname)
    prev->siblingname = newitem->name;

#if 0 // debug
  SoDebugError::postInfo("SoNodekitCatalog::addEntry",
			 "item position: %d", position);
#endif // debug

  if (position == this->items.getLength()) this->items.append(newitem);
  else this->items.insert(newitem, position);
  return TRUE;
}

/*!
  Add another allowable type for the given \a part. \a part must of course
  be a list container item.
*/
void
SoNodekitCatalog::addListItemType(int part, SoType type)
{
#if COIN_DEBUG // debug
  if (part < 0 || part >= this->getNumEntries()) {
    SoDebugError::post("SoNodekitCatalog::addListItemType",
		       "invalid part number, %d", part);
    return;
  }
  else if (type == SoType::badType()) {
    SoDebugError::post("SoNodekitCatalog::addListItemType",
		       "tried to add SoType::badType()");
    return;
  }
  else if (!this->items[part]->islist) {
    SoDebugError::post("SoNodekitCatalog::addListItemType",
		       "tried to add a listitemtype to non-list item");
    return;
  }
  else if (this->items[part]->itemtypeslist.find(type) != -1) {
    SoDebugError::post("SoNodekitCatalog::addListItemType",
		       "type ``%s''already present in list",
		       type.getName().getString());
    return;
  }
#endif // debug
  this->items[part]->itemtypeslist.append(type);
}

/*!
  Add another allowable type for the \a name part. The part must of course
  be a list container.
*/
void
SoNodekitCatalog::addListItemType(const SbName & name, SoType type)
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::addListItemType",
		       "invalid part name, \"%s\"", name.getString());
    return;
  }
#endif // debug
  this->addListItemType(part, type);
}

/*!
  Set the type and default type of a part to be subtypes of the old
  types. Useful for "narrowing" the specification of a nodekit which
  inherits the catalog of a more generic nodekit superclass.
*/
void
SoNodekitCatalog::narrowTypes(const SbName & name,
			      SoType newtype, SoType newdefaulttype)
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::narrowTypes",
		       "invalid part name, \"%s\"", name.getString());
    return;
  }
  else if ((!newtype.isDerivedFrom(this->items[part]->type)) ||
	   (!newdefaulttype.isDerivedFrom(this->items[part]->defaulttype))) {
    SoDebugError::post("SoNodekitCatalog::narrowTypes",
		       "the new types are not derived from the old types");
    return;
  }
#endif // debug
  this->items[part]->type = newtype;
  this->items[part]->defaulttype = newdefaulttype;
}

/*!
  Change whether or not the part with the given \a name is created by
  default.
*/
void
SoNodekitCatalog::setNullByDefault(const SbName & name, SbBool nullbydefault)
{
  int part = this->getPartNumber(name);
#if COIN_DEBUG // debug
  if (part == SO_CATALOG_NAME_NOT_FOUND) {
    SoDebugError::post("SoNodekitCatalog::setNullByDefault",
		       "invalid part name, \"%s\"", name.getString());
    return;
  }
#endif // debug
  this->items[part]->isdefaultnull = nullbydefault;
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
  Lists all catalog parts, which is useful for debugging. 
*/
void
SoNodekitCatalog::printCheck(void) const
{
  int nritems = this->getNumEntries();
  fprintf(stdout, "catalog printout: number of entries = %d\n", nritems);
  for (int i=0; i < nritems; i++) {
    CatalogItem * item = this->items[i];

    fprintf(stdout,
	    "#%d\n"
	    "    name = %s, type = %s, defaultType = %s\n"
	    "    nullByDefault = %d\n"
	    "    parentName = %s\n"
	    "    sibling = %s, listPart = %d\n",
	    i, item->name.getString(),
	    item->type == SoType::badType() ? "*bad*" : item->type.getName().getString(),
	    item->defaulttype == SoType::badType() ? "*bad*" : item->defaulttype.getName().getString(),
	    item->isdefaultnull, item->parentname.getString(),
	    item->siblingname.getString(), item->islist);

    if (item->islist) {
      fprintf(stdout, "listItemTypes =");
      for (int j=0; j < item->itemtypeslist.getLength(); j++) {
	fprintf(stdout, " %s", item->itemtypeslist[j].getName().getString());
      }
      fprintf(stdout, "\n");
    }

    fprintf(stdout, "    publicPart = %d\n", item->ispublic);
  }
}
