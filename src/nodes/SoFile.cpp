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

  You can also set the SoFile::name field manually. Such an action
  will then automatically trigger an invocation of a read operation
  which imports the filename you set in the field.
*/

#include <Inventor/nodes/SoFile.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoFieldSensor.h>


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

  this->namesensor = new SoFieldSensor(SoFile::nameFieldModified, this);
  this->namesensor->setPriority(0); // immediate sensor
  this->namesensor->attach(& this->name);

  this->children = new SoChildList(this);
}

/*!
  Destructor.
*/
SoFile::~SoFile()
{
  delete this->namesensor;
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
  if (!inherited::readInstance(in, flags)) return FALSE;

  // Note that a file specified in the SoFile::name field will be
  // automatically loaded upon import due to the SoFieldSensor
  // attached to it.

  // FIXME: this method isn't really necessary. 20000404 mortene.

  return TRUE;
}

// Private method. Read the file named in the SoFile::name field.
SbBool
SoFile::readNamedFile(SoInput * in)
{
  if (this->name.getValue().getLength() == 0 ||
      strcmp(this->name.getValue().getString(), UNDEFINED_FILE) == 0) {
    // We handle this different than Inventor, where the read process
    // fails upon an unspecified filename.
    SoReadError::post(in, "Undefined filename in SoFile");
    return TRUE;
  }

  // If we can't find file, ignore it. Note that this does not match
  // the way Inventor works, which will make the whole read process
  // exit with a failure code.
  if (!in->pushFile(this->name.getValue().getString())) return TRUE;

  SoSeparator * node = SoDB::readAll(in);
  // Popping the file off the stack again is done implicit in SoInput
  // upon hitting EOF (unless the read fails, see below).

  if (node) {
    this->children->truncate(0);
    this->children->append((SoNode *)node);
  }
  else {
    // Take care of popping the file off the stack. This is a bit
    // "hack-ish", but its done this way instead of loosening the
    // protection of SoInput::popFile().
    if (in->getCurFileName() == this->name.getValue()) {
      char dummy;
      while (!in->eof()) in->get(dummy);
    }

    // Note that we handle this differently than Inventor, which lets
    // the whole import fail.
    SoReadError::post(in, "Unable to read subfile: ``%s''",
                      this->name.getValue().getString());
  }

  return TRUE;
}

// Callback for the field sensor.
void
SoFile::nameFieldModified(void * userdata, SoSensor * sensor)
{
  SoFile * that = (SoFile *)userdata;
  SoInput in;
  (void)that->readNamedFile(&in);
}

/*!
  Returns a subgraph with a deep copy of the children of this node.
*/
SoGroup *
SoFile::copyChildren(void) const
{
  if (this->children->getLength() == 0) return NULL;
  assert(this->children->getLength() == 1);
  SoNode * rootcopy = (*(this->children))[0]->copy();
  assert(rootcopy->isOfType(SoGroup::getClassTypeId()));
  return (SoGroup *)rootcopy;
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
  this->children->truncate(0);
  inherited::copyContents(from, copyconnections);

  SoFile * filenode = (SoFile *)from;

  if (filenode->children->getLength() == 0) return;

  assert(filenode->children->getLength() == 1);

  SoNode * cp = (SoNode *)
    SoFieldContainer::findCopy((*(filenode->children))[0], copyconnections);
  this->children->append(cp);
}
