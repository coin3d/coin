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

  Engines enables the application programmer to make complex
  connections between fields.

  The most common cases where you use engines are: 1) to constrain the
  values of a field with regard to the contents of one or more other
  fields in the scene graph, 2) as a convenient way to animate parts
  of the scene graph (most often geometry, but also other aspects of
  the scene).

  The reference count of an engine will be increased by 1 for each
  connection made to one of its engine outputs, and decreased by one
  for a disconnect. See SoEngineOutput::addConnection() and
  SoEngineOutput::removeConnection(). When the reference count goes
  down to zero, the engine will automatically be destroyed, and
  subsequent attempts at using the engine will lead to a crash.

  If you want complete control over when an engine gets destructed,
  use SoBase::ref() and SoBase::unref() for explicit
  referencing/dereferencing.
*/

#include <Inventor/engines/SoEngines.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// FIXME: document these properly. 20000405 mortene.
/*!
  \fn const SoEngineOutputData * SoEngine::getOutputData(void) const
  \internal
*/
/*!
  \fn void SoEngine::evaluate(void)
  \internal
*/



SoType SoEngine::classTypeId = SoType::badType();

/*!
  Default constructor.
*/
SoEngine::SoEngine(void)
{
  this->stateflags.dirty = 0;
  this->stateflags.isnotifying = 0;
}

/*!
  Destructor.
*/
SoEngine::~SoEngine()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngine::~SoEngine", "%p", this);
#endif // debug
}

// Overrides SoBase::destroy().
void
SoEngine::destroy(void)
{
#if COIN_DEBUG && 0 // debug
  SbName n = this->getName();
  SoType t = this->getTypeId();
  SoDebugError::postInfo("SoEngine::destroy", "start -- '%s' (%s)",
                         n.getString(),
                         t.getName().getString());
#endif // debug

  // evaluate() before we actually destruct. It would be too late
  // during the destructor, as the SoEngine::evaluate() method is pure
  // virtual.
  //
  // The explicit call here is done so attached fields will get the
  // chance to update before we die. SoField::disconnect() will
  // normally call SoEngine::evaluate(), but we disable that feature
  // by setting SoEngineOutput::isEnabled() to FALSE before
  // decoupling.
  this->evaluateWrapper();

  // SoBase destroy().
  inherited::destroy();

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngine::destroy", "done -- '%s' (%s)",
                         n.getString(),
                         t.getName().getString());
#endif // debug
}

// Doc from superclass.
void
SoEngine::initClass(void)
{
  SoEngine::classTypeId =
    SoType::createType(SoFieldContainer::getClassTypeId(), SbName("Engine"));

  SoEngine::initClasses();
}

/*!
  Initializes all engines. Automatically called from
  SoEngine::initClass() upon initialization of Coin.
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
  Adds all outputs to list. Returns the number of outputs added to the
  list.
*/
int
SoEngine::getOutputs(SoEngineOutputList & l) const
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
SoEngine::getOutput(const SbName & outputname) const
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
SoEngine::getOutputName(const SoEngineOutput * output,
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
  Returns a copy of the engine. Input connections are shallow copied.
*/
SoEngine *
SoEngine::copy(void) const
{
  COIN_STUB();
  return NULL;
}

/*!
  Returns the engine named \a name, or \c NULL if no such engine
  exists.  If several engines have been registered under the same
  name, returns the \e last one which was registered.
*/
SoEngine *
SoEngine::getByName(const SbName & name)
{
  return (SoEngine *)SoBase::getNamedBase(name, SoEngine::getClassTypeId());
}

/*!
  Finds all engines named \a name. Returns the number of engines added
  to the \a el list.
*/
int
SoEngine::getByName(const SbName & name, SoEngineList & el)
{
  SoBaseList * baselist = (SoBaseList *)&el;
  return SoBase::getNamedBases(name, *baselist, SoEngine::getClassTypeId());
}

/*!
  Called when an input is changed. The default method does nothing,
  but subclasses may overload this method to do the The Right Thing
  when a specific field is changed.
*/
void
SoEngine::inputChanged(SoField * which)
{
}

// doc in parent
void
SoEngine::notify(SoNotList * nl)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngine::notify",
                         "%p (%s, '%s', dirty: %d) -- start",
                         this,
                         this->getTypeId().getName().getString(),
                         this->getName().getString(),
                         this->stateflags.dirty);
  this->stateflags.dirty = 0;
#endif // debug

  if (this->stateflags.dirty) {
    if (!this->isNotifying()) {
      // "notify" engine about the changed field
      this->stateflags.isnotifying = 1;
      this->inputChanged(nl->getLastField());
      this->stateflags.isnotifying = 0;
    }
    // nothing more to do, output connections have been notified earlier
  }
  else {
    this->stateflags.dirty = 1;
    if (!this->isNotifying() && this->isNotifyEnabled()) {
      this->stateflags.isnotifying = 1;
      this->inputChanged(nl->getLastField());

      SoEngineOutput * output;
      SoField * field;
      const SoEngineOutputData * outputs = this->getOutputData();
      int numoutputs = outputs->getNumOutputs();
      int numconnections;

#if COIN_DEBUG && 0 // debug
      if (this->getTypeId().isDerivedFrom(SoFieldConverter::getClassTypeId()))
        SoDebugError::postInfo("SoEngine::notify", "numoutputs: %d", numoutputs);
#endif // debug

      for (int i = 0; i < numoutputs; i++) {
        output = outputs->getOutput(this, i);
        if (output->isEnabled()) {
          numconnections = output->getNumConnections();
#if COIN_DEBUG && 0 // debug
          if (this->getTypeId().isDerivedFrom(SoFieldConverter::getClassTypeId()))
            SoDebugError::postInfo("SoEngine::notify",
                                   "numconnections: %d",
                                   numconnections);
#endif // debug
          for (int j = 0; j < numconnections; j++) {
            field = (*output)[j];
#if COIN_DEBUG && 0 // debug
            SoDebugError::postInfo("SoEngine::notify",
                                   "field %p (%s, '%s'), dirty: %d",
                                   field,
                                   field->getContainer() ? field->getContainer()->getTypeId().getName().getString() : "*none*",
                                   field->getContainer() ? field->getContainer()->getName().getString() : "*none*",
                                   field->getDirty());
            if (field->getDirty()) field->notify(nl);
#endif // debug
            if (!field->getDirty()) field->notify(nl);
          }
        }
      }
      this->stateflags.isnotifying = 0;
    }
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoEngine::notify",
                         "%p (%s, '%s') -- done",
                         this,
                         this->getTypeId().getName().getString(),
                         this->getName().getString());
#endif // debug
}

/*!
  Triggers an engine evaluation.
*/
void
SoEngine::evaluateWrapper(void)
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
SoEngine::writeInstance(SoOutput * out)
{
  if (this->writeHeader(out, FALSE, TRUE)) return;
  inherited::writeInstance(out);
  this->writeFooter(out);
}

/*!
  Writes the types of engine outputs for extension engines
  (i.e. engines not built in to Coin).
*/
void
SoEngine::writeOutputTypes(SoOutput * out)
{
  COIN_STUB();
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
SoEngine::readInstance(SoInput * in, unsigned short flags)
{
  COIN_STUB();
  return inherited::readInstance(in, flags);
}

/*!
  Returns whether this engine should be copied or simply referenced in
  a copy operation.
*/
SbBool
SoEngine::shouldCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  Returns whether we're in a notification process. This is needed to
  avoid double notification when an engine enables outputs during
  inputChanged().
*/
SbBool
SoEngine::isNotifying(void) const
{
  return this->stateflags.isnotifying == 1;
}
