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
#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/SbString.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoBaseKit::~SoBaseKit()
{
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

  if (partname == "this") return NULL; // toplevel entry is private

  SoNode * ptr = NULL;

  SbString s(partname.getString());
  int idx = 0;
  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();

  // Get string token.
  while ((idx < s.getLength()) && (s[idx] != '.') && (s[idx] != '[')) idx++;
  SbName subpartname(s.getSubString(0, idx-1));

  int nr = thiscat->getPartNumber(subpartname);
  if (nr == SO_CATALOG_NAME_NOT_FOUND) {
    SoTypeList tl;
    // Starts at index 1 to skip toplevel ``this'' part.
    for (int i = 1; i < thiscat->getNumEntries(); i++) {
      if (thiscat->getType(i).isDerivedFrom(SoBaseKit::getClassTypeId())) {
        if (thiscat->recursiveSearch(i, subpartname, &tl)) {
          subpartname = thiscat->getName(i);
          idx = 0;
          nr = i;
        }
      }
    }
  }

  if (nr == SO_CATALOG_NAME_NOT_FOUND) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoBaseKit::getPart",
                           "no such part: ``%s''", subpartname.getString());
#endif // COIN_DEBUG
    return NULL;
  }

#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoBaseKit::getPart",
                         "hit: ``%s''", subpartname.getString());
#endif // debug

  SbBool privateentry = !thiscat->isPublic(nr);

  SoSFNode * nodefield = (SoSFNode *) this->getField(subpartname);
  assert(nodefield && "erroneous catalog specification?");
  ptr = nodefield->getValue();

  if (!ptr) {
    if (!makeifneeded) return NULL;

    // Recursively allocate parents, while ignoring return value
    // (because we will hit private catalog entries).
    this->getPart(thiscat->getParentName(nr), TRUE);

    SoType nodetype = thiscat->getDefaultType(nr);
    assert(nodetype.canCreateInstance());
    ptr = (SoNode *) nodetype.createInstance();
    nodefield->setValue(ptr);

    // FIXME: should we addChild() ptr to parent? 19991205 mortene.
  }

  if (thiscat->isList(nr) && (idx < s.getLength()) && (s[idx] == '[')) {
    idx++;
    // Get index number token (if any).
    const char * startptr = s.getString() + idx;
    char * endptr;
    long int listindex = strtol(startptr, &endptr, 10);
    idx += endptr - startptr;
    if ((startptr == endptr) || (s[idx] != ']')) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoBaseKit::getPart",
                             "list index not properly specified");
#endif // COIN_DEBUG
      return NULL;
    }
    idx++;

    // FIXME: allocate and add list node if listindex > length of
    // list?  19991205 mortene.

    ptr = ((SoNodeKitListPart *)ptr)->getChild(listindex);
  }


  if (idx < s.getLength()) {
    if (!ptr) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoBaseKit::getPart",
                             "part ``%s'' not found", subpartname.getString());
#endif // COIN_DEBUG
      return NULL;
    }

    if (!ptr->getTypeId().isDerivedFrom(SoBaseKit::getClassTypeId())) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoBaseKit::getPart",
                             "``%s'' is not derived from SoBaseKit",
                             ptr->getTypeId().getName().getString());
#endif // COIN_DEBUG
      return NULL;
    }

    if (s[idx] == '.') idx++;
    SbName rest(s.getSubString(idx, s.getLength()-1));
    return ((SoBaseKit *)ptr)->getPart(rest, makeifneeded);
  }

  return privateentry ? NULL : ptr;
}

/*!
  FIXME: write function documentation
*/
SbString
SoBaseKit::getPartString(const SoBase * /*part*/)
{
  COIN_STUB();
  return SbString();
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, const SoPath * /*pathtoextend*/)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setPart(const SbName & /*partname*/, SoNode * /*from*/)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(char * /*namevaluepairliststring*/)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(char * /*partnamestring*/, char * /*parameterstring*/)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::doAction(SoAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::callback(SoCallbackAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::GLRender(SoGLRenderAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getBoundingBox(SoGetBoundingBoxAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getMatrix(SoGetMatrixAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::handleEvent(SoHandleEventAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::rayPick(SoRayPickAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::search(SoSearchAction * /*action*/)
{
  COIN_STUB();
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
SoBaseKit::getPrimitiveCount(SoGetPrimitiveCountAction * /*action*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SoChildList *
SoBaseKit::getChildren(void) const
{
  COIN_STUB();
  return NULL;
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
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setSearchingChildren(SbBool /*newval*/)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::typeCheck(const SbName & /*partname*/, const SoType & /*parttype*/, SoNode * /*node*/)
{
  COIN_STUB();
  return NULL;
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
SoBaseKit::getContainerNode(const SbName & /*listname*/, SbBool /*makeifneeded*/)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::getAnyPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, SbBool /*leafcheck*/, SbBool /*publiccheck*/)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToAnyPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, SbBool /*leafcheck*/, SbBool /*publiccheck*/, const SoPath * /*pathtoextend*/)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setAnyPart(const SbName & /*partname*/, SoNode * /*from*/, SbBool /*anypart*/)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createNodekitPartsList(void)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createDefaultParts(void)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
const SoNodekitParts *
SoBaseKit::getNodekitPartsList(void) const
{
  COIN_STUB();
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
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::readInstance(SoInput * /*in*/, unsigned short /*flags*/)
{
  COIN_STUB();
  return FALSE;
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
