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

#ifndef __SONODEKITCATALOG_H__
#define __SONODEKITCATALOG_H__

#include <Inventor/SoType.h>
#include <Inventor/lists/SbList.h>

class SbName;


#define SO_CATALOG_NAME_NOT_FOUND -1


class SoNodekitCatalog
{
public:
  static void initClass(void);

  SoNodekitCatalog(void);
  ~SoNodekitCatalog();

  int getNumEntries(void) const;
  int getPartNumber(const SbName & name) const;
  const SbName & getName(int part) const;

  SoType getType(int part) const;
  SoType getType(const SbName & name) const;
  SoType getDefaultType(int part) const;
  SoType getDefaultType(const SbName & name) const;
  SbBool isNullByDefault(int part) const;
  SbBool isNullByDefault(const SbName & name) const;

  SbBool isLeaf(int part) const;
  SbBool isLeaf(const SbName & name) const;
  const SbName & getParentName(int part) const;
  const SbName & getParentName(const SbName & name) const;
  int getParentPartNumber(int part) const;
  int getParentPartNumber(const SbName & name) const;
  const SbName & getRightSiblingName(int part) const;
  const SbName & getRightSiblingName(const SbName & name) const;
  int getRightSiblingPartNumber(int part) const;
  int getRightSiblingPartNumber(const SbName & name) const;

  SbBool isList(int part) const;
  SbBool isList(const SbName & name) const;
  SoType getListContainerType(int part) const;
  SoType getListContainerType(const SbName & name) const;
  const SoTypeList & getListItemTypes(int part) const;
  const SoTypeList & getListItemTypes(const SbName & name) const;

  SbBool isPublic(int part) const;
  SbBool isPublic(const SbName & name) const;

  SoNodekitCatalog * clone(SoType type) const;

  SbBool addEntry(const SbName & name, SoType type, SoType defaulttype,
                  SbBool isdefaultnull, const SbName & parent,
                  const SbName & rightsibling, SbBool islist,
                  SoType listcontainertype, SoType listitemtype,
                  SbBool ispublic);
  void addListItemType(int part, SoType type);
  void addListItemType(const SbName & name, SoType type);

  void narrowTypes(const SbName & name, SoType newtype, SoType newdefaulttype);
  void setNullByDefault(const SbName & name, SbBool nullbydefault);

  SbBool recursiveSearch(int part, const SbName & name,
                         SoTypeList * checked) const;

  void printCheck(void) const;

private:
  SbList<class CatalogItem *> items;
};

#endif // !__SONODEKITCATALOG_H__
