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
  return &SoBaseKit::classcatalog;
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
  Print out the nodekit catalog structure. Useful for debugging.
*/
void
SoBaseKit::printDiagram(void)
{
  this->printSubDiagram("this", 0);
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::printSubDiagram(const SbName & /*rootname*/, int /*level*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::printTable(void)
{
  assert(0 && "FIXME: not implemented yet");
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
