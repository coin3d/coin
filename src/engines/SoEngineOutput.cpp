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
  \class SoEngineOutput SoEngine.h Inventor/engines/SoEngine.h
  \brief The SoEngineOutput class is a representation of the output slots in an SoEngine.

  SoEngineOutput has methods for convenient handling of the connections made
  from SoEngine objects to SoField objects.

  \sa SoEngine, SoField
 */

#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/fields/SoField.h>

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
  Adds all fields connected to this output to \a list.
  Returns the number of fields added to the list.
  
  \sa addConnection()
  \sa removeConnection()
*/
int
SoEngineOutput::getForwardConnections(SoFieldList & list) const
{
  int n = this->connections.getLength();
  for (int i = 0; i < n; i++) {
    list.append(this->connections[i]);
  }
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
  Constructor.
*/
SoEngineOutput::SoEngineOutput()
{
  this->enabled = TRUE;
  this->container = NULL;
}

/*!
  Destructor.
*/
SoEngineOutput::~SoEngineOutput()
{
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
  \sa removeConnection()
  \sa getForwardConnections()
*/
void
SoEngineOutput::addConnection(SoField * f)
{
  int i = connections.find(f);
  if (i < 0) connections.append(f);
}

/*!
  Removes \a f from the list of connections from this output.
  \sa addConnection()
  \sa getForwardConnections()
*/
void
SoEngineOutput::removeConnection(SoField * f)
{
  int i = connections.find(f);
  if (i >= 0) connections.remove(i);
}

/*!
  Returns the number of fields in the list of connections.
  \sa operator[]
  \sa addConnection()
*/
int
SoEngineOutput::getNumConnections() const
{
  return connections.getLength();
}

/*!
  Returns the fields at index \a i in the list of connections.
  \sa getNumConnections()
*/
SoField *
SoEngineOutput::operator[](int i) const
{
  return this->connections[i];
}

/*!
  Disables notifications on fields connected to this output.
  This is done before the engine is evaulated, since the fields we
  are going to write into have already been notified.
  \sa doneWriting()
*/
void
SoEngineOutput::prepareToWrite(void) const
{
  int n = this->connections.getLength();
  for (int i = 0; i < n; i++) {
    this->connections[i]->enableNotify(FALSE);
  }
}

/*!
  Enables notification on fields connected to this output.
  Use this method to restore the notification flags after evaluating.
  \sa prepareToWrite()
*/
void
SoEngineOutput::doneWriting(void) const
{
  int n = this->connections.getLength();
  for (int i = 0; i < n; i++) {
    this->connections[i]->enableNotify(TRUE);
  }
}
