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
  \class SoFieldData Inventor/fields/SoFieldData.h
  \brief The SoFieldData class is a container for a set of fields.
  \ingroup fields

  FIXME: more doc (what is stored, who uses it (one instance pr
  container class), what for (keeping default values and enum values),
  ...)

  \sa SoField
*/

#include <Inventor/fields/SoFieldData.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#if 0 // FIXME: tmp disabled while developing. 19980928 mortene.
#include <Inventor/nodes/SoUnknownNode.h>
#endif // tmp disabled
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SbName.h>
#include <ctype.h>

static const char OPEN_BRACE_CHAR = '[';
static const char CLOSE_BRACE_CHAR = ']';
static const char VALUE_SEPARATOR_CHAR = ',';

struct SoFieldEntry {
  SbName name;
  SoField * ptr;
};

class SoEnumEntry {
public:
  static int growSize;
  SbName typeName;
  int num;
  int arraySize;
  int * vals;
  SbName * names; 

  SoEnumEntry(const SbName & name);
  ~SoEnumEntry();
};

int SoEnumEntry::growSize = 6;

SoEnumEntry::SoEnumEntry(const SbName & name)
  : typeName(name), num(0), arraySize(growSize)
{
  this->vals = new int[this->arraySize];
  this->names = new SbName[this->arraySize];
}

SoEnumEntry::~SoEnumEntry()
{
  delete[] this->vals;
  delete[] this->names;
}


/*!
  \fn SbBool SoFieldData::readFieldTypes(SoInput * in, SoFieldContainer * object)
  FIXME: write doc
*/

/*!
  Constructor.
 */
SoFieldData::SoFieldData(void)
{
}

/*!
  Constructor.
 */
SoFieldData::SoFieldData(const SoFieldData & fd)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  Constructor.
 */
SoFieldData::SoFieldData(const SoFieldData * fd)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  Constructor.
 */
SoFieldData::SoFieldData(int numfields)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  Destructor.
 */
SoFieldData::~SoFieldData()
{
  struct SoFieldEntry * tmpField;
  struct SoEnumEntry * tmpEnum;

  for (int i=0; i<fields.getLength(); i++) {
    tmpField = (struct SoFieldEntry *)fields[i];
    delete tmpField;
  }

  for (int j=0; j<enums.getLength(); j++) {
    tmpEnum = (SoEnumEntry *)enums[j];
    delete tmpEnum;
  }
}

/*!
  FIXME: write doc
 */
void
SoFieldData::addField(SoFieldContainer * defObject, const char * fieldName,
		      const SoField * field)
{
  struct SoFieldEntry * newField = new SoFieldEntry;
  newField->name = (char *)fieldName;
  newField->ptr = (SoField *)field;

  // FIXME: unconst cast for SGI-compile, pederb 990616 
  ((SoField *)field)->setContainer(defObject);

  this->fields.append(newField);
}

/*!
  FIXME: write doc
 */
void
SoFieldData::overlay(SoFieldContainer * to, const SoFieldContainer * from,
		     SbBool copyConnections) const
{
  assert(0 && "FIXME: not implemented");
}

/*!
  Returns number of fields contained within this instance.
 */
int
SoFieldData::getNumFields(void) const
{
  return this->fields.getLength();
}

/*!
  Returns the name of the field at \a index.
 */
const SbName &
SoFieldData::getFieldName(int index) const
{
  return ((SoFieldEntry *)fields[index])->name;
}

/*!
  FIXME: write doc
 */
SoField *
SoFieldData::getField(const SoFieldContainer * object, int index) const
{
  // FIXME: why the SoFieldContainer argument? 19990610 mortene.
  return ((SoFieldEntry *)fields[index])->ptr;
}

/*!
  FIXME: write doc
*/
int
SoFieldData::getIndex(const SoFieldContainer * fc, const SoField * field) const
{
  assert(0 && "FIXME: not implemented");
  return -1;
}

/*!
  FIXME: write doc
*/
void
SoFieldData::addEnumValue(const char * typeNameArg, const char * valNameArg,
			  int val)
{
  struct SoEnumEntry * e = NULL;
  SbName typeName = stripWhite(typeNameArg);
  SbName valName = stripWhite(valNameArg);
  int i;
  for (i=0; i<enums.getLength(); i++) {
    e = (struct SoEnumEntry *) enums[i];
    if (e->typeName == typeName)
      break;
    else
      e = NULL;
  }

  if (e == NULL) {
    e = new SoEnumEntry(typeName);
    enums.append(e);
  }

  if (e->num == e->arraySize) {
    e->arraySize += SoEnumEntry::growSize;
    int * ovals = e->vals;
    SbName * onames = e->names;
    e->vals = new int[e->arraySize];
    e->names = new SbName[e->arraySize];
    for (i=0; i<e->num; i++) {
      e->vals[i] = ovals[i];
      e->names[i] = onames[i];
    }
    delete[] ovals;
    delete[] onames;
  }
  e->vals[e->num] = val;
  e->names[e->num] = valName;
  e->num++;
}

/*!
  FIXME: write doc
*/
void
SoFieldData::getEnumData(const char * typeNameArg, int & num,
			 const int *& vals, const SbName *& names)
{
  SbName typeName = stripWhite(typeNameArg);
  int i;
  for (i=0; i<enums.getLength(); i++) {
    struct SoEnumEntry * e = (struct SoEnumEntry *) enums[i];
    if (e->typeName == typeName) {
      num = e->num;
      vals = e->vals;
      names = e->names;
      return;
    }
  }
  num = 0;
  vals = NULL;
  names = NULL;
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldData::read(SoInput * in, SoFieldContainer * object,
		  SbBool errorOnUnknownField, SbBool & notBuiltIn) const
{
  assert(!in->isBinary() && "FIXME: not implemented yet");

  // FIXME: use object and notBuiltIn somehow. 19990610 mortene.

  SbName fieldName;

  if (fields.getLength() == 0) return TRUE; 

  while (TRUE) {
    if (!in->read(fieldName, TRUE) || !fieldName) return TRUE;

    SbBool foundName;
    if (!this->read(in, object, fieldName, foundName)) return FALSE;

    if (!foundName) {
      if (errorOnUnknownField) {
	SoReadError::post(in, "Unknown field \"%s\"", fieldName.getString());
	return FALSE;
      }
      else {
	in->putBack(fieldName.getString());
	return TRUE;
      }
    }
  }
}

/*!
  FIXME: write doc
*/
SbBool
SoFieldData::read(SoInput * in, SoFieldContainer * object,
		  const SbName & fieldName, SbBool & foundName) const
{
  assert(!in->isBinary() && "FIXME: not implemented yet");

  // FIXME: use object somehow. 19990610 mortene.

  int i;
  for (i = 0; i < fields.getLength(); i++) {
    if (fieldName == getFieldName(i)) {
      if (!getField(object, i)->read(in, fieldName)) return FALSE;
      break;
    }
  }

  if (i == fields.getLength()) foundName = FALSE;
  else foundName = TRUE;

  return TRUE;
}

/*!
  FIXME: write doc
 */
void
SoFieldData::write(SoOutput * out, const SoFieldContainer * object) const
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoFieldData::copy(const SoFieldData * src)
{
  SoField * field, * srcf;
 
  int numf = src->getNumFields();
  for (int i=0; i < numf; i++) {
    // FIXME: need correct container. 19990610 mortene.
    srcf = src->getField(NULL, i);

    field = (SoField *)(srcf->getTypeId().createInstance());
    field->setDefault(srcf->isDefault());
    field->copyFrom(*srcf);

    // FIXME: need correct container. 19990610 mortene.
    this->addField(NULL, src->getFieldName(i).getString(), field);
  }
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldData::isSame(const SoFieldContainer * c1,
		    const SoFieldContainer * c2) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldData::readFieldDescriptions(SoInput * in, SoFieldContainer * object,
				   int numDescriptionsExpected) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoFieldData::writeFieldDescriptions(SoOutput * out,
				    const SoFieldContainer * object) const
{
  assert(0 && "FIXME: not implemented");
}

/*!
  \internal
*/
SbName
SoFieldData::stripWhite(const char * name)
{
  int firstchar = -1;
  int lastchar = -1;
  int lastwhite = -1;
  int i;
  for (i=0; name[i]; i++) {
    if (isspace(name[i]))
      lastwhite = i;
    else {
      if (firstchar == -1) firstchar = i;
      lastchar = i;
    }
  }

  if (lastchar > lastwhite)
    return SbName(&name[firstchar]);

  char buf[500];
  int b;
  for (b=0, i=firstchar; i<=lastchar; i++, b++)
    buf[b] = name[i];
  buf[b] = 0;
  return SbName(buf);
}

SbBool
SoFieldData::readFieldTypes(SoInput * in, SoFieldContainer * object)
{
  assert(!in->isBinary() && "FIXME: not implemented yet");

  SbBool gotChar;
  SbName fieldType, fieldName;
  char c;

  if (! ((gotChar = in->read(c)) || c != OPEN_BRACE_CHAR))
    return FALSE;

  if (in->read(c) && c == CLOSE_BRACE_CHAR)
    return TRUE;

  in->putBack(c);


#if 0 // FIXME: tmp disabled while developing. 19980928 mortene.
  SbBool alreadyHasFields =
    (object->getTypeId() != SoUnknownNode::getClassTypeId() ||
     getNumFields() != 0);
#else // tmp enabled
  SbBool alreadyHasFields = getNumFields() != 0;
#endif // tmp enabled

  while (TRUE) {

    if (!in->read(fieldType, TRUE) || ! in->read(fieldName, TRUE))
      return FALSE;

    if (! alreadyHasFields) {
      SoField * fld;

      // FIXME: use canCreateInstance() and isOfType() for extra
      // safety. 19980915 mortene.
      fld = (SoField *)(SoType::fromName(fieldType).createInstance());

      this->addField(object, fieldName.getString(), fld);
    }

    if (! in->read(c))
      return FALSE;
    if (c == VALUE_SEPARATOR_CHAR) {

      if (in->read(c)) {
	if (c == CLOSE_BRACE_CHAR)
	  return TRUE;
	else
	  in->putBack(c);
      }
    }
    else if (c == CLOSE_BRACE_CHAR)
      return TRUE;
    else
      return FALSE;
  }
}
