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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoFieldContainer Inventor/fields/SoFieldContainer.h
  \brief The SoFieldContainer class is the base class for all classes that
  contain fields.
  \ingroup general

  Classes containing fields are nodes and engines.

  It takes care of keeping track of the fields in a node, or the input fields
  of an engine; reading them, writing them, comparing them, copying them, etc.

  \sa SoField
*/


#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>

/*!
  \var SbBool SoFieldContainer::isBuiltIn
  FIXME: write doc
*/
/*!
  \var SoFieldData SoFieldContainer::fieldData
  FIXME: write doc
*/


SoType SoFieldContainer::classTypeId = SoType::badType();


/*! 
  The constructor. 
*/

SoFieldContainer::SoFieldContainer(void)
  : isBuiltIn(FALSE), donotify(TRUE), fieldData()
{
}

/*! 
  The destructor. 
*/

SoFieldContainer::~SoFieldContainer(void)
{
}

/*! 
  This static method initializes stuff so the SoFieldContainer class can
  function properly. 
*/
void
SoFieldContainer::initClass(void)
{
  // Make sure we init this class only once.
  assert(SoFieldContainer::classTypeId == SoType::badType());

  SoFieldContainer::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "FieldContainer", NULL);
}

/*! 
  This static method cleans up after the SoFieldContainer class, and generally
  undoes what SoFieldContainer::initClass() did. It is present for
  debugging purposes only.
*/
void
SoFieldContainer::cleanClass(void)
{
}

/*! 
  This static method returns the SoType object associated with
  SoFieldContainer objects. 
*/
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
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);
  assert(0 && "FIXME: not implemented");
}

/*! 
  This method checks to see if the fields have their default values, and
  returns \a TRUE if that is the case. 
*/
SbBool
SoFieldContainer::hasDefaultValues(void) const
{
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*! 
  This method returns TRUE if the values of the fields of this and container
  are equal. Fields are assumed to be of the same type. 
*/
SbBool
SoFieldContainer::fieldsAreEqual(const SoFieldContainer * const container) const
{
  const SoFieldData * const myfielddata = getFieldData();
  assert(myfielddata);

  const SoFieldData * const otherfielddata = container->getFieldData();
  assert(otherfielddata);

  assert(0 && "FIXME: not implemented");
  return FALSE;
}


/*! 
  This method copies the field values from container into this. The fields
  are assumed to be of the same type. The copyConnections flag decides
  whether the field connections are to be copied aswell.
*/
void
SoFieldContainer::copyFieldValues(const SoFieldContainer * const container,
				  SbBool copyConnections)
{
  // FIXME: handle copyConnections flag. 19990403 mortene.
  assert(!copyConnections);

  assert(container);
  const SoFieldData * const myfielddata = this->getFieldData();
  assert(myfielddata);
  const SoFieldData * const otherfielddata = container->getFieldData();
  assert(otherfielddata);

  const int numFields = myfielddata->getNumFields();

  assert(numFields == otherfielddata->getNumFields()); // or just return? 

  for (int i = 0; i < numFields; i++)
    myfielddata->getField(this, i)->copyFrom(*(otherfielddata->getField(container, i)));
}


/*! 
  This method parses the values of one or more fields from the
  \a fieldDataString argument. 

  The fields must be in the same format as inside Inventor files.
  \a TRUE is returned upon success, and \a FALSE otherwise. 

  \sa get()
*/
SbBool
SoFieldContainer::set(const char * const fieldDataString)
{
  return set(fieldDataString, NULL);
}

/*! 
  This methods stores the field data in the given fieldDataString argument. 
  The format is the same as SoFieldContainer::set() expects. 

  \sa set()
*/
void
SoFieldContainer::get(SbString & fieldDataString)
{
  get(fieldDataString, NULL);
}


/*! 
  This method adds the fields in this field container to the list argument,
  and returns the number of fields added. 
  
  The virtual nature of this function means that it can be overridden to
  e.g. hide private fields. 
*/
int
SoFieldContainer::getFields(SoFieldList & list) const
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write doc
 */
int
SoFieldContainer::getAllFields(SoFieldList & list) const
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*! 
  This method returns a pointer to the field with name \a fieldName, or
  \a NULL if no such field exists. 
*/
SoField *
SoFieldContainer::getField(const SbName & fieldName) const
{
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);
  const int numFields = fields->getNumFields();
  for (int i = 0; i < numFields; i++)
    if (fieldName == fields->getFieldName(i))
      return fields->getField(this, i);
  return (SoField *) NULL;
}

/*! 
  This method returns a pointer to the eventIn with name \a fieldName,
  or \a NULL if no such eventIn exists. 
*/
SoField *
SoFieldContainer::getEventIn(const SbName & fieldName) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*! 
  This method returns a pointer to the eventOut with name \a fieldName,
  or \a NULL if no such eventOut exists. 
*/
SoField *
SoFieldContainer::getEventOut(const SbName & fieldName) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}


/*! 
  Finds the name of the given \a field and returns the value in the
  \a fieldName argument. 

  \a TRUE is returned if the field is contained within this instance,
  and \a FALSE otherwise. 
*/
SbBool
SoFieldContainer::getFieldName(const SoField * const field,
			       SbName & fieldName) const
{
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);
  const int numFields = fields->getNumFields();
  for (int i = 0; i < numFields; i++) {
    if (field == fields->getField(this, i)) {
      fieldName = fields->getFieldName(i);
      return TRUE;
    }
  }
  return FALSE;
}


/*! 
  This method sets whether notification will be propagated on changing the
  values of the contained fields.  The old value of the setting is returned. 

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
  FIXME: write doc
 */
SbBool
SoFieldContainer::set(const char * const fieldDataString,
		      SoInput * const dictIn)
{
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);

  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*! 
  FIXME: write doc
 */
void
SoFieldContainer::get(SbString & fieldDataString, SoOutput * const dictOut)
{
  const SoFieldData * const fields = this->getFieldData();
  assert(fields);

  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
void
SoFieldContainer::notify(SoNotList * list)
{
  if (this->donotify) inherited::notify(list);
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldContainer::validateNewFieldValue(SoField * pField, void * newValue)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*! 
  FIXME: write doc
 */
void
SoFieldContainer::addWriteReference(SoOutput * out, SbBool isFromField)
{
  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
void
SoFieldContainer::writeInstance(SoOutput * out)
{
  assert(0 && "FIXME: not implemented");
}

/*! 
  FIXME: write doc
 */
SbBool
SoFieldContainer::getIsBuiltIn(void) const
{
  return this->isBuiltIn;
}


/*! 
  Must be overloaded in subclasses which actually contains fields. The
  default method returns \a NULL.

  FIXME: implementation is not yet correct. 19990403 mortene.
 */
const SoFieldData *
SoFieldContainer::getFieldData(void) const
{
#if 0 // breaks SoGroup::readInstance(). 19990403 mortene.
  if (this->fieldData.getNumFields() < 1) return NULL;
  else return &this->fieldData;
#else
  return &this->fieldData;
#endif
}


/*!
  FIXME: write doc
 */
void
SoFieldContainer::copyContents(const SoFieldContainer * fromFC,
			       SbBool copyConnections)
{
  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::copyThroughConnection(void) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}


/*! 
  FIXME: write doc
 */
void
SoFieldContainer::initCopyDict(void)
{
  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
void
SoFieldContainer::addCopy(const SoFieldContainer * const orig,
			  const SoFieldContainer * const copy)
{
  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::checkCopy(const SoFieldContainer * const orig)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}


/*! 
  FIXME: write doc
 */
SoFieldContainer *
SoFieldContainer::findCopy(const SoFieldContainer * const orig,
			   const SbBool copyConnections)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}


/*! 
  FIXME: write doc
 */
void
SoFieldContainer::copyDone(void)
{
  assert(0 && "FIXME: not implemented");
}


/*! 
  FIXME: write doc
 */
SbBool
SoFieldContainer::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: proof-read code (it has just been moved verbatim over from
  // SoNode). 19990403 mortene.

  // FIXME: handle "flags". 19990403 mortene.

  SbName typeString;
  if (in->read(typeString, TRUE)) {
    if (typeString == "fields") {
      if (!this->fieldData.readFieldTypes(in, this)) {
	SoReadError::post(in, "Bad field specifications for node");
	return FALSE;
      }
    }
    else {
      in->putBack(typeString.getString());
    }
  }

  SbBool notbuiltin;
  if (!this->fieldData.read(in, this, TRUE, notbuiltin)) return FALSE;
  return TRUE;
}
