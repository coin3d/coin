/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoNodeEngine SoNodeEngine.h Inventor/engines/SoNodeEngine.h
  \brief SoNodeEngine is the base class for Coin node engines.
  \ingroup engines

  Node engines have the same functionality as normal Engines, except
  that they inherit from SoNode, which makes it possible to insert
  node engines in the scene graph.

  This class is not part of the Open Inventor 2.1 API, and is an
  extension specific to Coin.
*/

// FIXME: currently most of the code in this class is simply copied
// from SoEngine.cpp. We should try to use a templant file or
// something to make it easier to maintain the code.  
// pederb, 2001-10-23

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SbName.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// FIXME: document these properly. 20000405 mortene.
/*!
  \fn const SoEngineOutputData * SoNodeEngine::getOutputData(void) const
  \COININTERNAL
*/
/*!
  \fn void SoNodeEngine::evaluate(void)
  \COININTERNAL
*/


// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoNodeEngine::classTypeId;

#define FLAG_ISNOTIFYING 0x1

/*!
  Default constructor.
*/
SoNodeEngine::SoNodeEngine(void)
{
  this->flags = 0;
}

/*!
  Destructor.
*/
SoNodeEngine::~SoNodeEngine()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNodeEngine::~SoNodeEngine", "%p", this);
#endif // debug
}

// Overrides SoBase::destroy().
void
SoNodeEngine::destroy(void)
{
#if COIN_DEBUG && 0 // debug
  SbName n = this->getName();
  SoType t = this->getTypeId();
  SoDebugError::postInfo("SoNodeEngine::destroy", "start -- '%s' (%s)",
                         n.getString(),
                         t.getName().getString());
#endif // debug

  // evaluate() before we actually destruct. It would be too late
  // during the destructor, as the SoNodeEngine::evaluate() method is pure
  // virtual.
  //
  // The explicit call here is done so attached fields will get the
  // chance to update before we die. SoField::disconnect() will
  // normally call SoNodeEngine::evaluate(), but we disable that feature
  // by setting SoEngineOutput::isEnabled() to FALSE before
  // decoupling.
  this->evaluateWrapper();

  // SoBase destroy().
  inherited::destroy();

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNodeEngine::destroy", "done -- '%s' (%s)",
                         n.getString(),
                         t.getName().getString());
#endif // debug
}

// Doc from superclass.
void
SoNodeEngine::initClass(void)
{
  SoNodeEngine::classTypeId =
    SoType::createType(SoNode::getClassTypeId(), SbName("NodeEngine"));
}

// Documented in superclass.
SoType
SoNodeEngine::getClassTypeId(void)
{
  return SoNodeEngine::classTypeId;
}

/*!
  Adds all outputs to list. Returns the number of outputs added to the
  list.
*/
int
SoNodeEngine::getOutputs(SoEngineOutputList & l) const
{
  const SoEngineOutputData * outputs = this->getOutputData();
  if (outputs == NULL) return 0;

  int n = outputs->getNumOutputs();
  for (int i = 0; i < n; i++) {
    l.append(outputs->getOutput(this, i));
  }
  return n;
}

/*!
  Returns the output with name \a outputname, or \c NULL if no such
  output exists.
*/
SoEngineOutput *
SoNodeEngine::getOutput(const SbName & outputname) const
{
  const SoEngineOutputData * outputs = this->getOutputData();
  if (outputs == NULL) return NULL;
  int n = outputs->getNumOutputs();
  for (int i = 0; i < n; i++) {
    if (outputs->getOutputName(i) == outputname)
      return outputs->getOutput(this, i);
  }
  return NULL;
}

/*!
  Sets \outputname to the name of \a output. Returns \c FALSE if no
  such output is contained within the engine instance.
*/
SbBool
SoNodeEngine::getOutputName(const SoEngineOutput * output,
                        SbName & outputname) const
{
  const SoEngineOutputData * outputs = this->getOutputData();
  if (outputs == NULL) return FALSE;

  int n = outputs->getNumOutputs();
  for (int i = 0; i < n; i++) {
    if (outputs->getOutput(this, i) == output) {
      outputname = outputs->getOutputName(i);
      return TRUE;
    }
  }
  return FALSE;
}

/*!
  Called when an input is changed. The default method does nothing,
  but subclasses may override this method to do the The Right Thing
  when a specific field is changed.
*/
void
SoNodeEngine::inputChanged(SoField * which)
{
}

// doc in parent
void
SoNodeEngine::notify(SoNotList * nl)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNodeEngine::notify", "%p - %s, start",
                         this, this->getTypeId().getName().getString());
#endif // debug

  // Avoid recursive notification calls.
  if (this->isNotifying()) return;
  this->flags |= FLAG_ISNOTIFYING;

  // FIXME: we don't add ourselves to the notification list. This
  // should probably be done, but I've postponed it until we see the
  // need to fix all interaction with the notification list. 20000911 mortene.


  // Let engine know that a field changed, so we can recalculate
  // internal variables, if necessary.
  this->inputChanged(nl->getLastField());

  // Notify the slave fields connected to our engine outputs.
  const SoEngineOutputData * outputs = this->getOutputData();
  int numoutputs = outputs->getNumOutputs();
  for (int i = 0; i < numoutputs; i++)
    outputs->getOutput(this, i)->touchSlaves(nl, this->isNotifyEnabled());

  this->flags &= ~FLAG_ISNOTIFYING;

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNodeEngine::notify", "%p - %s, done",
                         this, this->getTypeId().getName().getString());
#endif // debug
}

/*!
  Triggers an engine evaluation.
*/
void
SoNodeEngine::evaluateWrapper(void)
{
  const SoEngineOutputData * outputs = this->getOutputData();
  int i, n = outputs->getNumOutputs();
  for (i = 0; i < n; i++) {
    outputs->getOutput(this, i)->prepareToWrite();
  }
  this->evaluate();
  for (i = 0; i < n; i++) {
    outputs->getOutput(this, i)->doneWriting();
  }
}

/*!
  Returns the SoFieldData class which holds information about inputs
  in this engine.
*/
const SoFieldData **
SoNodeEngine::getFieldDataPtr(void)
{
  return NULL; // base class has no input
}

/*!
  Returns the SoEngineOutputData class which holds information about
  the outputs in this engine.
*/
const SoEngineOutputData **
SoNodeEngine::getOutputDataPtr(void)
{
  return NULL; // base class has no output
}

// Documented in superclass.
SbBool
SoNodeEngine::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: I believe there's code missing here for reading
  // SoUnknownEngine instances. 20000919 mortene.
  return inherited::readInstance(in, flags);
}

// Documented in superclass.
void
SoNodeEngine::writeInstance(SoOutput * out)
{
  inherited::writeInstance(out);
}

/*!
  Writes the types of engine outputs for extension engines
  (i.e. engines not built in to Coin).
*/
void
SoNodeEngine::writeOutputTypes(SoOutput * out)
{
  COIN_STUB();
}

/*!
  Returns whether we're in a notification process. This is needed to
  avoid double notification when an engine enables outputs during
  inputChanged().
*/
SbBool
SoNodeEngine::isNotifying(void) const
{
  return (this->flags & FLAG_ISNOTIFYING) != 0;
}
