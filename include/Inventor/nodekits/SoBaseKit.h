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

#ifndef __SOBASEKIT_H__
#define __SOBASEKIT_H__

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodes/SoNode.h>

class SoGroup;
class SoNodekitParts;
class SoNodeKitPath;
class SoNodekitCatalog;
class SoPath;

// Convenience macros. FIXME: not implemented yet.
#define SO_GET_PART(kit, name, classname)
#define SO_CHECK_PART(kit, name, classname)
#define SO_GET_ANY_PART(kit, name, classname)
#define SO_CHECK_ANY_PART(kit, name, classname)


class SoBaseKit : public SoNode
{
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

  SoChildList * children;
  SbBool connectionsSetUp;

private:
  static SbBool findPart(const SbString & partName, SoBaseKit *& kit,
                         int & partNum, SbBool & isList, int & listIdx,
                         const SbBool makeIfNeeded, SoPath * path = NULL);

  SbBool makePart(const int partNum);
  SbBool setPart(const int partNum, SoNode * node);
  int getRightSiblingIndex(const int partNum);

  static SoNodekitCatalog * classcatalog;
  static const SoNodekitCatalog ** parentcatalogptr;
  static SbBool searchchildren;

  int numCatalogEntries;
  SoSFNode ** fieldList;
};

#endif // !__SOBASEKIT_H__
