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
  \class SoSFEngine SoSFEngine.h Inventor/fields/SoSFEngine.h
  \brief The SoSFEngine class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/fields/SoMFEngine.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/SbName.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// FIXME: can we use SO_SFIELD_SOURCE() instead? Depends on whether or
// not we should do reference counting, I think. 19991227 mortene.
SO_SFIELD_REQUIRED_SOURCE(SoSFEngine);


/*!
  FIXME: write function documentation
*/
void
SoSFEngine::setValue(SoEngine * value)
{
  // FIXME: should we really ref/unref here? 19990630 mortene.

  if (this->value) this->value->unref();
  this->value = value;
  if (this->value) this->value->ref();
  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::operator == (const SoSFEngine & field) const
{
  return (this->getValue() == field.getValue());
}

/*!
  Does initialization common for all objects of the
  SoSFEngine class. This includes setting up the
  type system, among other things.
*/
void
SoSFEngine::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEngine);
}

/*!
  Constructor.
*/
SoSFEngine::SoSFEngine(void)
{
  this->value = NULL;
}

/*!
  Destructor.
*/
SoSFEngine::~SoSFEngine(void)
{
  this->setValue(NULL);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::readValue(SoInput * /* in */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSFEngine::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoSFEngine::writeValue(SoOutput * /* out */) const
{
  COIN_STUB();
}

void
SoSFEngine::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFEngine::getClassTypeId()) {
    ((SoMFEngine *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFEngine::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}

// Overridden from parent to propagate write reference counting to
// value member.
void
SoSFEngine::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);
  COIN_STUB();
}
