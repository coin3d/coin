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

// SoGlobalField is an internal class where the instances keeps track
// of the global fields in the application.
//
// Having global fields placed within their own field containers makes
// it easier to handle field-to-field connections during file import
// and export.
//
// Note that this class is not supposed to be used by the application
// programmer, and the only place within Coin where it is
// "consciously" used is from the SoDB class -- which also contains a
// complete "front-end" API to this class (see the
// SoDB::createGlobalField(), SoDB::getGlobalField() and
// SoDB::renameGlobalField() methods).


#include <Inventor/fields/SoGlobalField.h>
#include <Inventor/SbName.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()

SoType SoGlobalField::classTypeId = SoType::badType();


// Constructor.
SoGlobalField::SoGlobalField(const SbName & name, SoField * field)
{
  field->setContainer(this);

  // Initialize a fielddata container for the instance.
  this->classfielddata = new SoFieldData;
  this->classfielddata->addField(this, name, field);

  this->setName(name);
}

// Destructor.
SoGlobalField::~SoGlobalField()
{
  delete this->classfielddata->getField(this, 0);
  delete this->classfielddata;
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
    SoType::createType(inherited::getClassTypeId(), "GlobalField", NULL);
}

// Return id of SoGlobalField class instances.
SoType
SoGlobalField::getClassTypeId(void)
{
  return SoGlobalField::classTypeId;
}

// Virtual method which returns the class type id for this instance.
SoType
SoGlobalField::getTypeId(void) const
{
  return SoGlobalField::classTypeId;
}

// Returns a pointer to the global field stored in this container.
SoField *
SoGlobalField::getGlobalField(void) const
{
  return this->classfielddata->getField(this, 0);
}

// Overloaded to also set field name.
void
SoGlobalField::setName(const SbName & newname)
{
  // Set name of this instance.
  inherited::setName(newname);

  // SoFieldData doesn't have a rename method, so we do a little hack
  // to rename our field.
  SoFieldData fd;
  fd.addField(this, newname, this->getGlobalField());
  this->classfielddata->copy(&fd);
}

// Overloaded from SoBase to make sure "our" field is also accounted
// for.
void
SoGlobalField::addWriteReference(SoOutput * out, SbBool isfromfield)
{
  COIN_STUB();
}


// FIXME: say something about the write format.
void
SoGlobalField::writeInstance(SoOutput * out)
{
  COIN_STUB();
}

// Returns a pointer to the field data storage object for this
// instance. The SoFieldData object will always contain only a single
// field.
const SoFieldData *
SoGlobalField::getFieldData(void) const
{
  return this->classfielddata;
}
