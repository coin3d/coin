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

#ifndef COIN_SOCALCULATOR_H
#define COIN_SOCALCULATOR_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/lists/SbList.h>

struct so_eval_node;

class COIN_DLL_EXPORT SoCalculator : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoCalculator);

public:
  // inputs
  SoMFFloat a, b, c, d, e, f, g, h;
  SoMFVec3f A, B, C, D, E, F, G, H;
  SoMFString expression;

  // outputs
  SoEngineOutput oa, ob, oc, od; // (SoMFFloat)
  SoEngineOutput oA, oB, oC, oD; // (SoMFVec3f)

  SoCalculator(void);

  static void initClass(void);

protected:
  ~SoCalculator(void);

  virtual void inputChanged(SoField * which);

private:
  virtual void evaluate(void);
  SbList <struct so_eval_node*> evaluatorList;

  static void readfieldcb(const char *name, float *data, void *cbdata);
  static void writefieldcb(const char *name, float *data, int comp, void *cbdata);

  void evaluateExpression(struct so_eval_node *node, const int fieldidx);
  void findUsed(struct so_eval_node *node, char *inused, char *outused);

  float ta_th[8];
  SbVec3f tA_tH[8];

  float a_h[8];
  SbVec3f A_H[8];
  float oa_od[4];
  SbVec3f oA_oD[4];
};

#endif // !COIN_SOCALCULATOR_H
