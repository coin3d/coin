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
  \class SoNodeKitListPart SoNodeKitListPart.h Inventor/nodekits/SoNodeKitListPart.h
  \brief The SoNodeKitListPart class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/lists/SoTypeList.h>

SO_NODE_SOURCE(SoNodeKitListPart);


/*!
  Constructor.
*/
SoNodeKitListPart::SoNodeKitListPart()
{
  SO_NODE_CONSTRUCTOR(SoNodeKitListPart);
}

/*!
  Destructor.
*/
SoNodeKitListPart::~SoNodeKitListPart()
{
}

/*!
  Does initialization common for all objects of the
  SoNodeKitListPart class. This includes setting up the
  type system, among other things.
*/
void
SoNodeKitListPart::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNodeKitListPart);
}

/*!
  FIXME: write function documentation
*/
SoType
SoNodeKitListPart::getContainerType(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return SoType::badType();
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::setContainerType(SoType /*newContainerType*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
const SoTypeList &
SoNodeKitListPart::getChildTypes(void) const
{
  assert(0 && "FIXME: not implemented yet");
  static SoTypeList tl;
  return tl;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::addChildType(SoType /*typeToAdd*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::isTypePermitted(SoType /*typeToCheck*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::isChildPermitted(const SoNode * /*child*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::containerSet(const char * /*fieldDataString*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::lockTypes(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::isTypeLocked(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::addChild(SoNode * /*child*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::insertChild(SoNode * /*child*/, int /*childIndex*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoNodeKitListPart::getChild(int /*index*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
int
SoNodeKitListPart::findChild(SoNode * /*child*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return -1;
}

/*!
  FIXME: write function documentation
*/
int
SoNodeKitListPart::getNumChildren(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return -1;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::removeChild(int /*index*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::removeChild(SoNode * /*child*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::replaceChild(int /*index*/, SoNode * /*newChild*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::replaceChild(SoNode * /*oldChild*/, SoNode * /*newChild*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::affectsState(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::doAction(SoAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::callback(SoCallbackAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::GLRender(SoGLRenderAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::getBoundingBox(SoGetBoundingBoxAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::getMatrix(SoGetMatrixAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::handleEvent(SoHandleEventAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::pick(SoPickAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::search(SoSearchAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::getPrimitiveCount(SoGetPrimitiveCountAction * /*action*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoChildList *
SoNodeKitListPart::getChildren(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoGroup *
SoNodeKitListPart::getContainerNode(void)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::readInstance(SoInput * /*in*/, unsigned short /*flags*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::copyContents(const SoFieldContainer * /*fromFC*/,
                                SbBool /*copyConnections*/)
{
  assert(0 && "FIXME: not implemented yet");
}
