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
  SoBoolOperation::initClass();
  // FIXME: not implemented yet. 19990406 mortene.
//    SoCalculator::initClass();
  SoComposeVec2f::initClass();
  SoComposeVec3f::initClass();
  SoComposeVec4f::initClass();
  SoDecomposeVec2f::initClass();
  SoDecomposeVec3f::initClass();
  SoDecomposeVec4f::initClass();
  SoComposeRotation::initClass();
  // FIXME: not implemented yet. 19990406 mortene.
//    SoComposeRotationFromTo::initClass();
  SoDecomposeRotation::initClass();
  SoComposeMatrix::initClass();
  SoDecomposeMatrix::initClass();
  // FIXME: not implemented yet. 19990406 mortene.
//    SoComputeBoundingBox::initClass();
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
  // FIXME: not implemented yet. 19990406 mortene.
//    SoTimeCounter::initClass();
  // FIXME: not implemented yet. 19990406 mortene.
//    SoTransformVec3f::initClass();
  SoTriggerAny::initClass();
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
