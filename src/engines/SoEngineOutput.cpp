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
#include <Inventor/misc/SoBasic.h> // COIN_STUB()

/*!
  Returns the type of the connection.
 */
SoType
SoEngineOutput::getConnectionType(void) const
{
  return this->type;
}

/*!
  FIXME
 */
int
SoEngineOutput::getForwardConnections(SoFieldList & list) const
{
  int l=this->connections.getLength();
  for (int i=0;i<l;i++) {
    list.append(this->connections[i]);
  }
  return l;
}

/*!
  Set the enabled flag.
  TODO: doc on what the enabled flag does..

  \sa isEnabled().
 */
void
SoEngineOutput::enable(const SbBool flag)
{
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
  FIXME
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
  this->enabled=TRUE;
  this->container=NULL;
}

/*!
  FIXME
 */
SoEngineOutput::~SoEngineOutput()
{
}

/*!
  FIXME
 */
void
SoEngineOutput::setType(SoType type)
{
  this->type=type;
}

/*!
  FIXME
 */
void
SoEngineOutput::setContainer(SoEngine * engine)
{
  this->container=engine;
}

/*!
  FIXME
 */
void
SoEngineOutput::addConnection(SoField * f)
{
  int i=connections.find(f);
  if (i<0) connections.append(f);
}

/*!
  FIXME
 */
void
SoEngineOutput::removeConnection(SoField * f)
{
  int i=connections.find(f);
  if (i>=0) connections.remove(i);
}

/*!
  FIXME
 */
int
SoEngineOutput::getNumConnections() const
{
  return connections.getLength();
}

/*!
  FIXME
 */
SoField *
SoEngineOutput::operator[](int i) const
{
  return connections.get(i);
}

/*!
  FIXME: write function documentation
*/
void
SoEngineOutput::prepareToWrite(void) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoEngineOutput::doneWriting(void) const
{
  COIN_STUB();
}
