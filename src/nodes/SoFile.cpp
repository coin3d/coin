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
  \class SoFile SoFile.h Inventor/nodes/SoFile.h
  \brief The SoFile class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoFile.h>

#include <Inventor/SbName.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSubNode.h>

/*!
  \var SoSFString SoFile::name
  FIXME: write documentation for field
*/

static const char UNDEFINED_FILE[] = "<Undefined file>";

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoFile)
SoType SoFile::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoFile node class.
*/
void *
SoFile::createInstance(void)
{
  return new SoFile;
}

/*!
  Returns the unique type identifier for the SoFile class.
*/
SoType
SoFile::getClassTypeId(void)
{
  return SoFile::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoFile::getTypeId(void) const
{
  return SoFile::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoFile::SoFile()
{
//$ BEGIN TEMPLATE NodeConstructor(SoFile)
  // Make sure the class has been initialized.
  assert(SoFile::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(name, (UNDEFINED_FILE));

  this->children = new SoChildList(this);
}

/*!
  Destructor.
*/
SoFile::~SoFile()
{
  delete this->children;
}

/*!
  Does initialization common for all objects of the
  SoFile class. This includes setting up the
  type system, among other things.
*/
void
SoFile::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(File)
  // Make sure we only initialize once.
  assert(SoFile::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFile::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "File",
                       &SoFile::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoFile::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoFile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->children->traverse((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoFile::GLRender(SoGLRenderAction * action)
{
  this->children->traverse((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
SbBool 
SoFile::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: 19990423, pederb
  // not quite sure what to do when an error occurs during
  // subfile read. Should I return with an error, or continue
  // trodding along?
  
  if (!inherited::readInstance(in, flags)) return FALSE;

  if (name.getValue().getLength() == 0 ||
      strcmp(name.getValue().getString(), UNDEFINED_FILE) == 0) {
    SoReadError::post(in, "Undefined file name in SoFile"); 
    return TRUE;
  }

  // SoDB::readAll() will pop the file after it
  // is loaded

  in->pushFile(name.getValue().getString());
  SoSeparator * node = SoDB::readAll(in);
  if (node) this->children->append((SoNode *)node);
  else {
    SoReadError::post(in, "Unable to open subfile: %s",
		      this->name.getValue().getString());
  }
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SoGroup *
SoFile::copyChildren(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoChildList *
SoFile::getChildren() const
{
  return this->children;
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoFile::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoFile::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoFile::getMatrix(SoGetMatrixAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoFile::handleEvent(SoHandleEventAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoFile::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFile::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
void
SoFile::copyContents(const SoFieldContainer * /* fromFC */,
		     SbBool /* copyConnections */)
{
  assert(0 && "FIXME: not implemented");
}
