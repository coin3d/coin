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
  \class SoFile SoFile.h Inventor/nodes/SoFile.h
  \brief The SoFile class is node container for another model file.
  \ingroup nodes

  This node provides a way to split your models into a set of
  "component" models to include into larger "master" files.

  Just provide the name of the model file to include in the
  SoFile::name field, and it will automatically be loaded and have its
  nodes inserted into the scenegraph at the point of the SoFile node.
*/

#include <Inventor/nodes/SoFile.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoChildList.h>
#include <coindefs.h> // COIN_STUB()

/*!
  \var SoSFString SoFile::name

  Filename for model file to insert in the scenegraph at the location
  of the SoFile node.
*/

static const char UNDEFINED_FILE[] = "<Undefined file>";

// *************************************************************************

SO_NODE_SOURCE(SoFile);

/*!
  Constructor.
*/
SoFile::SoFile(void)
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

// Doc from superclass.
void
SoFile::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFile);
}

// Doc from superclass.
void
SoFile::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFile::GLRender(SoGLRenderAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
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
  // Popping the file off the stack again is done implicit in SoInput
  // upon hitting EOF.

  if (node) this->children->append((SoNode *)node);
  else {
    SoReadError::post(in, "Unable to open subfile: ``%s''",
                      this->name.getValue().getString());
  }
  return TRUE;
}

/*!
  Returns a group node with a deep copy of the children of this node.
 */
SoGroup *
SoFile::copyChildren(void) const
{
  COIN_STUB(); // FIXME
  return NULL;
}

// Doc from superclass.
SoChildList *
SoFile::getChildren(void) const
{
  return this->children;
}

// Doc from superclass.
void
SoFile::doAction(SoAction * action)
{
  this->children->traverse((SoAction *)action);
}

// Doc from superclass.
void
SoFile::callback(SoCallbackAction * action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoFile::doAction((SoAction *)action);
    action->invokePostCallbacks(this);
  }
}

// Doc from superclass.
void
SoFile::getMatrix(SoGetMatrixAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFile::handleEvent(SoHandleEventAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFile::pick(SoPickAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFile::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoFile::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFile::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  COIN_STUB(); // FIXME
}
