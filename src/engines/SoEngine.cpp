/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/engines/SoEngine.h>

#include <Inventor/SbName.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoEngines.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/lists/SoFieldList.h>

SoType SoEngine::classTypeId;

SoType
SoEngine::getClassTypeId(void)
{
  return SoEngine::classTypeId;
}

/*!
  FIXME: document. 19990925 mortene.
 */
int
SoEngine::getOutputs(SoEngineOutputList & list) const
{
  list.truncate(0);
  list.copy(*this->outputList);
  return list.getLength();
}

SoEngineOutput *
SoEngine::getOutput(const SbName & /* outputName */) const
{
  COIN_STUB();
  return NULL;
}

SbBool
SoEngine::getOutputName(const SoEngineOutput * /* output */,
                        SbName & /* outputName */) const
{
  COIN_STUB();
  return FALSE;
}

SoEngine *
SoEngine::copy(void) const
{
  COIN_STUB();
  return NULL;
}

SoEngine *
SoEngine::getByName(const SbName & /* name */)
{
  COIN_STUB();
  return NULL;
}

int
SoEngine::getByName(const SbName & /* name */, SoEngineList & /* list */)
{
  COIN_STUB();
  return -1;
}

void
SoEngine::initClass()
{
  SoEngine::classTypeId =
    SoType::createType(SoFieldContainer::getClassTypeId(), SbName("Engine"));

  SoEngine::initEngines();
}

void
SoEngine::initEngines()
{
#if !defined(COIN_EXCLUDE_SOBOOLOPERATION)
  SoBoolOperation::initClass();
#endif // !COIN_EXCLUDE_SOBOOLOPERATION
#if !defined(COIN_EXCLUDE_SOCALCULATOR)
  // FIXME: not implemented yet. 19990406 mortene.
//    SoCalculator::initClass();
#endif // !COIN_EXCLUDE_SOCALCULATOR
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC2F)
  SoComposeVec2f::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC3F)
  SoComposeVec3f::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC4F)
  SoComposeVec4f::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC2F)
  SoDecomposeVec2f::initClass();
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC3F)
  SoDecomposeVec3f::initClass();
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC4F)
  SoDecomposeVec4f::initClass();
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SOCOMPOSEROTATION)
  SoComposeRotation::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SOCOMPOSEROTATIONFROMTO)
  // FIXME: not implemented yet. 19990406 mortene.
//    SoComposeRotationFromTo::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEROTATIONFROMTO
#if !defined(COIN_EXCLUDE_SODECOMPOSEROTATION)
  SoDecomposeRotation::initClass();
#endif // !COIN_EXCLUDE_SODECOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SOCOMPOSEMATRIX)
  SoComposeMatrix::initClass();
#endif // !COIN_EXCLUDE_SOCOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SODECOMPOSEMATRIX)
  SoDecomposeMatrix::initClass();
#endif // !COIN_EXCLUDE_SODECOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SOCOMPUTEBOUNDINGBOX)
  // FIXME: not implemented yet. 19990406 mortene.
//    SoComputeBoundingBox::initClass();
#endif // !COIN_EXCLUDE_SOCOMPUTEBOUNDINGBOX
#if !defined(COIN_EXCLUDE_SOCONCATENATE)
  SoConcatenate::initClass();
#endif // !COIN_EXCLUDE_SOCONCATENATE
#if !defined(COIN_EXCLUDE_SOCOUNTER)
  SoCounter::initClass();
#endif // !COIN_EXCLUDE_SOCOUNTER
#if !defined(COIN_EXCLUDE_SOELAPSEDTIME)
  SoElapsedTime::initClass();
#endif // !COIN_EXCLUDE_SOELAPSEDTIME
#if !defined(COIN_EXCLUDE_SOFIELDCONVERTER)
  SoFieldConverter::initClass();
#endif // !COIN_EXCLUDE_SOFIELDCONVERTER
#if !defined(COIN_EXCLUDE_SOGATE)
  SoGate::initClass();
#endif // !COIN_EXCLUDE_SOGATE
#if !defined(COIN_EXCLUDE_SOINTERPOLATE)
  SoInterpolate::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATE
#if !defined(COIN_EXCLUDE_SOINTERPOLATEFLOAT)
    SoInterpolateFloat::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATEFLOAT
#if !defined(COIN_EXCLUDE_SOINTERPOLATEROTATION)
    SoInterpolateRotation::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATEROTATION
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC2F)
    SoInterpolateVec2f::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC2F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC3F)
    SoInterpolateVec3f::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC3F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC4F)
    SoInterpolateVec4f::initClass();
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC4F
#if !defined(COIN_EXCLUDE_SOONOFF)
  SoOnOff::initClass();
#endif // !COIN_EXCLUDE_SOONOFF
#if !defined(COIN_EXCLUDE_SOONESHOT)
  SoOneShot::initClass();
#endif // !COIN_EXCLUDE_SOONESHOT
#if !defined(COIN_EXCLUDE_SOSELECTONE)
  SoSelectOne::initClass();
#endif // !COIN_EXCLUDE_SOSELECTONE
#if !defined(COIN_EXCLUDE_SOTIMECOUNTER)
  // FIXME: not implemented yet. 19990406 mortene.
//    SoTimeCounter::initClass();
#endif // !COIN_EXCLUDE_SOTIMECOUNTER
#if !defined(COIN_EXCLUDE_SOTRANSFORMVEC3F)
  // FIXME: not implemented yet. 19990406 mortene.
//    SoTransformVec3f::initClass();
#endif // !COIN_EXCLUDE_SOTRANSFORMVEC3F
#if !defined(COIN_EXCLUDE_SOTRIGGERANY)
  SoTriggerAny::initClass();
#endif // !COIN_EXCLUDE_SOTRIGGERANY
}

void
SoEngine::inputChanged(SoField * /* which */)
{
}

void
SoEngine::notify(SoNotList *list)
{
  //  SoDebugError::postInfo("SoEngine::notify","");
  if (this->isNotifyEnabled()) {
    this->inputChanged(list->getLastField());
    SoFieldList flist;
    SoEngineOutputList olist;
    int numOutputs=this->getOutputs(olist);
    if (numOutputs>0) {
      for (int j=0;j<numOutputs;j++)
        if (olist[j]->isEnabled()) olist[j]->getForwardConnections(flist);

      for (int i=0;i<flist.getLength();i++)
        flist[i]->notify(list);
    }
  }
}

void
SoEngine::startEvaluate()
{
  this->evaluate();
}
