/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLInterpOutput SoVRMLInterp.h Inventor/VRMLnodes/SoVRMLInterp.h
  \brief The SoVRMLInterpOutput class is a representation of the output
  slots in an SoVRMLInterpolator.

  SoVRMLInterpOutput has methods for convenient handling of the
  connections made from SoVRMLInterpolator objects to SoField objects.

  \sa SoVRMLInterpolator, SoField
 */

#include <Inventor/VRMLnodes/SoVRMLInterpOutput.h>
#include <Inventor/fields/SoField.h>
#include <coindefs.h> // COIN_STUB()
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Constructor.
 */
SoVRMLInterpOutput::SoVRMLInterpOutput(void)
{
  this->enabled = TRUE;
  this->owner = NULL;
}

/*!
  Destructor.
 */
SoVRMLInterpOutput::~SoVRMLInterpOutput()
{
  // FIXME: disconnect fields? 19990622 mortene.
}

/*!
  Returns the type of the connection.
 */
SoType
SoVRMLInterpOutput::getConnectionType(void) const
{
  // FIXME: is this correct? 19990623 mortene.
  return SoField::getClassTypeId();
}

/*!
  Returns number of fields which are auditing this interpolator output,
  and adds all of them to the \a list.

  \sa getNumConnections()
 */
int
SoVRMLInterpOutput::getForwardConnections(SoFieldList & list) const
{
  int l = this->connectedfields.getLength();
  for (int i=0;i<l;i++) list.append(this->connectedfields[i]);
  return l;
}

/*!
  Set the enabled flag.
  FIXME: doc on what the enabled flag does..

  \sa isEnabled()
 */
void
SoVRMLInterpOutput::enable(const SbBool flag)
{
  this->enabled = flag;
}

/*!
  Returns status of the enabled flag.

  \sa enable().
 */
SbBool
SoVRMLInterpOutput::isEnabled(void) const
{
  return this->enabled;
}

/*!
  Returns the instance owning this object.

  \sa setContainer()
 */
SoVRMLInterpolator *
SoVRMLInterpOutput::getContainer(void) const
{
  return this->owner;
}

/*!
  Set owner.

  \sa getContainer()
 */
void
SoVRMLInterpOutput::setContainer(SoVRMLInterpolator * interp)
{
  this->owner = interp;
}

/*!
  Add a new field auditing this interpolator output to our list.

  \sa removeConnection(), getForwardConnections()
 */
void
SoVRMLInterpOutput::addConnection(SoField * f)
{
#if COIN_DEBUG
  int i=this->connectedfields.find(f);
  if (i < 0) {
    SoDebugError::postWarning("SoVRMLInterpOutput::addConnection",
                              "field already in auditor list");
    return;
  }
#endif // COIN_DEBUG
  this->connectedfields.append(f);
}

/*!
  Remove \a f from the list of field auditors.

  \sa addConnection(), getForwardConnections()
 */
void
SoVRMLInterpOutput::removeConnection(SoField * f)
{
  int i=this->connectedfields.find(f);
#if COIN_DEBUG
  if (i < 0) {
    SoDebugError::postWarning("SoVRMLInterpOutput::removeConnection",
                              "field not in auditor list");
    return;
  }
#endif // COIN_DEBUG
  this->connectedfields.remove(i);
}

/*!
  Returns number of fields auditing this interpolator output.

  \sa getForwardConnections()
 */
int
SoVRMLInterpOutput::getNumConnections(void) const
{
  return this->connectedfields.getLength();
}

/*!
  Returns the auditing field at index \a i.
 */
SoField *
SoVRMLInterpOutput::operator[](int i) const
{
#if COIN_DEBUG
  if (i < 0 || i >= this->connectedfields.getLength()) {
    SoDebugError::postWarning("SoVRMLInterpOutput::operator[]",
                              "index out of bounds");
    return NULL;
  }
#endif // COIN_DEBUG
  return this->connectedfields[i];
}

/*!
  FIXME: write function documentation
*/
void
SoVRMLInterpOutput::prepareToWrite(void) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoVRMLInterpOutput::doneWriting(void) const
{
  COIN_STUB();
}
