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

#ifndef COIN_SOCALCULATOR_H
#define COIN_SOCALCULATOR_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/lists/SbList.h>

struct so_eval_node;

class SoCalculator : public SoEngine {
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

  void evaluateExpression(struct so_eval_node *node);
  void findUsed(struct so_eval_node *node, char *inused, char *outused);

  SoField *getFieldFake(const char *fieldname);

  float ta_th[8];
  SbVec3f tA_tH[8];

  float a_h[8];
  SbVec3f A_H[8];
  float oa_od[4];
  SbVec3f oA_oD[4];
};

#endif // !COIN_SOCALCULATOR_H
