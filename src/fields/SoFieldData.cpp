/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoFieldData SoFieldData.h Inventor/fields/SoFieldData.h
  \brief The SoFieldData class is a container for a prototype set of fields.
  \ingroup fields

  This class is instantiated once for each class of objects which use
  fields, and which needs to be able to import and export them.

  Each field of a class is stored with the name it has been given
  within its "owner" class and a pointer offset to the dynamic
  instance of the field itself.

  Enumeration sets are stored with (name, value) pairs, to make it
  possible to address, read and save enum type fields by name.

  It is unlikely that application programmers should need to use any
  of the methods of this class directly.

  \sa SoField, SoFieldContainer */

/*¡
  Some methods related to reading VRML 2 files are missing.
 */


/* IMPORTANT NOTE:
 * If you make any changes (bugfixes, improvements) in this class,
 * remember to also check the SoEngineOutputData class, as it is
 * heavily based on this class.
 */

#include <Inventor/fields/SoFieldData.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/SbName.h>
#include <coindefs.h> // COIN_STUB()
#include <ctype.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

static const char OPEN_BRACE_CHAR = '[';
static const char CLOSE_BRACE_CHAR = ']';
static const char VALUE_SEPARATOR_CHAR = ',';

// Internal classes, start /////////////////////////////////////////////////

#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoFieldEntry {
public:
  SoFieldEntry(const char * n, int offset) : name(n), ptroffset(offset) { }
  // Copy constructors.
  SoFieldEntry(const SoFieldEntry * fe) { this->copy(fe); }
  SoFieldEntry(const SoFieldEntry & fe) { this->copy(&fe); }

  int operator==(const SoFieldEntry & fe) const {
    // don't consider ptroffset here, since this will not be equal
    // for fields containers with dynamic fields.
    return (this->name == fe.name);
  }
  int operator!=(const SoFieldEntry & fe) const { 
    return ! operator==(&fe); 
  }

  SbName name;
  int ptroffset;

private:
  void copy(const SoFieldEntry * fe) {
    this->name = fe->name;
    this->ptroffset = fe->ptroffset;
  }
};

class SoEnumEntry {
public:
  SoEnumEntry(const SbName & name) : nameoftype(name) { }
  // Copy constructors.
  SoEnumEntry(const SoEnumEntry * ee) { this->copy(ee); }
  SoEnumEntry(const SoEnumEntry & ee) { this->copy(&ee); }
  
  int operator==(const SoEnumEntry & ee) const {
    return ((this->nameoftype == ee.nameoftype) &&
            (this->names == ee.names) && (this->values == ee.values));
  }
  int operator!=(const SoEnumEntry & ee) const { return ! operator==(&ee); }

  SbName nameoftype;
  SbList<SbName> names;
  SbList<int> values;

private:
  void copy(const SoEnumEntry * ee) {
    this->nameoftype = ee->nameoftype;
    this->names = ee->names;
    this->values = ee->values;
  }
};

#endif // DOXYGEN_SKIP_THIS

// Internal classes, end ///////////////////////////////////////////////////



/*!
  Default constructor.
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
  Copy constructor taking a pointer value as an argument. Handles \c
  NULL pointers by behaving like the default constructor.
*/
SoFieldData::SoFieldData(const SoFieldData * fd)
{
  if (fd) this->copy(fd);
}

/*!
  Constructor. Takes an indication on the number of fields which
  should be stored, to make sure the memory handling is efficient.
 */
SoFieldData::SoFieldData(int /* numfields */)
{
  // Ignore the argument, I don't think there's any point in doing
  // optimization here. 19991231 mortene.
}

/*!
  Destructor.
 */
SoFieldData::~SoFieldData()
{
  this->freeResources();
}

// Empties internal lists, while deallocating the memory used for the
// entries.
void
SoFieldData::freeResources(void)
{
  for (int i=0; i < this->fields.getLength(); i++) delete this->fields[i];
  this->fields.truncate(0);

  for (int j=0; j < this->enums.getLength(); j++) delete this->enums[j];
  this->enums.truncate(0);
}

/*!
  Add a new field to our internal list.

  The \a name will be stored along with an pointer offset between \a
  base and \a field, which will be valid for all instances of the
  class type of \a base.
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
  this->fields.append(new SoFieldEntry(name, vfield - vbase));
}

/*!
  Copy fields from container \a from to container \a to. If
  \a copyconnections is \c TRUE, we'll also copy the connections
  field \a from has made.

  If you think the method signature is a bit strange, you're correct.
  This should really have been a static method (the owner \c this
  instance of the method isn't used at all, due to how the internal
  representation of field template list are stored), but for unknown
  reasons this is a dynamic method in Open Inventor. So also in Coin,
  to keep compatibility.
 */
void
SoFieldData::overlay(SoFieldContainer * to, const SoFieldContainer * from,
                     SbBool copyconnections) const
{
  if (to == from) return;

  const SoFieldData * fd0 = to->getFieldData();
  const SoFieldData * fd1 = from->getFieldData();
  if (!fd0 && !fd1) return;

  // The field containers should have equal SoFieldData sets.
  assert(fd0 && fd1 && *fd0==*fd1);

  int num = fd0->getNumFields();
  for (int i=0; i < num; i++) {
    SoField * field0 = fd0->getField(to, i);
    SoField * field1 = fd1->getField(from, i);
    field0->copyFrom(*field1);
    if (field1->isDefault()) field0->setDefault(TRUE);
    field0->fixCopy(copyconnections);
    if (copyconnections) field0->copyConnection(field1);
  }
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
  return this->fields[index]->name;
}

/*!
  Returns a pointer to the field at \a index within the \a object
  instance.
 */
SoField *
SoFieldData::getField(const SoFieldContainer * object, int index) const
{
  assert(index >= 0 && index < this->fields.getLength());
  char * fieldptr = (char *)object;
  fieldptr += this->fields[index]->ptroffset;
  return (SoField *)fieldptr;
}

/*!
  Returns the internal index value of \a field in \a fc. If \a field
  is not part of \a fc, returns -1.
*/
int
SoFieldData::getIndex(const SoFieldContainer * fc, const SoField * field) const
{
  char * vbase = (char *)fc;
  char * vfield = (char *)field;
  int ptroffset = vfield - vbase;

  for (int i=0; i < this->fields.getLength(); i++)
    if (this->fields[i]->ptroffset == ptroffset) return i;

  return -1;
}

/*!
  Either adds a new enum set (with an initial member), or adds a new value
  member to an existing enum set.
*/
void
SoFieldData::addEnumValue(const char * enumname, const char * valuename,
                          int value)
{
  SoEnumEntry * e = NULL;

  for (int i=0; !e && (i < this->enums.getLength()); i++) {
    if (this->enums[i]->nameoftype == enumname) e = this->enums[i];
  }

  if (e == NULL) {
    e = new SoEnumEntry(enumname);
    this->enums.append(e);
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoFieldData::addEnumValue",
                         "enumname: %s, valuename: %s, value: %d",
                         enumname, valuename, value);
#endif // debug

  assert(e->names.find(valuename) == -1);
  e->names.append(valuename);
  // Note that an enum can have several names mapping to the same
  // value. 20000101 mortene.
  e->values.append(value);
}

/*!
  Returns the \a names and \a values of enumeration entry with the \a
  enumname. The number of (name, value) pairs available in the
  enumeration is returned in \a num.
*/
void
SoFieldData::getEnumData(const char * enumname, int & num,
                         const int *& values, const SbName *& names)
{
  num = 0;
  values = NULL;
  names = NULL;

  for (int i=0; i < this->enums.getLength(); i++) {
    SoEnumEntry * e = this->enums[i];
    if (e->nameoftype == enumname) {
      num = e->names.getLength();
      if (num) {
        assert(e->names.getLength() == e->values.getLength());
        names = e->names.getArrayPtr();
        values = e->values.getArrayPtr();
      }
      return;
    }
  }
}

/*!
  Read field data from the \a in stream for fields belonging to \a
  object. Returns \c TRUE if everything went ok, or \c FALSE if any
  error conditions occurs.

  \a erroronunknownfield decides whether or not \c FALSE should be
  returned if a name identifier not recognized as a fieldname of \a
  object is encountered. Note that \a erroronunknownfield should be \c
  FALSE if \a object is a container with child objects, otherwise the
  code will fail upon the first child name specification.

  If \a notbuiltin is \c TRUE on return, \a object is an unknown node
  or engine type. Unknown nodes are recognized by the \c fields
  keyword first in their file format definition, and unknown engines
  by the \c inputs keyword.

*/
SbBool
SoFieldData::read(SoInput * in, SoFieldContainer * object,
                  SbBool erroronunknownfield, SbBool & notbuiltin) const
{
  notbuiltin = FALSE;

  if (in->isBinary()) {
    uint32_t fieldsval;
    if (!in->read(fieldsval)) {
      SoReadError::post(in, "Premature EOF");
      return FALSE;
    }

    uint8_t numfields = fieldsval & 0xff;
    uint8_t fieldflags = fieldsval >> 8;

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoFieldData::read", "0x%08x => 0x%02x 0x%02x",
                           fieldsval, fieldflags, numfields);
#endif // debug

    // Unknown node type, must read field descriptions.
    if (fieldflags & SoFieldData::NOTBUILTIN) {
      if (!this->readFieldDescriptions(in, object, numfields)) return FALSE;
    }

    // Check for more flags, in case there's any we've missed.
    if (fieldflags & ~(SoFieldData::NOTBUILTIN)) {
      SoReadError::post(in,
                        "Unknown flags in control word: 0x%02x, "
                        "please report to coin-support@sim.no",
                        fieldflags);
    }

#if COIN_DEBUG
    if (numfields > this->fields.getLength())
      SoDebugError::postWarning("SoFieldData::read",
                                "Suspicious number of fields: %d", numfields);
#endif // COIN_DEBUG

    if (numfields == 0) return TRUE;

    for (int i=0; i < numfields; i++) {
      SbName fieldname;
      if (!in->read(fieldname, TRUE) || !fieldname) {
        SoReadError::post(in, "Couldn't read field number %d", i);
        return FALSE;
      }

#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoFieldData::read",
                             "fieldname: '%s'", fieldname.getString());
#endif // debug
      SbBool foundname;
      if (!this->read(in, object, fieldname, foundname)) {
        if (!foundname) SoReadError::post(in, "Unknown field \"%s\"",
                                          fieldname.getString());
        return FALSE;
      }
    }
  }
  else { // ASCII format.
    SbBool firstidentifier = TRUE;
    while (TRUE) {
      SbName fieldname;
      if (!in->read(fieldname, TRUE)) return TRUE; // Terminates loop on "}"

      SbBool readok;
      if (in->checkISReference(object, fieldname, readok)) {
        continue; // skip to next field
      }
      if (!readok) {
        SoReadError::post(in, "Error while searching for IS keyword for field \"%s\"",
                          fieldname.getString());
        return FALSE;
      }
      // This should be caught in SoInput::read(SbName, SbBool).
      assert(fieldname != "");

      SbBool foundname;
      if (!this->read(in, object, fieldname, foundname) && foundname)
        return FALSE;

      if (!foundname) {
        // User extension node with explicit field definitions.
        if (firstidentifier && fieldname == "fields") {
          notbuiltin = TRUE;
          if (!this->readFieldDescriptions(in, object, 0)) return FALSE;
        }
        // User extension engine with explicit input field definitions.
        else if (firstidentifier && fieldname == "inputs") {
          notbuiltin = TRUE;
          // FIXME: read input defs and inputs (and output
          // defs?). 20000102 mortene.
          COIN_STUB();
          return FALSE;
        }
        else if (erroronunknownfield) {
          SoReadError::post(in, "Unknown field \"%s\"", fieldname.getString());
          return FALSE;
        }
        else {
          in->putBack(fieldname.getString());
          return TRUE;
        }
      }
      firstidentifier = FALSE;
    }
  }

  return TRUE;
}

/*!
  Find field \a fieldname in \a object, and if it is available, set
  \a foundname to \c TRUE and try to read the field specification
  from \a in. If \a foundname is set to \c TRUE, the return value
  says whether or not the field specification could be read without
  any problems.

  If \a fieldname is not part of \a object, returns \c FALSE with \a
  foundname also set to \c FALSE.
*/
SbBool
SoFieldData::read(SoInput * in, SoFieldContainer * object,
                  const SbName & fieldname, SbBool & foundname) const
{
  for (int i = 0; i < this->fields.getLength(); i++) {
    if (fieldname == this->getFieldName(i)) {
      foundname = TRUE;
      return this->getField(object, i)->read(in, fieldname);
    }
  }

  foundname = FALSE;
  return FALSE;
}

/*!
  Write to \a out field names and field values for the fields of
  \a object.
 */
void
SoFieldData::write(SoOutput * out, const SoFieldContainer * object) const
{
  uint16_t i;

  if (out->getStage() == SoOutput::COUNT_REFS) {
    // Handle first stage of write operations.
    for (i=0; i < this->getNumFields(); i++)
      this->getField(object, i)->write(out, this->getFieldName(i));
    return;
  }
  // Ok, we've passed the first write stage and is _really_ writing.


  // Make sure all fields get written on user extension nodes (or
  // SoUnknownNode), even if they have their default flags set to
  // TRUE.
  SbList<SbBool> defaultflags(this->getNumFields() ? this->getNumFields() : 1);
  if (!object->getIsBuiltIn()) {
    for (i=0; i < this->getNumFields(); i++) {
      SoField * f = this->getField(object, i);
      defaultflags.append(f->isDefault());
      f->setDefault(FALSE);
    }
  }

  // FIXME: is this really the best place to write the flags +
  // numfields value? 20000102 mortene.

  if (out->isBinary()) {
    // Check how many fields will be written.
    uint8_t numfields = 0;
    for (int j=0; j < this->getNumFields(); j++) {
      const SoField * f = this->getField(object, j);
      if (f->shouldWrite()) {
        // This is an amazingly lame limitation, but we can't really
        // fix it without breaking compatibility with the SGI binary
        // .iv format.  (The moral of the story is: avoid binary
        // .iv-files.)
        assert((numfields < 255) &&
               "too many fields to handle with binary .iv format");
        numfields++;
      }
    }

    uint16_t fieldflags = 0x0000;
    // FIXME: take care of setting flags for SoUnknownEngines, if
    // necessary. 20000102 mortene.
    if (!object->getIsBuiltIn()) fieldflags |= SoFieldData::NOTBUILTIN;

    uint32_t w = fieldflags;
    w <<= 8;
    w |= numfields;

    out->write(w);
  }

  // FIXME: write descriptions for SoUnknownEngine, if
  // necessary. 20000102 mortene.
  if (!object->getIsBuiltIn()) this->writeFieldDescriptions(out, object);

  for (i=0; i < this->getNumFields(); i++)
    this->getField(object, i)->write(out, this->getFieldName(i));

  // Reset the default flags if we're writing a user extension node
  // (or SoUnknownNode).
  if (!object->getIsBuiltIn()) {
    for (i=0; i < this->getNumFields(); i++) {
      this->getField(object, i)->setDefault(defaultflags[i]);
    }
  }
}

/*!
  Copy contents of \a src into this instance.

  If there was any data set up in this instance before the method was
  called, the old data is removed first.

  Note that this only copies the field set template specification from
  \a src, \e not any actual fields.
 */
void
SoFieldData::copy(const SoFieldData * src)
{
  this->freeResources();

  int i;
  for (i=0; i < src->fields.getLength(); i++) {
    this->fields.append(new SoFieldEntry(src->fields[i]));
  }
  for (i=0; i < src->enums.getLength(); i++) {
    this->enums.append(new SoEnumEntry(src->enums[i]));
  }
}

/*!
  Compares \a c1 and \a c2 to see if they have the same field data set
  and if the fields of \a c1 have the same values as the fields of \a c2.

  Field connections are not considered (i.e. we will return \c TRUE if
  the values of the fields of \a c1 are equal to the fields of \a c2,
  even if they differ in how they have made connections to other
  fields).

  If you think the method signature is a bit strange, you're correct.
  This should really have been a static method (the owner \c this
  instance of the method isn't used at all, due to how the internal
  representation of field template list are stored), but for unknown
  reasons this is a dynamic method in Open Inventor. So also in Coin,
  to keep compatibility.

 */
SbBool
SoFieldData::isSame(const SoFieldContainer * c1,
                    const SoFieldContainer * c2) const
{
  if (c1 == c2) return TRUE;

  const SoFieldData * fd1 = c1->getFieldData();
  const SoFieldData * fd2 = c2->getFieldData();
  if (!fd1 && !fd2) return TRUE;
  if (!fd1 || !fd2) return FALSE;
  if (*fd1 != *fd2) return FALSE;

  int num = fd1->getNumFields();
  for (int i=0; i < num; i++)
    if (*(fd1->getField(c1, i)) != *(fd2->getField(c2, i))) return FALSE;

  return TRUE;
}

/*!
  Reads a set of field specifications from \a in for an unknown nodeclass type,
  in the form "[ FIELDCLASS FIELDNAME, FIELDCLASS FIELDNAME, ... ]".

  \a numdescriptionsexpected is used for binary format import to know
  how many descriptions should be parsed.

 */
SbBool
SoFieldData::readFieldDescriptions(SoInput * in, SoFieldContainer * object,
                                   int numdescriptionsexpected) const
{
  // These two macros are convenient for reading with error detection.
#define READ_CHAR(c) \
    if (!in->read(c)) { \
      SoReadError::post(in, "Premature end of file"); \
      return FALSE; \
    }

  const SbName EVENTIN("eventIn");
  const SbName EVENTOUT("eventOut");
  const SbName FIELD("field");
  const SbName EXPOSEDFIELD("exposedField");
  const SbName IS("IS");
  
  char c;
  if (!in->isBinary()) {
    READ_CHAR(c);
    if (c != OPEN_BRACE_CHAR) {
      SoReadError::post(in, "Expected '%c', got '%c'", OPEN_BRACE_CHAR, c);
      return FALSE;
    }
  }

  for (int j=0; !in->isBinary() || (j < numdescriptionsexpected); j++) {

    if (!in->isBinary()) {
      READ_CHAR(c);
      if (c == CLOSE_BRACE_CHAR) return TRUE;
      else in->putBack(c);
    }

    SbName fieldtypename;
    
    if (!in->read(fieldtypename, TRUE)) {
      SoReadError::post(in, "Couldn't read name of field type");
      return FALSE;
    }

    SbName fieldtype("");
    if (fieldtypename == EVENTIN ||
        fieldtypename == EVENTOUT ||
        fieldtypename == FIELD ||
        fieldtypename == EXPOSEDFIELD) { 
      fieldtype = fieldtypename;
      if (!in->read(fieldtypename, TRUE)) {
        SoReadError::post(in, "Couldn't read name of field type");
        return FALSE;
      }
    }
    
    SoType type = SoType::fromName(fieldtypename.getString());
    if ((type == SoType::badType()) ||
        !type.isDerivedFrom(SoField::getClassTypeId())) {
      SoReadError::post(in, "Unknown field type '%s'",
                        fieldtypename.getString());
      return FALSE;
    }
    else if (!type.canCreateInstance()) {
      SoReadError::post(in, "Abstract class type '%s'", fieldtypename.getString());
      return FALSE;
    }
    
    SbName fieldname;
    if (!in->read(fieldname, TRUE)) {
      SoReadError::post(in, "Couldn't read name of field");
      return FALSE;
    }


#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoFieldData::readFieldDescriptions",
                           "type: ``%s'', name: ``%s''",
                           fieldtypename.getString(), fieldname.getString());
#endif // debug

    SoField * newfield = NULL;
    for (int i=0; !newfield && (i < this->fields.getLength()); i++) {
      if (this->fields[i]->name == fieldname) {
        newfield = this->getField(object, i);
      }
    }
    if (!newfield) {
      // Cast away const -- ugly.
      SoFieldData * thisp = (SoFieldData *)this;
      newfield = (SoField *)type.createInstance();
      newfield->setContainer(object);
      newfield->setDefault(TRUE);
      thisp->addField(object, fieldname.getString(), newfield);
    }

    if (fieldtype == EVENTIN || fieldtype == EVENTOUT) {
      if (fieldtype == EVENTIN) {
        newfield->setFieldType(SoField::EVENTIN_FIELD);
      }
      else {
        newfield->setFieldType(SoField::EVENTOUT_FIELD);        
      }
      SbBool readok;
      (void) in->checkISReference(object, fieldname.getString(), readok);
      if (!readok) {
        SoReadError::post(in, "Error while searching for IS keyword for field '%s'",
                          fieldname.getString());
        return FALSE;
      }
    }
    else if (fieldtype == FIELD || fieldtype == EXPOSEDFIELD) {
      if (fieldtype == EXPOSEDFIELD) {
        newfield->setFieldType(SoField::EXPOSED_FIELD);
      }
      if (!newfield->read(in, fieldname)) {
        SoReadError::post(in, "Unable to read value for field '%s'",
                          fieldname.getString());
        return FALSE;
      }
    }

    SbBool readok;
    (void) in->checkISReference(object, fieldname, readok);
    if (!readok) {
      SoReadError::post(in, "Unable to search for IS keyword");
      return FALSE;
    }
    if (!in->isBinary()) {
      READ_CHAR(c);
      if (c != VALUE_SEPARATOR_CHAR) in->putBack(c);
      // (Allow missing value separators (i.e. no "," character
      // between two field descriptions)).
    }
  }

#undef READ_CHAR

  return TRUE;
}


/*!
  Write a set of field specifications to \a out for an unknown nodeclass type,
  in the form "[ FIELDCLASS FIELDNAME, FIELDCLASS FIELDNAME, ... ]".
 */
void
SoFieldData::writeFieldDescriptions(SoOutput * out,
                                    const SoFieldContainer * object) const
{
  SoFieldList forwardlist;
  
  if (!out->isBinary()) {
    out->indent();
    out->write("fields [ ");
  }

  SbBool extnode = !object->getIsBuiltIn();

  SbBool atleastonewritten = FALSE;
  for (int i = 0; i < this->getNumFields(); i++) {
    const SoField * f = this->getField(object, i);
    // Write field description if shouldWrite() returns TRUE, or if
    // 'object' is an extension node and some field is connected from
    // the field FIXME: SoFieldContainer::isBuiltIn() is protected so
    // we can't determine if 'object' is a built in node or not. We
    // just have to assume that it is.  pederb, 2002-02-07
    forwardlist.truncate(0);
    if (f->shouldWrite() || (extnode && f->getForwardConnections(forwardlist) > 0)) {
      if (!out->isBinary() && atleastonewritten) out->write(", ");
      out->write((const char *)(f->getTypeId().getName()));
      if (!out->isBinary()) out->write(' ');
      out->write((const char *)(this->getFieldName(i)));
      atleastonewritten = TRUE;
    }
  }

  if (!out->isBinary()) out->write(" ]\n");
}

// Check for equality.
int
SoFieldData::operator==(const SoFieldData * fd) const
{
  int i, n;
  n = this->enums.getLength();
  if (n != fd->enums.getLength()) return FALSE;
  for (i = 0; i < n; i++) {
    if (*(this->enums[i]) != *(fd->enums[i])) return FALSE;
  }

  n = this->fields.getLength();
  if (n != fd->fields.getLength()) return FALSE;
  for (i = 0; i < n; i++) {
    if (*(this->fields[i]) != *(fd->fields[i])) return FALSE;
  }

  return TRUE;
}
