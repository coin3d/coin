/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOBASEKIT_H
#define COIN_SOBASEKIT_H

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodes/SoNode.h>
#ifdef COIN_INTERNAL
class SoNodeKitPath;
#else // !COIN_INTERNAL
// Include this header file for better Open Inventor compatibility.
#include <Inventor/SoNodeKitPath.h>
#endif // !COIN_INTERNAL

class SoGroup;
class SoNodekitParts;
class SoNodekitCatalog;
class SoPath;

// Convenience macros. FIXME: document. 20000113 mortene.

#define SO_GET_PART(_kit_, _name_, _classname_) \
 ((_classname_ *)_kit_->getPart(_name_, TRUE))

#define SO_CHECK_PART(_kit_, _name_, _classname_) \
 ((_classname_ *)_kit_->getPart(_name_, FALSE))

#define SO_GET_ANY_PART(_kit_, _name_, _classname_) \
 ((_classname_ *)_kit_->getAnyPart(_name_, TRUE, FALSE, FALSE))

#define SO_CHECK_ANY_PART(_kit_, _name_, _classname_) \
 ((_classname_ *)_kit_->getAnyPart(_name_, FALSE, FALSE, FALSE))


class COIN_DLL_EXPORT SoBaseKit : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoBaseKit);

  SO_KIT_CATALOG_ENTRY_HEADER(callbackList);

public:
  static void initClass(void);
  SoBaseKit(void);

  static const SoNodekitCatalog * getClassNodekitCatalog(void);
  virtual const SoNodekitCatalog * getNodekitCatalog(void) const;

  virtual SoNode * getPart(const SbName & partname, SbBool makeifneeded);
  SbString getPartString(const SoBase * part);
  virtual SoNodeKitPath * createPathToPart(const SbName & partname,
                                           SbBool makeifneeded,
                                           const SoPath * pathtoextend = NULL);
  virtual SbBool setPart(const SbName & partname, SoNode * from);
  SbBool set(const char * namevaluepairliststring);
  SbBool set(const char * partnamestring, const char * parameterstring);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual SoChildList * getChildren(void) const;

  void printDiagram(void);
  void printSubDiagram(const SbName & rootname, int level);
  void printTable(void);

  virtual void addWriteReference(SoOutput * out, SbBool isfromfield = FALSE);
  SbBool forceChildDrivenWriteRefs(SoOutput * out);

  static SbBool isSearchingChildren(void);
  static void setSearchingChildren(const SbBool newval);
  static SoNode * typeCheck(const SbName & partname, const SoType & parttype,
                            SoNode * node);


protected:
  virtual ~SoBaseKit();

  static const SoNodekitCatalog ** getClassNodekitCatalogPtr(void);

  virtual SoNode * addToCopyDict(void) const;
  virtual void copyContents(const SoFieldContainer * fromfc,
                            SbBool copyconnections);

  SoGroup * getContainerNode(const SbName & listname,
                             SbBool makeifneeded = TRUE);
  virtual SoNode * getAnyPart(const SbName & partname, SbBool makeifneeded,
                              SbBool leafcheck = FALSE,
                              SbBool publiccheck = FALSE);
  virtual SoNodeKitPath * createPathToAnyPart(const SbName & partname,
                                              SbBool makeifneeded,
                                              SbBool leafcheck = FALSE,
                                              SbBool publiccheck = FALSE,
                                              const SoPath * pathtoextend = NULL);
  virtual SbBool setAnyPart(const SbName & partname, SoNode * from,
                            SbBool anypart = TRUE);
  void createNodekitPartsList(void); // not part of Coin
  void createFieldList(void); // replaces above method
  virtual void createDefaultParts(void);
  const SoNodekitParts * getNodekitPartsList(void) const; // not part of Coin

  void catalogError(void);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void setDefaultOnNonWritingFields(void);
  void countMyFields(SoOutput * out);

  virtual SbBool setPart(const int partNum, SoNode * node);

  SoChildList * children;
  SbBool connectionsSetUp;

private:
  friend class SoInteractionKit;
  static SbBool findPart(const SbString & partname, SoBaseKit *& kit,
                         int & partnum, SbBool & islist, int & listidx,
                         const SbBool makeifneeded, SoPath * path = NULL,
                         const SbBool recsearch = FALSE);

  SbBool makePart(const int partnum);
  int getRightSiblingIndex(const int partnum);
  int findNodeInThisKit(SoNode *node, const int parentnum = -1) const;

  static SoNodekitCatalog * classcatalog;
  static const SoNodekitCatalog ** parentcatalogptr;
  static SbBool searchchildren;

  int numCatalogEntries;
  SoSFNode ** fieldList;
};

#endif // !COIN_SOBASEKIT_H
