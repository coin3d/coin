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
  \class SoFieldData Inventor/fields/SoFieldData.h
  \brief The SoFieldData class is a container for a set of fields.
  \ingroup fields

  FIXME: more doc (what is stored, who uses it (one instance pr
  container class), what for (keeping default values and enum values),
  ...)

  \sa SoField
*/

#include <Inventor/fields/SoFieldData.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <ctype.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

static const char OPEN_BRACE_CHAR = '[';
static const char CLOSE_BRACE_CHAR = ']';
static const char VALUE_SEPARATOR_CHAR = ',';

class SoFieldEntry {
public:
  SoFieldEntry(const char * n, int offset) : name(n), ptroffset(offset) { }
  // Copy constructors.
  SoFieldEntry(const SoFieldEntry * fe) { this->copy(fe); }
  SoFieldEntry(const SoFieldEntry & fe) { this->copy(&fe); }

  SbName name;
  int ptroffset;

private:
  void copy(const SoFieldEntry * fe)
    {
      this->name = fe->name;
      this->ptroffset = fe->ptroffset;
    }
};

class SoEnumEntry {
public:
  SoEnumEntry(const SbName & name);
  ~SoEnumEntry();
  // Copy constructors.
  SoEnumEntry(const SoEnumEntry * ee) { this->copy(ee); }
  SoEnumEntry(const SoEnumEntry & ee) { this->copy(&ee); }

  static int growsize;
  SbName nameoftype;
  int num;
  int arraysize;
  int * vals;
  SbName * names;

private:
  void copy(const SoEnumEntry * ee)
    {
      this->nameoftype = ee->nameoftype;
      this->arraysize = ee->arraysize;
      this->num = ee->num;

      this->vals = new int[this->arraysize];
      this->names = new SbName[this->arraysize];

      for (int i=0; i < this->num; i++) {
        this->vals[i] = ee->vals[i];
        this->names[i] = ee->names[i];
      }
    }
};

int SoEnumEntry::growsize = 6;

SoEnumEntry::SoEnumEntry(const SbName & name)
  : nameoftype(name), num(0), arraysize(SoEnumEntry::growsize)
{
  this->vals = new int[this->arraysize];
  this->names = new SbName[this->arraysize];
}

SoEnumEntry::~SoEnumEntry()
{
  delete[] this->vals;
  delete[] this->names;
}


/*!
  Constructor.
 */
SoFieldData::SoFieldData(void)
{
}

/*!
  Copy constructor.
 */
SoFieldData::SoFieldData(const SoFieldData & fd)
{
  this->copy(&fd);
}

/*!
  Copy constructor by pointer values. Handles \c NULL pointers by
  behaving like the default constructor.
 */
SoFieldData::SoFieldData(const SoFieldData * fd)
{
  if (fd) this->copy(fd);
}

/*!
  Constructor. Takes an indication on the number of fields which
  should be stored.
 */
SoFieldData::SoFieldData(int /* numfields */)
{
  COIN_STUB();
}

/*!
  Destructor.
 */
SoFieldData::~SoFieldData()
{
  struct SoFieldEntry * tmpField;
  SoEnumEntry * tmpEnum;

  for (int i=0; i < this->fields.getLength(); i++) {
    tmpField = (struct SoFieldEntry *)fields[i];
    delete tmpField;
  }

  for (int j=0; j < this->enums.getLength(); j++) {
    tmpEnum = (SoEnumEntry *)enums[j];
    delete tmpEnum;
  }
}

/*!
  FIXME: write doc
 */
void
SoFieldData::addField(SoFieldContainer * base, const char * name,
                      const SoField * field)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoFieldData::addField",
                         "class: ``%s'', field: ``%s''",
                         base->getTypeId().getName().getString(),
                         name);
#endif // debug
  char * vbase = (char *)base;
  char * vfield = (char *)field;
  struct SoFieldEntry * newfield = new SoFieldEntry(name, vfield - vbase);
  this->fields.append(newfield);
}

/*!
  FIXME: write doc
 */
void
SoFieldData::overlay(SoFieldContainer * /* to */,
                     const SoFieldContainer * /* from */,
                     SbBool /* copyConnections */) const
{
  COIN_STUB();
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
  char * fieldptr = (char *)object;
  fieldptr += this->fields[index]->ptroffset;
  return (SoField *)fieldptr;
}

/*!
  FIXME: write doc
*/
int
SoFieldData::getIndex(const SoFieldContainer * /* fc */,
                      const SoField * /* field */) const
{
  COIN_STUB();
  return -1;
}

/*!
  FIXME: write doc
*/
void
SoFieldData::addEnumValue(const char * typeNameArg, const char * valNameArg,
                          int val)
{
  SoEnumEntry * e = NULL;
  SbName typeName = stripWhite(typeNameArg);
  SbName valName = stripWhite(valNameArg);
  int i;
  for (i=0; i<enums.getLength(); i++) {
    e = (SoEnumEntry *) enums[i];
    if (e->nameoftype == typeName)
      break;
    else
      e = NULL;
  }

  if (e == NULL) {
    e = new SoEnumEntry(typeName);
    enums.append(e);
  }

  if (e->num == e->arraysize) {
    e->arraysize += SoEnumEntry::growsize;
    int * ovals = e->vals;
    SbName * onames = e->names;
    e->vals = new int[e->arraysize];
    e->names = new SbName[e->arraysize];
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
    SoEnumEntry * e = (SoEnumEntry *) enums[i];
    if (e->nameoftype == typeName) {
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
                  SbBool errorOnUnknownField, SbBool & /* notBuiltIn */) const
{
  // FIXME: use notBuiltIn somehow. 19990610 mortene.

  if (in->isBinary()) {
    int numfields;
    if (!in->read(numfields)) {
      SoReadError::post(in, "premature EOF");
      return FALSE;
    }

    // FIXME: invalid check? (same field may be written several
    // times?) 19990711 mortene.
    if (numfields > this->fields.getLength()) {
      SoReadError::post(in, "invalid number of fields: %d", numfields);
      return FALSE;
    }

    if (numfields == 0) return TRUE;

    for (int i=0; i < numfields; i++) {
      SbName fieldName;
      // FIXME: return TRUE? Strange. Investigate closer. 19991230 mortene.
      if (!in->read(fieldName, TRUE) || !fieldName) return TRUE;

#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoFieldData::read",
                             "fieldname: '%s'", fieldName.getString());
#endif // debug
      SbBool foundName;
      if (!this->read(in, object, fieldName, foundName)) return FALSE;

      // FIXME: handle case for binary format. 19990711 mortene.
      assert(foundName && "FIXME: doesn't work in binary mode yet");
    }
  }
  else {
    while (TRUE) {
      SbName fieldname;
      // FIXME: return TRUE? Strange. Investigate closer. 19991230 mortene.
      if (!in->read(fieldname, TRUE) || !fieldname) return TRUE;

      SbBool foundname;
      if (!this->read(in, object, fieldname, foundname) && foundname)
        return FALSE;

      if (!foundname) {
        if (errorOnUnknownField) {
          SoReadError::post(in, "Unknown field \"%s\"", fieldname.getString());
          return FALSE;
        }
        else {
          in->putBack(fieldname.getString());
          return TRUE;
        }
      }
    }
  }

  return TRUE;
}

/*!
  Find field \a fieldName in \a object, and if it is available, sets
  \a foundName to \c TRUE and tries to read the field specification
  from \a in. If \a foundName is returned as \c TRUE, the return value
  says whether or not the field specification could be read without
  any problems.

  If \a fieldName is not part of \a object, returns \c FALSE with \a
  foundName also set to \c FALSE.
*/
SbBool
SoFieldData::read(SoInput * in, SoFieldContainer * object,
                  const SbName & fieldName, SbBool & foundName) const
{
  for (int i = 0; i < this->fields.getLength(); i++) {
    if (fieldName == this->getFieldName(i)) {
      foundName = TRUE;
      return this->getField(object, i)->read(in, fieldName);
    }
  }

  foundName = FALSE;
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoFieldData::write(SoOutput * /* out */,
                   const SoFieldContainer * /* object */) const
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoFieldData::copy(const SoFieldData * src)
{
#if 0 // OBSOLETED: we don't store copies of the actual fields. 19991230 mortene.
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
#else // NEW CODE
  // FIXME: dealloc entries in this->fields and this->enums
  // before copy operation. 19991230 mortene.
  int i;
  for (i=0; i < src->fields.getLength(); i++) {
    this->fields.append(new SoFieldEntry(src->fields[i]));
  }
  for (i=0; i < src->enums.getLength(); i++) {
    this->enums.append(new SoEnumEntry(src->enums[i]));
  }
#endif // NEW CODE
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldData::isSame(const SoFieldContainer * /* c1 */,
                    const SoFieldContainer * /* c2 */) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoFieldData::readFieldDescriptions(SoInput * /* in */, SoFieldContainer * /* object */,
                                   int /* numDescriptionsExpected */) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoFieldData::writeFieldDescriptions(SoOutput * /* out */,
                                    const SoFieldContainer * /* object */) const
{
  COIN_STUB();
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

  // FIXME: this looks like it has the potential to overflow. 19991229 mortene.
  char buf[500];
  int b;
  for (b=0, i=firstchar; i<=lastchar; i++, b++)
    buf[b] = name[i];
  buf[b] = 0;
  return SbName(buf);
}

/*!
  \fn SbBool SoFieldData::readFieldTypes(SoInput * in, SoFieldContainer * object)
  FIXME: write doc
*/
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
