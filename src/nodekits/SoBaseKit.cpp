/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoBaseKit SoBaseKit.h Inventor/nodekits/SoBaseKit.h
  \brief The SoBaseKit class is the toplevel superclass for nodekits.
  \ingroup nodekits

  FIXME, write class doc:

  * what are nodekits?
  * why nodekits?
  * how to use them
  * how to define your own
  * ...

*/

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/SoInput.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SbBool SoBaseKit::searchchildren = FALSE;

SO_KIT_SOURCE(SoBaseKit);

/*!
  \fn const SoNodekitCatalog * SoBaseKit::getClassNodekitCatalog(void)
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/

/*!
  \fn const SoNodekitCatalog * SoBaseKit::getNodekitCatalog(void) const
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/

/*!
  \fn const SoNodekitCatalog ** SoBaseKit::getClassNodekitCatalogPtr(void)
  Returns the pointer to the pointer of the nodekit catalog
  for this class.
*/


/*!
  Constructor.
*/
SoBaseKit::SoBaseKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoBaseKit);

  // Can't use ADD_CATALOG_ENTRY macro for the toplevel "this" entry,
  // as we don't want to call SO_NODE_ADD_FIELD(). This is how the
  // invocation would have looked if we could use the macro:
  //
  // SO_KIT_ADD_CATALOG_ENTRY(this, SoBaseKit, TRUE, "", "", FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoBaseKit::classcatalog->addEntry("this",
                                      SoBaseKit::getClassTypeId(),
                                      SoBaseKit::getClassTypeId(),
                                      TRUE,
                                      "",
                                      "",
                                      FALSE,
                                      SoType::badType(),
                                      SoType::badType(),
                                      FALSE);
  }


  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_LIST_ENTRY(callbackList, SoSeparator, TRUE, this, "", SoCallback, TRUE);
  SO_KIT_ADD_LIST_ITEM_TYPE(callbackList, SoEventCallback);

  // this could be created on demand, but will make it more complicated
  this->children = new SoChildList(this);

  this->fieldList = NULL;
  this->numCatalogEntries = 0;
  this->connectionsSetUp = FALSE;
  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoBaseKit::~SoBaseKit()
{
  delete [] this->fieldList;
  delete this->children;
}

/*!
  Does initialization common for all objects of the
  SoBaseKit class. This includes setting up the
  type system, among other things.
*/
void
SoBaseKit::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBaseKit);
}

/*!
  Returns a pointer to the node part with \a partname.

  If the part is not in the nodekit's catalog, return NULL.

  If the part is in the catalog, has not been made and \a makeifneeded
  is TRUE, construct the part and all it's parents, and return the
  node pointer. If the node part has not been made and \a makeifneeded
  is FALSE, return NULL.

  FIXME: describe syntax for specifiying "paths". 19991205 mortene.
*/
SoNode *
SoBaseKit::getPart(const SbName & partname, SbBool makeifneeded)
{
  return this->getAnyPart(partname, makeifneeded, TRUE, TRUE);
}

/*!
  FIXME: write function documentation
*/
SbString
SoBaseKit::getPartString(const SoBase *part)
{
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  if (part->isOfType(SoNode::getClassTypeId())) {
    int idx = this->findNodeInThisKit((SoNode*)part);
    if (idx >= 0) {
      return SbString(catalog->getName(idx).getString());
    }
    return SbString();
  }
  else if (part->isOfType(SoPath::getClassTypeId())) {
    SoFullPath *path = (SoFullPath*)part;
    int pathidx = path->findNode(this);
    if (pathidx < 0) return SbString();
    SoBaseKit *kit = this;
    SbString partname;
    int parentnum = 0;
    SoNode *tail = path->getTail();
    SoNode *node = kit;
    while (node != tail) {
      node = path->getNode(++pathidx);
      int partnum = kit->findNodeInThisKit(node, parentnum);
      if (partnum < 0) {
#if COIN_DEBUG && 1 // debug
        SoDebugError::postInfo("SoBaseKit::getPartString",
                               "Illegal path");
#endif // debug
        return SbString();
      }
      // FIXME: not quite sure if this test is sufficient, pederb 2000-02-01
      if (catalog->isLeaf(partnum)) {
        if (partname != "") partname += '.';
        partname += catalog->getName(partnum).getString();
      }
      if (node->isOfType(SoNodeKitListPart::getClassTypeId())) {
        // no sense in using SoNodeKitListPart as a non-leaf node, right?
        assert(catalog->isLeaf(partnum));
        SoNodeKitListPart *list = (SoNodeKitListPart*)node;
        pathidx += 2; // // skip container node
        if (pathidx >= path->getLength()) {
#if COIN_DEBUG && 1 // debug
          SoDebugError::postInfo("SoBaseKit::getPartString",
                                 "Path too short");
#endif // debug
          return SbString();
        }
        node = path->getNode(pathidx);
        int childidx = list->findChild(node);
        assert(childidx >= 0);
        partname += '[';
        partname.addIntString(childidx);
        partname += ']';
      }
      if (node->isOfType(SoBaseKit::getClassTypeId())) {
        kit = (SoBaseKit*) node;
        catalog = kit->getNodekitCatalog();
        parentnum = 0;
      }
      else {
        // search more in this kit
        parentnum = partnum;
      }
    }
    return partname;
  }
  return SbString();
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToPart(const SbName &partname, SbBool makeifneeded, const SoPath *pathtoextend)
{
  return this->createPathToAnyPart(partname, makeifneeded, TRUE, TRUE, pathtoextend);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setPart(const SbName &partname, SoNode *from)
{
  return this->setAnyPart(partname, from, FALSE);
}

static const char *
skip_spaces(const char *ptr)
{
  while (isspace(*ptr)) ptr++;
  return ptr;
}

static int
find_partname_length(const char *ptr)
{
  int cnt = 0;
  while (ptr[cnt] && !isspace(ptr[cnt]) && ptr[cnt] != '{' && ptr[cnt] != '}') cnt++;
  return cnt;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(const char *namevaluepairliststring)
{
  int stringlen = strlen(namevaluepairliststring); // cache this value
  const char *currptr = skip_spaces(namevaluepairliststring);
  SoInput memInput;

  while (*currptr) {
    int partnamelen = find_partname_length(currptr);
    const char *start = skip_spaces(currptr + partnamelen);
    if (*start != '{') { // first non-space after partname should be a {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoBaseKit::set",
                             "parse error at byte %d in input string",
                             start-namevaluepairliststring);
#endif // debug
      return FALSE;
    }
    start++; // skip {
    SbString partname(currptr, 0, partnamelen-1);
    SoBaseKit *kit = this;
    int partNum;
    SbBool isList;
    int listIdx;
    if (!SoBaseKit::findPart(partname, kit, partNum, isList, listIdx, TRUE)) {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoBaseKit::set",
                             "part ``%s'' not found",
                             partname.getString());
#endif // debug
      return FALSE;
    }

    SoNode *node = kit->fieldList[partNum]->getValue();
    if (isList) {
      SoNodeKitListPart *list = (SoNodeKitListPart*)node;
      if (listIdx < 0 || listIdx >= list->getNumChildren()) {
#if COIN_DEBUG && 1 // debug
        SoDebugError::postInfo("SoBaseKit::set",
                               "index %d out of bounds for part ``%s''",
                               listIdx, partname.getString());
#endif // debug
        return FALSE;
      }
      node = list->getChild(listIdx);
    }
    memInput.setBuffer((void*)start, stringlen - (start-namevaluepairliststring));
    SbBool dummy;
    if (!node->getFieldData()->read(&memInput, node, TRUE, dummy)) {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoBaseKit::set",
                             "error while parsing data for part ``%s''",
                             partname.getString());
#endif // debug
      return FALSE;
    }
    currptr = start + (int) memInput.getNumBytesRead();
    if (*currptr == '}') currptr++;
    assert(currptr <= namevaluepairliststring + stringlen);
    currptr = skip_spaces(currptr);
  }
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(const char *partnamestring, const char * parameterstring)
{
  SbString partname(partnamestring);
  int partNum;
  SbBool isList;
  int listIdx;
  SoBaseKit *kit = this;
  if (SoBaseKit::findPart(partname, kit, partNum, isList, listIdx, TRUE)) {
    SoNode *node = kit->fieldList[partNum]->getValue();
    assert(node != NULL); // makeifneeded was TRUE in findPart call
    if (isList) {
      assert(node->isOfType(SoNodeKitListPart::getClassTypeId()));
      SoNodeKitListPart *list = (SoNodeKitListPart*) node;
      if (listIdx < 0 || listIdx >= list->getNumChildren()) {
#if COIN_DEBUG && 1 // debug
        SoDebugError::postInfo("SoBaseKit::set",
                               "index %d out of bounds for part: %s",
                               listIdx, partnamestring);
#endif // debug
        return FALSE;
      }
      else {
        node = list->getChild(listIdx);
      }
    }
    if (node) {
      SoInput memInput;
      SbBool dummy;
      memInput.setBuffer((void*)parameterstring, strlen(parameterstring));
      const SoFieldData *fielddata = node->getFieldData();
      return fielddata->read(&memInput, node, TRUE, dummy);
    }
  }
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::doAction(SoAction *action)
{
  int numIndices;
  const int * indices;
  switch (action->getPathCode(numIndices, indices)) {
  case SoAction::IN_PATH:
    this->children->traverse(action, 0, indices[numIndices - 1]);
    break;
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->children->traverse(action); // traverse all children
    break;
  case SoAction::OFF_PATH:
    {
      SoChildList *children = this->getChildren();
      int n = children->getLength();
      for (int i = 0; i < n; i++) {
        if ((*children)[i]->affectsState())
          children->traverse(action, i);
      }
      break;
    }
  default:
    assert(0 && "Unknown path code");
    break;
  }
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::callback(SoCallbackAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::GLRender(SoGLRenderAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getBoundingBox(SoGetBoundingBoxAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getMatrix(SoGetMatrixAction *action)
{
  int numIndices;
  const int * indices;
  if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH) {
    this->children->traverse(action, 0, indices[numIndices - 1]);
  }
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::handleEvent(SoHandleEventAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::rayPick(SoRayPickAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::search(SoSearchAction *action)
{
  inherited::search(action);
  if (action->isFound() || !this->searchchildren) return;
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::write(SoWriteAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoBaseKit::doAction((SoAction*)action);
}

/*!
  FIXME: write function documentation
*/
SoChildList *
SoBaseKit::getChildren(void) const
{
  return this->children;
}

/*!
  Print out the full nodekit catalog structure. Useful for debugging.
*/
void
SoBaseKit::printDiagram(void)
{
  fprintf(stdout, "CLASS So%s\n", this->getTypeId().getName().getString());
  this->printSubDiagram("this", 0);
}

/*!
  Print out the nodekit catalog structure from \a rootname and downwards
  in the catalog tree, with indentation starting at \a level.
*/
void
SoBaseKit::printSubDiagram(const SbName & rootname, int level)
{
  const SoNodekitCatalog * parentcatalog = NULL;
  if (this->getTypeId() != SoBaseKit::getClassTypeId()) {
    SoType parenttype = this->getTypeId().getParent();
    SoBaseKit * parentobj = (SoBaseKit *)parenttype.createInstance();
    parentcatalog = parentobj->getNodekitCatalog();
    parentobj->ref();
    parentobj->unref();
  }

  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();

  int i = 0;
  if (!parentcatalog ||
      parentcatalog->getPartNumber(rootname) == SO_CATALOG_NAME_NOT_FOUND ||
      parentcatalog->getType(rootname) != thiscat->getType(rootname)) {
    fprintf(stdout, "-->");
    i++;
  }
  for (; i < level+1; i++) fprintf(stdout, "   ");

  fprintf(stdout, "\"%s\"\n", rootname.getString());

  for (int j=0; j < thiscat->getNumEntries(); j++) {
    if (thiscat->getParentName(j) == rootname)
      this->printSubDiagram(thiscat->getName(j), level + 1);
  }
}

/*!
  Write the complete nodekit catalog in table form.
*/
void
SoBaseKit::printTable(void)
{
  fprintf(stdout, "CLASS So%s\n", this->getTypeId().getName().getString());

  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();
  for (int i=0; i < thiscat->getNumEntries(); i++) {
    const SoType t = thiscat->getType(i);
    fprintf(stdout, "%s   \"%s\",  So%s ",
            thiscat->isPublic(i) ? "   " : "PVT",
            thiscat->getName(i).getString(),
            t.getName().getString());
    if (thiscat->isList(i)) {
      SoTypeList tlist = thiscat->getListItemTypes(i);
      fprintf(stdout, "[ ");
      for (int j=0; j < tlist.getLength(); j++) {
        if (j) fprintf(stdout, ", ");
        fprintf(stdout, "So%s", tlist[j].getName().getString());
      }
      fprintf(stdout, " ] ");
    }
    else {
      fprintf(stdout, " --- ");
    }

    if (t != thiscat->getDefaultType(i)) {
      fprintf(stdout, ", (default type = So%s)",
              thiscat->getDefaultType(i).getName().getString());
    }
    fprintf(stdout, "\n");
  }
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::addWriteReference(SoOutput * /*out*/, SbBool /*isfromfield*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::forceChildDrivenWriteRefs(SoOutput * /*out*/)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::isSearchingChildren(void)
{
  return SoBaseKit::searchchildren;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setSearchingChildren(const SbBool newval)
{
  SoBaseKit::searchchildren = newval;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::typeCheck(const SbName & /*partname*/, const SoType & parttype, SoNode *node)
{
  if (node == NULL) {
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoBaseKit::typeCheck",
                           "node was NULL");
#endif // debug
    return NULL;
  }
  if (!node->isOfType(parttype)) {
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoBaseKit::typeCheck",
                           "wrong type: %s", node->getTypeId().getName().getString());
#endif // debug
    return NULL;
  }
  return node;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::addToCopyDict(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::copyContents(const SoFieldContainer * /*fromfc*/, SbBool /*copyconnections*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SoGroup *
SoBaseKit::getContainerNode(const SbName &listname, SbBool makeifneeded)
{
  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;
  if (SoBaseKit::findPart(SbString(listname.getString()), kit, partNum,
                          isList, listIdx, makeifneeded)) {
    SoNode *node = kit->fieldList[partNum]->getValue();
    if (node == NULL) return NULL;
    if (!isList) {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoBaseKit::getContainerNode",
                             "part ``%s'' is not a container",
                             listname.getString());
#endif // debug
      return NULL;
    }
    assert(node->isOfType(SoNodeKitListPart::getClassTypeId()));
    SoNodeKitListPart *list = (SoNodeKitListPart*)node;
    return list->getContainerNode();
  }
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::getAnyPart(const SbName &partname, SbBool makeifneeded, SbBool leafcheck, SbBool publiccheck)
{

  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  SbString partstring(partname.getString());

  if (SoBaseKit::findPart(partstring, kit, partNum, isList, listIdx, makeifneeded)) {
    if (!publiccheck || kit->getNodekitCatalog()->isPublic(partNum)) {
      if (!leafcheck || kit->getNodekitCatalog()->isLeaf(partNum)) {
        if (isList) {
          SoNode *partnode = kit->fieldList[partNum]->getValue();
          if (partnode == NULL) return NULL;
          assert(partnode->isOfType(SoNodeKitListPart::getClassTypeId()));
          SoNodeKitListPart *list = (SoNodeKitListPart*) partnode;
          if (listIdx >= 0 && listIdx < list->getNumChildren()) {
            return list->getChild(listIdx);
          }
          else {
#if COIN_DEBUG && 1 // debug
            SoDebugError::postInfo("SoBaseKit::getAnyPart",
                                   "index %d out of bounds for part: %s",
                                   listIdx, partname.getString());
#endif // debug

          }
        }
        else {
          return kit->fieldList[partNum]->getValue();
        }
      }
    }
  }
  // FIXME:
  // run cleanup?, in case some node has been temporarily created while
  // searching for the part?? pederb, 2000-01-05
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToAnyPart(const SbName &partname, SbBool makeifneeded, SbBool /*leafcheck*/, SbBool /*publiccheck*/, const SoPath *pathtoextend)
{
  // FIXME: leafcheck and publiccheck support, pederb 2000-01-07

  SoFullPath *path;
  if (pathtoextend) {
    path = (SoFullPath*)pathtoextend->copy();
    // pop off nodes beyond this kit node
    if (path->containsNode(this)) while (path->getTail() != this && path->getLength()) path->pop();
    else if (path->getLength()) {
      SoNode *node = path->getTail();
      if (!node->getChildren() || node->getChildren()->find(this) < 0) {
#if COIN_DEBUG && 1 // debug
        SoDebugError::postInfo("SoBaseKit::createPathToAnyPart",
                                 "pathtoextend is illegal");
#endif // debug
        return NULL;
      }
      path->append(this); // this should be safe now
    }
  }
  else {
    path = (SoFullPath*)new SoPath(this);
  }
  path->ref();

  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  if (SoBaseKit::findPart(SbString(partname.getString()), kit, partNum,
                          isList, listIdx, makeifneeded, path)) {
    SoNode *node = kit->fieldList[partNum]->getValue();
    if (node) {
      path->append(node);
      if (isList) {
        SoNodeKitListPart *list = (SoNodeKitListPart*)node;
        if (listIdx < 0 || listIdx >= list->getNumChildren()) {
#if COIN_DEBUG && 1 // debug
          SoDebugError::postInfo("SoBaseKit::createPathToAnyPart",
                                 "index %d out of bounds for part ``%s''",
                                 listIdx, partname.getString());
#endif // debug
          path->unref();
          return NULL;
        }
        else {
          path->append(list->getContainerNode());
          path->append(list->getChild(listIdx));
        }
      }
      path->unrefNoDelete();
      return (SoNodeKitPath*)path;
    }
  }
  path->unref();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setAnyPart(const SbName &partname, SoNode *from, SbBool anypart)
{
  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  SbString partstring(partname.getString());

  if (SoBaseKit::findPart(partstring, kit, partNum, isList, listIdx, TRUE)) {
    if (anypart || kit->getNodekitCatalog()->isPublic(partNum)) {
      if (isList) {
        SoNode *partnode = kit->fieldList[partNum]->getValue();
        if (partnode) {
          assert(partnode->isOfType(SoNodeKitListPart::getClassTypeId()));
          SoNodeKitListPart *list = (SoNodeKitListPart*) partnode;
          if (listIdx >= 0 && listIdx <= list->getNumChildren()) {
            if (listIdx == list->getNumChildren())
              list->addChild(from);
            else
              list->replaceChild(listIdx, from);
            return TRUE;
          }
          else {
#if COIN_DEBUG && 1 // debug
            SoDebugError::postInfo("SoBaseKit::setAnyPart",
                                   "index %d out of bounds for part ``%s''",
                                   listIdx, partname.getString());
#endif // debug

          }
        }
      }
      else {
        return kit->setPart(partNum, from);
      }
    }
  }
  // FIXME:
  // run cleanup, in case some node has been temporarily created while
  // searching for the part?? pederb, 2000-01-05
  return FALSE;
}

/*!
  Not part of the Coin API. It is supposed to create the SoNodekitParts
  class instance. Since this class can only be used by SoBaseKit
  (all members are private, with SoBaseKit as friend), we decided
  to not support this class, and solve the problem of recording
  which parts are created in another way.
*/
void
SoBaseKit::createNodekitPartsList(void)
{
  COIN_STUB();
}

/*!
  Replaces the createNodekitPartsList() method. Sets up the information
  needed to quickly determine which parts are created.
*/
void
SoBaseKit::createFieldList(void)
{
  // FIXME:
  // is there any way to make sure this code is only run once, and in
  // the top level constructor. pederb, 2000-01-06
  //
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  // only do this if the catalog has been created
  if (catalog) {
    delete [] this->fieldList;
    this->fieldList = NULL;
    this->numCatalogEntries = catalog->getNumEntries();
    this->fieldList = new SoSFNode*[this->numCatalogEntries];
    this->fieldList[0] = NULL; // first catalog entry is "this"
    for (int i = 1; i < this->numCatalogEntries; i++) {
      this->fieldList[i] = (SoSFNode*)this->getField(catalog->getName(i));
      assert(this->fieldList[i] != NULL);
    }
  }
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createDefaultParts(void)
{
  // FIXME:
  // is there any way to make sure this code is only run once, and in
  // the top level constructor. pederb, 2000-01-06
  //
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  // only do this if the catalog has been created
  if (catalog) {
    assert(this->fieldList != NULL);
    int n = this->numCatalogEntries;
    for (int i = 1; i < n; i++) {
      if (this->fieldList[i]->getValue() == NULL && !catalog->isNullByDefault(i)) {
        this->makePart(i);
        this->fieldList[i]->setDefault(TRUE);
      }
    }
  }
}

/*!
  In Open Inventor, this method returns a pointer to a private class.
  It will always return \c NULL in Coin.

  \sa createNodekitPartsList()
*/
const SoNodekitParts *
SoBaseKit::getNodekitPartsList(void) const
{
#if COIN_DEBUG
  SoDebugError::post("SoBaseKit::getNodekitPartsList",
                     "OIV method not supported by Coin (returns private "
                     "class)");
#endif // COIN_DEBUG
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::catalogError(void)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setUpConnections(SbBool /*onoff*/, SbBool /*doitalways*/)
{
  return this->connectionsSetUp;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::readInstance(SoInput *in, unsigned short flags)
{
  int i;

  // store old part values to find which parts are read
  SoNode **nodelist = new SoNode*[this->numCatalogEntries];
  for (i = 1; i < this->numCatalogEntries; i++) {
    nodelist[i] = this->fieldList[i]->getValue();
  }

  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    for (i = 1; i < this->numCatalogEntries; i++) {
      SoNode *partnode = this->fieldList[i]->getValue();
      if (partnode != nodelist[i]) {
        partnode->ref(); // ref to make sure node is not deleted
        this->fieldList[i]->setValue(nodelist[i]); // restore old value
        nodelist[i] = partnode; // set value for second iteration
      }
      else nodelist[i] = NULL;
    }
    for (i = 1; i < this->numCatalogEntries; i++) {
      if (nodelist[i]) { // part has changed
        this->setPart(i, nodelist[i]);
        nodelist[i]->unrefNoDelete(); // should be safe to unref now
      }
    }
  }
  delete [] nodelist;
  return ret;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::countMyFields(SoOutput * /*out*/)
{
  COIN_STUB();
}

//
// recurse until not possible to split string any more, and return information
// about part and the kit the part is found in.
// Remember to set kit=this before calling this method, also remember that
// kit might change during this search.
//
// compoundname parts are created during this search, so it might be necessary
// to do a nodekit cleanup if part is not public, or if part is set to NULL.
//
//
// if path != NULL, kit-nodes will be appended to the path during the search
// The actualt part is not added to the path. The head of the path should
// be set to the kit-node performing the search.
//
SbBool
SoBaseKit::findPart(const SbString &partname, SoBaseKit *&kit, int &partNum,
                    SbBool &isList, int &listIdx, const SbBool makeIfNeeded,
                    SoPath *path)
{
  // BNF:
  //
  // partname = singlename | compoundname
  // compoundname = singlename | compoundname.singlename
  // singlename = singlepartname | singlelistelementname
  // singlelistelementname = singlelistname[idx]
  //
  // singlepartname is name of a part ("ordinary", nodekit or list)
  // singlelistname is name of a part which is a list
  // idx is an integer value

  if (partname == "this") {
    isList = FALSE;
    partNum = 0;
    return TRUE;
  }

  const char *stringptr = partname.getString();
  const char *periodptr = strchr(stringptr, '.'); // find first period
  const char *startbracket = strchr(stringptr, '[');

  if (periodptr && (startbracket > periodptr))
    startbracket = NULL; // will handle later

  isList = FALSE; // set to FALSE first
  SbString firstpartname;
  if (startbracket) { // get index
    long int listindex = strtol(startbracket+1, NULL, 10);
    if (listindex == LONG_MIN || listindex == LONG_MAX) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoBaseKit::findPart",
                             "list index not properly specified");
#endif // COIN_DEBUG
      return FALSE;
    }
    firstpartname = partname.getSubString(0, startbracket-stringptr-1);
    listIdx = (int) listindex;
    isList = TRUE;
  }
  else if (periodptr) {
    firstpartname = partname.getSubString(0, periodptr-stringptr-1);
  }
  else firstpartname = partname;

  partNum = kit->getNodekitCatalog()->getPartNumber(firstpartname);
  if (partNum == SO_CATALOG_NAME_NOT_FOUND) {
    return FALSE;
  }

  assert(partNum < kit->numCatalogEntries);
  SoSFNode *nodefield = kit->fieldList[partNum];
  assert(nodefield);

  if (makeIfNeeded && nodefield->getValue() == NULL) {
    kit->makePart(partNum);
  }

  if (path) {
    const SoNodekitCatalog *catalog = kit->getNodekitCatalog();
    SbList <SoNode*> nodestopart;
    int parent = catalog->getParentPartNumber(partNum);
    while (parent > 0) {
      SoNode *node = kit->fieldList[parent]->getValue();
      if (node == NULL) {
        assert(makeIfNeeded == FALSE);
        break;
      }
      nodestopart.push(node);
      parent = catalog->getParentPartNumber(parent);
    }
    assert(parent == 0 || !makeIfNeeded);
    while (nodestopart.getLength()) {
      SoNode *node = nodestopart.pop();
      path->append(node);
    }
  }

  if (periodptr == NULL) {
    // singlename or singlelistname found, do not recurse any more
    return TRUE; // all info has been found, just return TRUE
  }
  else { // recurse
    SoNode *node = nodefield->getValue();
    if (node == NULL) return FALSE;
    SbString newpartname = partname.getSubString(periodptr-stringptr+1);
    if (isList) {
      SoNodeKitListPart *list = (SoNodeKitListPart*) node;
      if (listIdx < 0 || listIdx >= list->getNumChildren()) {
#if COIN_DEBUG
        SoDebugError::postInfo("SoBaseKit::findPart",
                               "index (%d) out of bounds for part ``%s''",
                               listIdx,
                               firstpartname.getString());
#endif // COIN_DEBUG
        return FALSE;
      }
      SoNode *partnode = list->getChild(listIdx);
      assert(partnode->isOfType(SoBaseKit::getClassTypeId()));
      kit = (SoBaseKit*)partnode;

      if (path) {
        path->append(list);
        path->append(list->getContainerNode());
      }
    }
    else {
      // FIXME: replace with test and debug message? pederb, 2000-01-04
      assert(node->isOfType(SoBaseKit::getClassTypeId()));
      kit = (SoBaseKit*)node;
    }
    if (path) path->append(kit);
    return SoBaseKit::findPart(newpartname, kit, partNum, isList,
                               listIdx, makeIfNeeded);
  }
}

//
// makes part, makes sure node is connected in the scene
//
SbBool
SoBaseKit::makePart(const int partNum)
{
  assert(partNum > 0 && partNum < this->numCatalogEntries);
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  assert(catalog);
  assert(this->fieldList[partNum]->getValue() == NULL);

  SoNode *node = (SoNode*)catalog->getDefaultType(partNum).createInstance();
  return this->setPart(partNum, node);
}

/*!
  Sets parts, updates nodekit scene graph, and makes sure
  graph is valid with respect to right siblings and parent.
  This method is virtual to enable subclasses to detect when a
  part changes value.

  This method is not part of the OIV API.
*/
SbBool
SoBaseKit::setPart(const int partNum, SoNode *node)
{
  assert(partNum > 0 && partNum < this->numCatalogEntries);
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  assert(catalog);

  int parentIdx = catalog->getParentPartNumber(partNum);
  assert(parentIdx >= 0 && parentIdx < this->numCatalogEntries);
  SoNode *parent = NULL;
  if (parentIdx == 0) parent = this;
  else parent = this->fieldList[parentIdx]->getValue();
  if (parent == NULL) {
    this->makePart(parentIdx);
    parent = this->fieldList[parentIdx]->getValue();
  }
  assert(parent != NULL);
  SoChildList *childlist = parent->getChildren();
  assert(childlist != NULL);

  SoNode *oldnode = this->fieldList[partNum]->getValue();

  if (oldnode != NULL) { // part exists, replace
    int oldIdx = childlist->find(oldnode);
    assert(oldIdx >= 0);
    childlist->remove(oldIdx);
    if (node) childlist->insert(node, oldIdx);
  }
  else if (node) { // find where to insert in parent childlist
    int rightSibling = this->getRightSiblingIndex(partNum);
    if (rightSibling >= 0) { // part has right sibling, insert before
      int idx = childlist->find(this->fieldList[rightSibling]->getValue());
      assert(idx >= 0);
      childlist->insert(node, idx);
    }
    else childlist->append(node);
  }

  // set part field value
  this->fieldList[partNum]->setValue(node);
  return TRUE;
}

//
// returns part number of existing right sibling or -1 if none exists
//
int
SoBaseKit::getRightSiblingIndex(const int partNum)
{
  assert(partNum > 0 && partNum < this->numCatalogEntries);
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();

  int sibling = catalog->getRightSiblingPartNumber(partNum);

  // iterate until no more siblings or until we find an existing one
  while (sibling >= 0 && this->fieldList[sibling]->getValue() == NULL) {
    sibling = catalog->getRightSiblingPartNumber(sibling);
  }
  return sibling;
}

//
// Searches the field list to find of a node is in this kit.
// Returns catalog index, -1 if not found
//
// parentnum is checked if >= 0
//
int
SoBaseKit::findNodeInThisKit(SoNode *node, const int parentnum) const
{
  const SoNodekitCatalog *catalog = this->getNodekitCatalog();
  if (node == (SoNode*)this) return 0;
  int n = this->numCatalogEntries;
  for (int i = 1; i < n; i++) {
    if (this->fieldList[i]->getValue() == node &&
        (parentnum < 0 || catalog->getParentPartNumber(i) == parentnum))
      return i;
  }
  return -1;
}
