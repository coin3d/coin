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
  \class SoFieldContainer SoFieldContainer.h Inventor/fields/SoFieldContainer.h
  \brief The SoFieldContainer class is a base class for all classes that
  contain fields.
  \ingroup general

  The classes containing fields in Coin are the node and engine
  classes, so they are all subclasses of SoFieldContainer.

  SoFieldContainer provides methods for reading, writing, comparing
  for equality, copy operations, etc on fields.

  \sa SoField

 */


#include <Inventor/fields/SoFieldContainer.h>

#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/VRMLnodes/SoVRMLInterpOutput.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldData.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SbBool SoFieldContainer::isBuiltIn

  Flag for storing whether or not this class instance is a built-in
  class or not. By knowing the difference between a class which is
  native Coin or a user extension, it is possible to automatically
  store and read extension nodes and engines.
*/


SoType SoFieldContainer::classTypeId = SoType::badType();


/*!
  Constructor.
*/
SoFieldContainer::SoFieldContainer(void)
  : isBuiltIn(TRUE), donotify(TRUE)
{
}

/*!
  Destructor.
*/
SoFieldContainer::~SoFieldContainer(void)
{
}

// Overridden from parent class.
void
SoFieldContainer::initClass(void)
{
  // Make sure we init this class only once.
  assert(SoFieldContainer::classTypeId == SoType::badType());
  // Make sure parent class has been initialized.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFieldContainer::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "FieldContainer", NULL);
}

// Overridden from parent class.
SoType
SoFieldContainer::getClassTypeId(void)
{
  return SoFieldContainer::classTypeId;
}

/*!
  This method sets all fields which we are parenting to their respective
  default values.
*/
void
SoFieldContainer::setToDefaults(void)
{
  const SoFieldData * fd = this->getFieldData();
  if (!fd) return;

  // Allocate a fresh template to retrieve values from.
  const SoFieldContainer * from = (const SoFieldContainer *)
    this->getTypeId().createInstance();

  fd->overlay(this, from, FALSE);

  SoFieldList l;
  int n = this->getFields(l);
  for (int i=0; i < n; i++) l[i]->setDefault(TRUE);
}

/*!
  This method checks to see if the fields of this container have their
  default values, and returns \c TRUE if that is the case.
*/
SbBool
SoFieldContainer::hasDefaultValues(void) const
{
  const SoFieldData * fd = this->getFieldData();
  if (!fd) return TRUE;

  // Allocate a fresh template to compare with.
  const SoFieldContainer * fc = (const SoFieldContainer *)
    this->getTypeId().createInstance();

  return fd->isSame(this, fc);
}

/*!
  This method returns \c TRUE if the values of the fields of this and
  \a container are equal.
*/
SbBool
SoFieldContainer::fieldsAreEqual(const SoFieldContainer * container) const
{
  const SoFieldData * fd0 = this->getFieldData();

  if (fd0 == NULL) {
    if (container->getFieldData() == NULL) return TRUE;
    return FALSE;
  }
  
  return fd0->isSame(this, container);
}


/*!
  This method copies the field values from container into this. The fields
  are assumed to be of the same type. The \a copyconnections flag decides
  whether the field connections are to be copied aswell.
*/
void
SoFieldContainer::copyFieldValues(const SoFieldContainer * container,
                                  SbBool copyconnections)
{
  const SoFieldData * fd0 = this->getFieldData();

  if (fd0 == NULL) {
    if (container->getFieldData() == NULL) return;
#if COIN_DEBUG
    SoDebugError::postInfo("SoFieldContainer::copyFieldValues",
                           "tried to copy from fieldcontainer of wrong type");
    return;
#endif // COIN_DEBUG
  }

  fd0->overlay(this, container, copyconnections);
}


/*!
  This method parses the values of one or more fields from the
  \a fielddata string.

  The fields must be in the same format as the Open Inventor file
  format.  \c TRUE is returned upon success, and \c FALSE otherwise.

  \sa get()
 */
SbBool
SoFieldContainer::set(const char * const fielddata)
{
  return this->set(fielddata, NULL);
}

/*!
  This methods stores the field data in the given \a fielddata string.
  The format of the returned string follows the Open Inventor file
  format.

  \sa set()
*/
void
SoFieldContainer::get(SbString & fielddata)
{
  this->get(fielddata, NULL);
}


/*!
  This method adds the fields in this container to the \a l argument,
  and returns the number of fields added.

  The virtual nature of this function means that it can be overridden to
  e.g. hide private fields.
*/
int
SoFieldContainer::getFields(SoFieldList & l) const
{
  const SoFieldData * fd = this->getFieldData();
  if (!fd) return 0;
  int numfields = fd->getNumFields();
  for (int i=0; i < numfields; i++) l.append(fd->getField(this, i));
  return numfields;
}

/*!
  Returns VRML eventIn and eventOut fields in addition to the
  "ordinary" fields.

  \sa getFields()
 */
int
SoFieldContainer::getAllFields(SoFieldList & l) const
{
  // No VRML support yet. Add eventIn and eventOut fields when it is
  // implemented.
  COIN_STUB();
  return this->getFields(l);
}

/*!
  This method returns a pointer to the field with \a name, or
  \c NULL if no such field exists.
*/
SoField *
SoFieldContainer::getField(const SbName & name) const
{
  const SoFieldData * fields = this->getFieldData();
  if (!fields) return NULL;

  const int numfields = fields->getNumFields();
  for (int i = 0; i < numfields; i++) {
    if (name == fields->getFieldName(i))
      return fields->getField(this, i);
  }

  return (SoField *)NULL;
}

/*!
  This method returns a pointer to the eventIn with name \a name,
  or \c NULL if no such eventIn exists.
*/
SoField *
SoFieldContainer::getEventIn(const SbName & name) const
{
  // No VRML support yet. Add eventIn and eventOut fields when it is
  // implemented.
  COIN_STUB();
  return NULL;
}

/*!
  This method returns a pointer to the eventOut with name \a name,
  or \c NULL if no such eventOut exists.
*/
SoField *
SoFieldContainer::getEventOut(const SbName & name) const
{
  // No VRML support yet. Add eventIn and eventOut fields when it is
  // implemented.
  COIN_STUB();
  return NULL;
}


/*!
  Finds the name of the given \a field and returns the value in the
  \a name argument.

  \c TRUE is returned if the field is contained within this instance,
  and \c FALSE otherwise.
*/
SbBool
SoFieldContainer::getFieldName(const SoField * const field,
                               SbName & name) const
{
  const SoFieldData * fields = this->getFieldData();
  if (!fields) return FALSE;
  int idx = fields->getIndex(this, field);
  if (idx == -1) return FALSE;
  name = fields->getFieldName(idx);
  return TRUE;
}


/*!
  This method sets whether notification will be propagated on changing the
  values of the contained fields.  The old value of the setting is returned.

  Notification is default \e on. Turning off automatic notification
  can be useful for optimizing performance. During series of updates
  you may want to avoid the propagation of notifications upon every
  field value change if you make a lot of them. This is how you should
  handle those cases:

  \code
  node->enableNotify(FALSE);
  // ...
  // Make modifications to fields of "node" here.
  // ...
  node->enableNotify(TRUE);
  node->touch();
  \endcode

  \sa isNotifyEnabled()
 */
SbBool
SoFieldContainer::enableNotify(const SbBool enable)
{
  const SbBool old = this->donotify;
  this->donotify = enable;
  return old;
}

/*!
  This method returns whether notification of changes to the field values
  in the container is propagated to its auditors.

  \sa enableNotify()
*/
SbBool
SoFieldContainer::isNotifyEnabled(void) const
{
  return this->donotify;
}


/*!
  This method parses the values of one or more fields from the
  \a fielddata string.

  The fields must be in the same format as the Open Inventor file
  format.  \c TRUE is returned upon success, and \c FALSE otherwise.

  If \a in is specified, we use the reference dictionary provided
  within that object.

  \sa get()
*/
SbBool
SoFieldContainer::set(const char * fielddata, SoInput * in)
{
  const SoFieldData * fields = this->getFieldData();
  assert(fields);

  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoFieldContainer::get(SbString & fielddata, SoOutput * out)
{
  const SoFieldData * fields = this->getFieldData();
  assert(fields);

  COIN_STUB();
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::notify(SoNotList * l)
{
  if (this->donotify) inherited::notify(l);
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldContainer::validateNewFieldValue(SoField * /* field */,
                                        void * /* newval */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  Overloaded from SoBase to make sure field connections into other
  field containers are also accounted for.
 */
void
SoFieldContainer::addWriteReference(SoOutput * out, SbBool isfromfield)
{
  inherited::addWriteReference(out, isfromfield);
  if (isfromfield) return;

  // FIXME: move this into SoField::write(), and generally fix up the
  // write reference counting, so the program flow is about the same
  // during the first write pass as the second. 19990707 mortene.
  const SoFieldData * fd = this->getFieldData();
  for (int i=0; i < fd->getNumFields(); i++) {
    SoField * fieldmaster;
    SoEngineOutput * enginemaster;
    SoVRMLInterpOutput * interpmaster;
    if (fd->getField(this, i)->getConnectedField(fieldmaster))
      fieldmaster->getContainer()->addWriteReference(out, TRUE);
    else if (fd->getField(this, i)->getConnectedEngine(enginemaster))
      enginemaster->getContainer()->addWriteReference(out, TRUE);
    else if (fd->getField(this, i)->getConnectedVRMLInterp(interpmaster))
      interpmaster->getContainer()->addWriteReference(out, TRUE);
  }
}


/*!
  Writes all the fields contained in this instance to the output stream
  within \a out.
 */
void
SoFieldContainer::writeInstance(SoOutput * out)
{
  const SoFieldData * fd = this->getFieldData();
  if (fd) fd->write(out, this);
}

/*!
  Returns \c TRUE if this object is instantiated from one of the native
  Coin classes, \c FALSE if the object's class is outside the standard
  Coin library.
 */
SbBool
SoFieldContainer::getIsBuiltIn(void) const
{
  return this->isBuiltIn;
}


/*!
  Returns a pointer to the class-wide field data storage object
  for this instance. If no fields are present, returns \c NULL.
 */
const SoFieldData *
SoFieldContainer::getFieldData(void) const
{
  return NULL;
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::copyContents(const SoFieldContainer * /* from */,
                               SbBool /* copyconnections */)
{
  COIN_STUB();
}


/*!
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::copyThroughConnection(void) const
{
  COIN_STUB();
  return NULL;
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::initCopyDict(void)
{
  COIN_STUB();
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::addCopy(const SoFieldContainer * const /* orig */,
                          const SoFieldContainer * const /* copy */)
{
  COIN_STUB();
}


/*!
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::checkCopy(const SoFieldContainer * const /* orig */)
{
  COIN_STUB();
  return NULL;
}


/*!
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::findCopy(const SoFieldContainer * const /* orig */,
                           const SbBool /* copyconnections */)
{
  COIN_STUB();
  return NULL;
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::copyDone(void)
{
  COIN_STUB();
}


/*!
  FIXME: write doc
 */
SbBool
SoFieldContainer::readInstance(SoInput * in, unsigned short /* flags */)
{
  const SoFieldData * fd = this->getFieldData();
  if (fd) {
    SbBool notbuiltin;
    // The "error on unknown field" is FALSE, in case we are a group
    // node with children specified in the file.
    return fd->read(in, this, FALSE, notbuiltin);
  }
  return TRUE;
}
