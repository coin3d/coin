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
  \class SoEngine SoEngine.h Inventor/engines/SoEngine.h
  \brief SoEngine is the base class for Coin engines.
  \ingroup engines

  Engines enables you encapsulate both motion and geometry into a single
  scene graph... FIXME doc
*/

#include <Inventor/engines/SoEngines.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <stdlib.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SoType SoEngine::classTypeId = SoType::badType();

/*!
  Default constructor.
*/
SoEngine::SoEngine()
{
  this->stateflags.dirty = 0;
  this->stateflags.isnotifying = 0;
}

/*!
  Destructor.
*/
SoEngine::~SoEngine()
{
}

/*!
  Initializes the engine subsystem.
*/
void
SoEngine::initClass(void)
{
  SoEngine::classTypeId =
    SoType::createType(SoFieldContainer::getClassTypeId(), SbName("Engine"));

  SoEngine::initClasses();
}

/*!
  Initializes all engines.
*/
void
SoEngine::initClasses(void)
{
  SoBoolOperation::initClass();
  SoCalculator::initClass();
  SoComposeVec2f::initClass();
  SoComposeVec3f::initClass();
  SoComposeVec4f::initClass();
  SoDecomposeVec2f::initClass();
  SoDecomposeVec3f::initClass();
  SoDecomposeVec4f::initClass();
  SoComposeRotation::initClass();
  SoComposeRotationFromTo::initClass();
  SoDecomposeRotation::initClass();
  SoComposeMatrix::initClass();
  SoDecomposeMatrix::initClass();
  SoComputeBoundingBox::initClass();
  SoConcatenate::initClass();
  SoCounter::initClass();
  SoElapsedTime::initClass();
  SoFieldConverter::initClass();
  SoGate::initClass();
  SoInterpolate::initClass();
    SoInterpolateFloat::initClass();
    SoInterpolateRotation::initClass();
    SoInterpolateVec2f::initClass();
    SoInterpolateVec3f::initClass();
    SoInterpolateVec4f::initClass();
  SoOnOff::initClass();
  SoOneShot::initClass();
  SoSelectOne::initClass();
  SoTimeCounter::initClass();
  SoTransformVec3f::initClass();
  SoTriggerAny::initClass();
}

// overloaded from parent
SoType
SoEngine::getClassTypeId(void)
{
  return SoEngine::classTypeId;
}

/*!
  Adds all outputs to \a list. Returns the number of outputs added to
  the list.
*/
int
SoEngine::getOutputs(SoEngineOutputList & list) const
{
  const SoEngineOutputData *outputs = this->getOutputData();
  if (outputs == NULL) return 0;

  int n = outputs->getNumOutputs();
  for (int i = 0; i < n; i++) {
    list.append(outputs->getOutput(this, i));
  }
  return n;
}

/*!
  Returns the output with name \a outputname, or \e NULL if no such
  output exists.
*/
SoEngineOutput *
SoEngine::getOutput(const SbName & outputname) const
{
  const SoEngineOutputData *outputs = this->getOutputData();
  if (outputs == NULL) return NULL;
  int n = outputs->getNumOutputs();
  for (int i = 0; i < n; i++) {
    if (outputs->getOutputName(i) == outputname)
      return outputs->getOutput(this, i);
  }
  return NULL;
}

/*!
  Sets \outputname to the name of \a output. Returns \e FALSE if no
  such output is contained within the engine instance.
*/
SbBool
SoEngine::getOutputName(const SoEngineOutput * output,
                        SbName & outputname) const
{
  const SoEngineOutputData *outputs = this->getOutputData();
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
  Returns a copy of this engine. Inputs are copied without copying what's
  at the other end.
*/
SoEngine *
SoEngine::copy(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  Returns the engine named \a name, or \e NULL if no such engine
  exists.  If several engines have been registered under the same name,
  returns the \e last one which was registered.
*/
SoEngine *
SoEngine::getByName(const SbName & name)
{
  return (SoEngine*)SoBase::getNamedBase(name, SoEngine::getClassTypeId());
}

/*!
  Finds all engines named \a name. Returns the number of engines added
  to \a list.
*/
int
SoEngine::getByName(const SbName &name, SoEngineList &list)
{
  SoBaseList *baselist = (SoBaseList*)&list;
  return SoBase::getNamedBases(name, *baselist, SoEngine::getClassTypeId());
}

/*!
  Called when an input is changed. The default method does nothing, but
  subclasses may overload this method to do the The Right Thing when a
  specific field is changed.
*/
void
SoEngine::inputChanged(SoField * /* which */)
{
}

// doc in parent
void
SoEngine::notify(SoNotList *list)
{
  if (this->stateflags.dirty) {
    if (!this->isNotifying()) {
      // "notify" engine about the changed field
      this->stateflags.isnotifying = 1;
      this->inputChanged(list->getLastField());
      this->stateflags.isnotifying = 0;
    }
    // just return, output connections have been notified earlier
    return;
  }
  
  this->stateflags.dirty = 1;
  if (!this->isNotifying() && this->isNotifyEnabled()) {
    this->stateflags.isnotifying = 1;
    this->inputChanged(list->getLastField());

    SoEngineOutput *output;
    SoField *field;
    const SoEngineOutputData *outputs = this->getOutputData();
    int numOutputs = outputs->getNumOutputs();
    int numConnections;
    
    for (int i = 0; i < numOutputs; i++) {
      output = outputs->getOutput(this, i);
      if (output->isEnabled()) {
        numConnections = output->getNumConnections();
        for (int j = 0; j < numConnections; j++) {
          field = (*output)[j];
          if (!field->getDirty()) field->notify(list);
        }
      }
    }
    this->stateflags.isnotifying = 0;
  }
}

/*!
  Triggers an engine evaluation.
*/
void
SoEngine::evaluateWrapper(void)
{
  const SoEngineOutputData *outputs = this->getOutputData();
  SoEngineOutput *output; 
  int i, n = outputs->getNumOutputs();
  for (i = 0; i < n; i++) {
    outputs->getOutput(this, i)->prepareToWrite();
  }
  this->evaluate();
  for (i = 0; i < n; i++) {
    outputs->getOutput(this, i)->doneWriting();
  }
  this->stateflags.dirty = 0;
}

/*!
  Returns the SoFieldData class which holds information about inputs
  in this engine.
*/
const SoFieldData **
SoEngine::getInputDataPtr(void)
{
  return NULL; // base class has no output
}

/*!
  Returns the SoEngineOutputData class which holds information about
  the outputs in this engine.
*/
const SoEngineOutputData **
SoEngine::getOutputDataPtr(void)
{
  return NULL; // base class has no input
}

// overloaded from parent
void
SoEngine::writeInstance(SoOutput *out)
{
  COIN_STUB();
  inherited::writeInstance(out);
}

/*!
  FIXME: doc
*/
SoFieldContainer *
SoEngine::copyThroughConnection(void) const
{
  COIN_STUB();
  return NULL;
}

// overloaded from parent
SbBool
SoEngine::readInstance(SoInput *in, unsigned short flags)
{
  COIN_STUB();
  return inherited::readInstance(in, flags);
}

/*!
  Returns whether this engine should be copied or simply  referenced.
  int a copy operation.
*/
SbBool
SoEngine::shouldCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  Returns whether we're in a notification process. This is needed to
  avoid double notification when an engine enable outputs during
  inputChanged().
*/
SbBool
SoEngine::isNotifying(void) const
{
  return this->stateflags.isnotifying == 1;
}
