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

// SoGlobalField is an internal class where the instances keeps track
// of the global fields in the application.
//
// Having global fields placed within their own field containers makes
// it easier to handle field-to-field connections during file import
// and export.
//
// Note that this class is not supposed to be used by the application
// programmer -- so the API class definition header file is not
// installed.
//
// The only place within Coin where it is "consciously" used is from
// the SoDB class -- which also contains a complete "front-end" API to
// this class (see the SoDB::createGlobalField(),
// SoDB::getGlobalField() and SoDB::renameGlobalField() methods).


#include <Inventor/fields/SoGlobalField.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/lists/SoBaseList.h>
#include <Inventor/SbName.h>
#include <../tidbits.h> // coin_atexit()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoGlobalField::classTypeId;

SoBaseList * SoGlobalField::allcontainers = NULL;


// Constructor. Pass NULL for the field pointer to construct an empty
// SoGlobalField instance.
SoGlobalField::SoGlobalField(const SbName & name, SoField * field)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoGlobalField::SoGlobalField",
                         "name=='%s', field==%p(%s)",
                         name.getString(), field,
                         field ? field->getTypeId().getName().getString() : "");
#endif // debug

  if (field) {
    field->setContainer(this);

    // Initialize a fielddata container for the instance.
    this->classfielddata = new SoFieldData;
    this->classfielddata->addField(this, name, field);
  }
  else {
    this->classfielddata = NULL;
  }

  this->setName(name);
  SoGlobalField::allcontainers->append(this);
}

// Destructor.
SoGlobalField::~SoGlobalField()
{
  if (this->classfielddata) delete this->classfielddata->getField(this, 0);
  delete this->classfielddata;
}

// Instantiates and returns a new SoGlobalField instance.
void *
SoGlobalField::createInstance(void)
{
  return new SoGlobalField(SbName(""), NULL);
}

// Do common initializations.
void
SoGlobalField::initClass(void)
{
  // Make sure we init this class only once.
  assert(SoGlobalField::classTypeId == SoType::badType());
  // Make sure parent class has been initialized.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGlobalField::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "GlobalField",
                       SoGlobalField::createInstance);

  SoGlobalField::allcontainers = new SoBaseList;

#if COIN_DEBUG
  coin_atexit((coin_atexit_f *)SoGlobalField::clean);
#endif // COIN_DEBUG
}

// Free up resources.
void
SoGlobalField::clean(void)
{
#if COIN_DEBUG
  while (SoGlobalField::allcontainers->getLength())
    SoGlobalField::allcontainers->remove(0);

  delete SoGlobalField::allcontainers;
  SoGlobalField::allcontainers = NULL;
#endif // COIN_DEBUG
}

// Return index in list of global fields of the global field with the
// given name. Returns -1 if no global field exists with this name.
int
SoGlobalField::getGlobalFieldIndex(const SbName & name)
{
  int idx = SoGlobalField::allcontainers->getLength() - 1;
  while (idx >= 0 && (*SoGlobalField::allcontainers)[idx]->getName() != name)
    idx--;
  return idx;
}

// Remove the given global field.
void
SoGlobalField::removeGlobalFieldContainer(SoGlobalField * fieldcontainer)
{
  int idx = SoGlobalField::getGlobalFieldIndex(fieldcontainer->getName());
  SoGlobalField::allcontainers->remove(idx);
  return;
}

// Returns SoGlobalField instance with the given name.
SoGlobalField *
SoGlobalField::getGlobalFieldContainer(const SbName & name)
{
  int idx = SoGlobalField::getGlobalFieldIndex(name);
  return
    (idx == -1) ? NULL : (SoGlobalField *)(*SoGlobalField::allcontainers)[idx];
}

// Returns the complete set of SoGlobalField instances.
SoBaseList *
SoGlobalField::getGlobalFieldContainers(void)
{
  return SoGlobalField::allcontainers;
}

// Return id of SoGlobalField class instances.
SoType
SoGlobalField::getClassTypeId(void)
{
  return SoGlobalField::classTypeId;
}

// Virtual method which returns the class type id for this instance.
// Doc in superclass.
SoType
SoGlobalField::getTypeId(void) const
{
  return SoGlobalField::classTypeId;
}


// Returns a pointer to the global field stored in this container.
SoField *
SoGlobalField::getGlobalField(void) const
{
  return this->classfielddata ? this->classfielddata->getField(this, 0) : NULL;
}

// Overridden to also set field name.
void
SoGlobalField::setName(const SbName & newname)
{
  // Set name of this instance.
  inherited::setName(newname);

  if (this->classfielddata) {
    // SoFieldData doesn't have a rename method, so we do a little
    // hack to rename our field.
    SoFieldData fd;
    fd.addField(this, newname, this->getGlobalField());
    this->classfielddata->copy(&fd);
  }
}

// Read data for this SoGlobalField instance.
SbBool
SoGlobalField::readInstance(SoInput * in, unsigned short flags)
{
  // A bit more coding and we could let the readInstance() method be
  // called on already initialized SoGlobalField instances, but I
  // don't think there's any point. mortene.
  assert(this->classfielddata == NULL);

  // This macro is convenient for reading with error detection.
#define READ_VAL(val) \
  if (!in->read(val)) { \
    SoReadError::post(in, "Premature end of file"); \
    return FALSE; \
  }


  SbString str;
  READ_VAL(str);
  if (str != "type") {
    SoReadError::post(in, "invalid identifier, expected 'type', got '%s'",
                      str.getString());
    return FALSE;
  }

  SbName type;
  READ_VAL(type);

  SoType fieldtype = SoType::fromName(type);
  if (fieldtype == SoType::badType()) {
    SoReadError::post(in, "invalid field type '%s'", type.getString());
    return FALSE;
  }
  if (!fieldtype.canCreateInstance()) {
    SoReadError::post(in, "abstract type '%s'", type.getString());
    return FALSE;
  }
  if (!fieldtype.isDerivedFrom(SoField::getClassTypeId())) {
    SoReadError::post(in, "'%s' not a field type", type.getString());
    return FALSE;
  }

  if (in->isBinary()) {
    int dummy;
    READ_VAL(dummy); // Nr of fields. This should always be 1.
#if COIN_DEBUG
    if (dummy != 1) {
      SoDebugError::postWarning("SoGlobalField::readInstance",
                                "%d fields (should be 1)", dummy);
    }
#endif // COIN_DEBUG
  }

  SbName fieldname;
  READ_VAL(fieldname);
  inherited::setName(fieldname);

  SoField * f = (SoField *)fieldtype.createInstance();
  if (!f->read(in, fieldname)) {
    delete f;
    return FALSE;
  }

  f->setContainer(this);
  this->classfielddata = new SoFieldData;
  this->classfielddata->addField(this, fieldname, f);

#undef READ_VAL

  return TRUE;
}

// Overridden from SoBase to make sure we're accounted for, even
// though we -- as a container for a global field -- only exists
// through a field-to-field connection.
void
SoGlobalField::addWriteReference(SoOutput * out, SbBool isfromfield)
{
  assert(this->classfielddata);
  inherited::addWriteReference(out, FALSE);
}


// Overridden from parent class to have the field type written.
void
SoGlobalField::writeInstance(SoOutput * out)
{
  assert(this->classfielddata);

  if (this->writeHeader(out, FALSE, FALSE)) return;

  SoField * f = this->getGlobalField();
  assert(f);

  if (!out->isBinary()) out->indent();
  out->write("type");
  if (!out->isBinary()) out->write(' ');
  out->write(f->getTypeId().getName());
  if (!out->isBinary()) out->write('\n');

  SbBool isdefault = f->isDefault();
  f->setDefault(FALSE);
  inherited::writeInstance(out);
  f->setDefault(isdefault);

  this->writeFooter(out);
}

// Returns a pointer to the field data storage object for this
// instance. The SoFieldData object will always contain only a single
// field.
const SoFieldData *
SoGlobalField::getFieldData(void) const
{
  return this->classfielddata;
}
