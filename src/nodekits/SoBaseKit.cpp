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

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodekitCatalog.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/SbString.h>


SO_NODE_SOURCE(SoBaseKit);


SoNodekitCatalog * SoBaseKit::classcatalog = NULL;
const SoNodekitCatalog ** SoBaseKit::parentcatalogptr;


/*!
  Constructor.
*/
SoBaseKit::SoBaseKit()
{
  SO_NODE_CONSTRUCTOR(SoBaseKit);
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

  SoBaseKit::parentcatalogptr = NULL;

  SoBaseKit::classcatalog = new SoNodekitCatalog;
  SoBaseKit::classcatalog->addEntry("this",
				    SoBaseKit::getClassTypeId(),
				    SoBaseKit::getClassTypeId(),
				    TRUE, "", "", FALSE,
				    SoType::badType(),
				    SoType::badType(),
				    FALSE);
  SoBaseKit::classcatalog->addEntry("callbackList",
				    SoNodeKitListPart::getClassTypeId(),
				    SoNodeKitListPart::getClassTypeId(),
				    TRUE, "this", "", TRUE,
				    SoSeparator::getClassTypeId(),
				    SoCallback::getClassTypeId(),
				    TRUE);
  SoBaseKit::classcatalog->addListItemType("callbackList",
					   SoEventCallback::getClassTypeId());
}


/*!
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/
const SoNodekitCatalog *
SoBaseKit::getClassNodekitCatalog(void)
{
  return SoBaseKit::classcatalog;
}

/*!
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/
const SoNodekitCatalog *
SoBaseKit::getNodekitCatalog(void) const
{
  return SoBaseKit::classcatalog;
}

/*!
  Returns the pointer to the pointer of the nodekit catalog
  for this class.
*/
const SoNodekitCatalog **
SoBaseKit::getClassNodekitCatalogPtr(void)
{
  return (const class SoNodekitCatalog **)&SoBaseKit::classcatalog;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::getPart(const SbName & /*partname*/, SbBool /*makeifneeded*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbString
SoBaseKit::getPartString(const SoBase * /*part*/)
{
  assert(0 && "FIXME: not implemented yet");
  return SbString();
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, const SoPath * /*pathtoextend*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setPart(const SbName & /*partname*/, SoNode * /*from*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(char * /*namevaluepairliststring*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(char * /*partnamestring*/, char * /*parameterstring*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::doAction(SoAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::callback(SoCallbackAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::GLRender(SoGLRenderAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getBoundingBox(SoGetBoundingBoxAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getMatrix(SoGetMatrixAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::handleEvent(SoHandleEventAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::rayPick(SoRayPickAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::search(SoSearchAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::write(SoWriteAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::getPrimitiveCount(SoGetPrimitiveCountAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoChildList *
SoBaseKit::getChildren(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  Print out the full nodekit catalog structure. Useful for debugging.
*/
void
SoBaseKit::printDiagram(void)
{
  fprintf(stdout, "CLASS %s\n", this->getTypeId().getName().getString());
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

  int i = 0;
  if (!parentcatalog ||
      parentcatalog->getPartNumber(rootname) == SO_CATALOG_NAME_NOT_FOUND) {
    fprintf(stdout, "-->");
    i++;
  }
  for (; i < level+1; i++) fprintf(stdout, "   ");

  fprintf(stdout, "\"%s\"\n", rootname.getString());

  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();
  for (int j=0; j < thiscat->getNumEntries(); j++) {
    // FIXME: make a list of sorted children to print in correct
    // sibling order. 19991118 mortene.
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
  fprintf(stdout, "CLASS %s\n", this->getTypeId().getName().getString());

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
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::forceChildDrivenWriteRefs(SoOutput * /*out*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::isSearchingChildren(void)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setSearchingChildren(SbBool /*newval*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::typeCheck(const SbName & /*partname*/, const SoType & /*parttype*/, SoNode * /*node*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::addToCopyDict(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::copyContents(const SoFieldContainer * /*fromfc*/, SbBool /*copyconnections*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoGroup *
SoBaseKit::getContainerNode(const SbName & /*listname*/, SbBool /*makeifneeded*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::getAnyPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, SbBool /*leafcheck*/, SbBool /*publiccheck*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToAnyPart(const SbName & /*partname*/, SbBool /*makeifneeded*/, SbBool /*leafcheck*/, SbBool /*publiccheck*/, const SoPath * /*pathtoextend*/)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setAnyPart(const SbName & /*partname*/, SoNode * /*from*/, SbBool /*anypart*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createNodekitPartsList(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createDefaultParts(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
const SoNodekitParts *
SoBaseKit::getNodekitPartsList(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::catalogError(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setUpConnections(SbBool /*onoff*/, SbBool /*doitalways*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::readInstance(SoInput * /*in*/, unsigned short /*flags*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setDefaultOnNonWritingFields(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::countMyFields(SoOutput * /*out*/)
{
  assert(0 && "FIXME: not implemented yet");
}
