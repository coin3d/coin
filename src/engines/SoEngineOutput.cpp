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
  \class SoEngineOutput SoEngine.h Inventor/engines/SoEngine.h
  \brief The SoEngineOutput class is the output slots in SoEngine instances.
  \ingroup engines

  SoEngineOutput has methods for convenient handling of the
  connections made from SoEngine objects to SoField objects.

  \sa SoEngine, SoField
 */

#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/fields/SoField.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Constructor. The SoEngineOutput will initially not be contained
  within an SoEngine nor will it have any slave fields attached.

  \sa setContainer()
*/
SoEngineOutput::SoEngineOutput(void)
{
  this->enabled = TRUE;
  this->container = NULL;
}

/*!
  Destructor.
*/
SoEngineOutput::~SoEngineOutput()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngineOutput::~SoEngineOutput", "start %p", this);
#endif // debug

  // Avoids evaluation from the fields in SoField::disconnect() (which
  // would again lead to problems with the pure virtual
  // SoEngine::evaluate() function during destruction of our
  // container).
  this->enabled = FALSE;

  // Disconnect all fields.
  SoFieldList fl;
  int nr = this->getForwardConnections(fl);
  for (int i=0; i < nr; i++) fl[i]->disconnect(this);

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngineOutput::~SoEngineOutput", "done %p", this);
#endif // debug
}

/*!
  Returns the type of the engine output.
*/
SoType
SoEngineOutput::getConnectionType(void) const
{
  assert(this->container != NULL);
  const SoEngineOutputData *outputs = this->container->getOutputData();
  assert(outputs);
  int idx = outputs->getIndex(this->container, this);
  assert(idx >= 0);
  return outputs->getType(idx);
}

/*!
  Adds all slave fields connected to this output to \a fl.  Returns
  the number of slaves.

  \sa addConnection()
  \sa removeConnection()
*/
int
SoEngineOutput::getForwardConnections(SoFieldList & fl) const
{
  int n = this->slaves.getLength();
  for (int i = 0; i < n; i++) fl.append(this->slaves[i]);
  return n;
}

/*!
  Sets the enabled flag. If output is disabled, the fields connected
  to this output will not be changed when the engine is evaluated.

  \sa isEnabled().
*/
void
SoEngineOutput::enable(const SbBool flag)
{
  if (this->container) {
    // need to notify fields connected to this output
    this->container->stateflags.dirty = 0;
  }
  this->enabled = flag;
}

/*!
  Returns status of the enabled flag.

  \sa enable().
*/
SbBool
SoEngineOutput::isEnabled(void) const
{
  return this->enabled;
}

/*!
  Returns the engine containing this output.

  \sa setContainer()
*/
SoEngine*
SoEngineOutput::getContainer(void) const
{
  return this->container;
}

/*!
  Sets the engine containing this output.

  \sa getContainer()
*/
void
SoEngineOutput::setContainer(SoEngine * engine)
{
  this->container = engine;
}

/*!
  Adds \a f to the list of connections from this output.

  Adds 1 to the reference count of the container engine.

  \sa removeConnection(), getForwardConnections()
*/
void
SoEngineOutput::addConnection(SoField * f)
{
  int i = this->slaves.find(f);
#if COIN_DEBUG
  if (i != -1) {
    SoDebugError::postWarning("SoEngineOutput::addConnection",
                              "connection from %p already made", f);
    return;
  }
#endif // COIN_DEBUG
  this->slaves.append(f);

  this->getContainer()->ref();
}

/*!
  Removes \a f from the list of connections from this output.

  Subtracts 1 from the reference count of the container engine. If the
  reference count reaches zero (which will happen if this was the last
  connection and the application programmer did not explicitly \e ref
  the engine), the container engine will be deallocated.

  \sa addConnection(), getForwardConnections()
*/
void
SoEngineOutput::removeConnection(SoField * f)
{
  int i = this->slaves.find(f);
#if COIN_DEBUG
  if (i == -1) {
    SoDebugError::postWarning("SoEngineOutput::removeConnection",
                              "no connection from %p present", f);
    return;
  }
#endif // COIN_DEBUG
  this->slaves.remove(i);

  this->getContainer()->unref();
}

/*!
  Returns the number of fields in the list of connections.

  \sa operator[], addConnection()
*/
int
SoEngineOutput::getNumConnections(void) const
{
  return this->slaves.getLength();
}

/*!
  Returns the field at index \a i in the list of connections.

  \sa getNumConnections()
*/
SoField *
SoEngineOutput::operator[](int i) const
{
  return this->slaves[i];
}

/*!
  Disables notifications on fields connected to this output.  This is
  done before the engine is evaulated, since the fields we are going
  to write into have already been notified.

  \sa doneWriting()
*/
void
SoEngineOutput::prepareToWrite(void) const
{
  int n = this->slaves.getLength();
  for (int i = 0; i < n; i++) {
    this->slaves[i]->enableNotify(FALSE);
  }
}

/*!
  Enables notification on fields connected to this output.  Use this
  method to restore the notification flags after evaluating.

  \sa prepareToWrite()
*/
void
SoEngineOutput::doneWriting(void) const
{
  int n = this->slaves.getLength();
  for (int i = 0; i < n; i++) {
    this->slaves[i]->enableNotify(TRUE);
  }
}
