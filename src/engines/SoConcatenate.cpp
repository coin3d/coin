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
  \class SoConcatenate SoConcatenate.h Inventor/engines/SoConcatenate.h
  \brief The SoConcatenate class is used to concatenate several inputs into one output.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SbString.h>

SO_ENGINE_SOURCE(SoConcatenate);

/*!
  Default constructor.
*/
SoConcatenate::SoConcatenate(SoType inputType)
{
  SO_ENGINE_CONSTRUCTOR(SoConcatenate);

  for (int i=0;i<10;i++)
    this->input[i]=(SoMField *)inputType.createInstance();


  this->output = new SoEngineOutput;
  //FIXME: Use ADD_OUTPUT instead? (SoMField) kintel.
#if 0 // this code is obsoleted. FIXME: pederb
  this->output->setType(inputType);
#endif
  this->output->setContainer(this);

#if 0 // old kintel code. will not work any more FIXME: reimplement
  this->outputList->append(this->output);
#endif // 0
}

// overloaded from parent
void
SoConcatenate::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoConcatenate);
}

//
// private members
//
SoConcatenate::~SoConcatenate()
{
  delete this->output;
}

// overloaded from parent
void
SoConcatenate::evaluate()
{
  int n = 0, i, j;
  SbString value;

  for (i = 0; i < 10; i++)
    n += this->input[i]->getNum();

  SO_ENGINE_OUTPUT((*output),SoMField,setNum(n));

  n = 0;
  for (i = 0; i < 10; i++) {
    for (j = 0; j < this->input[i]->getNum(); j++) {
      this->input[i]->get1(j, value);
      SO_ENGINE_OUTPUT((*output), SoMField, set1(n, value.getString()));
      n++;
    }
  }
}
