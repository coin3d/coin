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


#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \var SoSFString SoFile::name
  FIXME: write documentation for field
*/

static const char UNDEFINED_FILE[] = "<Undefined file>";

// *************************************************************************

SO_NODE_SOURCE(SoFile);

/*!
  Constructor.
*/
SoFile::SoFile()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFile);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoFile);
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
  SoFile::doAction((SoAction*)action);
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

  in->pushFile(name.getValue().getString());
  SoSeparator * node = SoDB::readAll(in);
  // FIXME: this should really be done implicit in SoInput upon
  // hitting EOF. 19990708 mortene.
  in->popFile();
  if (node) this->children->append((SoNode *)node);
  else {
    SoReadError::post(in, "Unable to open subfile: ``%s''",
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
  COIN_STUB();
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
SoFile::doAction(SoAction *action)
{
  this->children->traverse((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoFile::callback(SoCallbackAction *action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoFile::doAction((SoAction *)action);
    action->invokePostCallbacks(this);
  }
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoFile::getMatrix(SoGetMatrixAction *action)
{
  SoFile::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoFile::handleEvent(SoHandleEventAction *action)
{
  SoFile::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoFile::pick(SoPickAction *action)
{
  SoFile::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFile::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoFile::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
void
SoFile::copyContents(const SoFieldContainer * /* fromFC */,
                     SbBool /* copyConnections */)
{
  COIN_STUB();
}
