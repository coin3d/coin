/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoFieldSensor.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <string.h>

/*!
  \var SoSFString SoFile::name

  Filename for model file to insert in the scenegraph at the location
  of the SoFile node.
*/

// *************************************************************************

class SoFileP {
public:
  static const char UNDEFINED_FILE[];
};

const char SoFileP::UNDEFINED_FILE[] = "<Undefined file>";

// *************************************************************************

SO_NODE_SOURCE(SoFile);

/*!
  Constructor.
*/
SoFile::SoFile(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFile);

  SO_NODE_ADD_FIELD(name, (SoFileP::UNDEFINED_FILE));

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
  SO_NODE_INTERNAL_INIT_CLASS(SoFile, SO_FROM_INVENTOR_1);
}

/*!  
  Returns the read filename, possibly including the (relative) path
  where the file was found.  Returns an empty string if no file has
  been read.

  This method is an extension versus the Open Inventor API.  
*/
const SbString & 
SoFile::getFullName(void) const
{
  return this->fullname;
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
  // We detach the sensor and later call readNamedFile() explicitly
  // instead of letting readNamedFile() be called implicitly due to
  // the SoFieldSensor on this->name.
  //
  // This is done so the same SoInput instance is used for
  // readNamedFile() as for this method, and then also the same name
  // dictionary -- which is necessary for "cross-file" references to
  // work.
  //
  // (Fixed Bugzilla #202.)

  this->fullname.makeEmpty();
  this->namesensor->detach();
  SbBool result = inherited::readInstance(in, flags);
  this->namesensor->attach(& this->name);
  return result && this->readNamedFile(in);
}

/*!
  Read the file named in the SoFile::name field.

  \since 2001-12-05. Used to be private, moved into protected space to
  enable subclasses to detect when the file is (re)read.
*/
SbBool
SoFile::readNamedFile(SoInput * in)
{
  if (this->name.getValue().getLength() == 0 ||
      strcmp(this->name.getValue().getString(), SoFileP::UNDEFINED_FILE) == 0) {
    // We handle this different than Inventor, where the read process
    // fails upon an unspecified filename.
    SoReadError::post(in, "Undefined filename in SoFile");
    return TRUE;
  }

  // If we can't find file, ignore it. Note that this does not match
  // the way Inventor works, which will make the whole read process
  // exit with a failure code.
  if (!in->pushFile(this->name.getValue().getString())) return TRUE;

  this->fullname = in->getCurFileName();

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
  that->fullname.makeEmpty();
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
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
  else {
    this->children->traverse((SoAction *)action);
  }
}

// Doc from superclass.
void
SoFile::callback(SoCallbackAction * action)
{
  SoFile::doAction((SoAction *)action);
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
SoFile::audioRender(SoAudioRenderAction * action)
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
