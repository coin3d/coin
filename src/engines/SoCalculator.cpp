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


#include <Inventor/engines/SoCalculator.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>
#include <assert.h>

/* ChangeLog / General Messages
 *
 * 990520 larsa:    started implementation
 *
 */

/*!
  \class SoCalculator Inventor/engines/SoCalculator.h
  \brief General purpose calculator for floats and 3D float vectors.
*/

SO_ENGINE_SOURCE(SoCalculator);

SoCalculator::SoCalculator(void)
{
  SO_ENGINE_CONSTRUCTOR(SoCalculator);

  SO_ENGINE_ADD_INPUT(a, (0.0f));
  SO_ENGINE_ADD_INPUT(b, (0.0f));
  SO_ENGINE_ADD_INPUT(c, (0.0f));
  SO_ENGINE_ADD_INPUT(d, (0.0f));
  SO_ENGINE_ADD_INPUT(e, (0.0f));
  SO_ENGINE_ADD_INPUT(f, (0.0f));
  SO_ENGINE_ADD_INPUT(g, (0.0f));
  SO_ENGINE_ADD_INPUT(h, (0.0f));
  SO_ENGINE_ADD_INPUT(A, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(B, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(C, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(D, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(E, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(F, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(G, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(H, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_ENGINE_ADD_INPUT(expression, (""));

  SO_ENGINE_ADD_OUTPUT(oa, SoMFFloat); 
  SO_ENGINE_ADD_OUTPUT(ob, SoMFFloat); 
  SO_ENGINE_ADD_OUTPUT(oc, SoMFFloat); 
  SO_ENGINE_ADD_OUTPUT(od, SoMFFloat); 
  SO_ENGINE_ADD_OUTPUT(oA, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(oB, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(oC, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(oD, SoMFVec3f);
}

SoCalculator::~SoCalculator(void)
{
}

void
SoCalculator::initClass(void)
{
  SO_ENGINE_INIT_CLASS(SoCalculator, SoEngine, "Engine");
}

void
SoCalculator::evaluate(void)
{
}

void
SoCalculator::inputChanged(SoField * which)
{
}
