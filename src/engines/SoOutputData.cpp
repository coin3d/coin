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
  \class SoEngineOutputData SoOutputData.h Inventor/engines/SoOutputData.h
  \brief The SoEngineOutputData class is a container for a prototype set of outputs.

  This class is instantiated once for each class of objects which use
  outputs, and which needs to be able to import and export them.

  Each output of a class is stored with the name and type it has been
  given within its "owner" class and a pointer offset to the dynamic
  instance of the output itself.

  It is unlikely that application programmers should need to use any
  of the methods of this class directly.
*/

/* IMPORTANT NOTE:
 * If you make any changes (bugfixes, improvements) in this class,
 * remember to also check the SoFieldData class, as this code is heavily
 * based on that class.
 */

#include <Inventor/engines/SoOutputData.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoType.h>
#include <coindefs.h> // COIN_STUB()

#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoOutputDataEntry {
public:
  SoOutputDataEntry(const char * n, SoType t, int offset)
    : name(n), type(t), ptroffset(offset) { }
  SoOutputDataEntry(const SoOutputDataEntry * oe) { this->copy(oe); }
  SoOutputDataEntry(const SoOutputDataEntry & oe) { this->copy(&oe); }

  int operator==(const SoOutputDataEntry * oe) const
  {
    return ((this->name == oe->name) && (this->ptroffset == oe->ptroffset) &&
            (this->type == oe->type));
  }
  int operator!=(const SoOutputDataEntry * oe) const { return ! operator==(oe); }
  int operator==(const SoOutputDataEntry & oe) const { return operator==(&oe); }
  int operator!=(const SoOutputDataEntry & oe) const { return ! operator==(&oe); }

  SbName name;
  SoType type;
  int ptroffset;

private:
  void copy(const SoOutputDataEntry * oe)
  {
    this->name = oe->name;
    this->type = oe->type;
    this->ptroffset = oe->ptroffset;
  }
};

#endif // DOXYGEN_SKIP_THIS

/*!
  Constructor.
*/
SoEngineOutputData::SoEngineOutputData()
{
}

/*!
  Copy constructor.
*/
SoEngineOutputData::SoEngineOutputData(const SoEngineOutputData *data)
{
  if (data) {
    int n = data->outputlist.getLength();
    for (int i = 0; i < n; i++) {
      this->outputlist.append(new SoOutputDataEntry(data->outputlist[i]));
    }
  }
}

/*!
  Constructor. Supply the approximated number of outputs in the engine
  ot optimize memory allocation.
*/
SoEngineOutputData::SoEngineOutputData(int approxnum)
  : outputlist(approxnum)
{
}

/*!
  Destructor.
*/
SoEngineOutputData::~SoEngineOutputData()
{
  for (int i = 0; i < this->outputlist.getLength(); i++) {
    delete this->outputlist[i];
  }
}

/*!
  Add a new output to our internal list.

  The \a name and \a type will be stored along with a pointer offset
  between \a base and \a output, which will be valid for all instances
  of the class type of \a base.
*/
void
SoEngineOutputData::addOutput(const SoEngine *base, const char *name,
                              const SoEngineOutput *output, SoType type)
{
  char * vbase = (char *)base;
  char * voutput = (char *)output;
  this->outputlist.append(new SoOutputDataEntry(name, type, voutput-vbase));
}

/*!
  Returns the number of outputs contained within this instance.
*/
int
SoEngineOutputData::getNumOutputs(void) const
{
  return this->outputlist.getLength();
}

/*!
  Returns the name of the output at \a index.
*/
const SbName &
SoEngineOutputData::getOutputName(int index) const
{
  return this->outputlist[index]->name;
}

/*!
  Returns a pointer to the output at \a index within the \a engine
  instance.
*/
SoEngineOutput *
SoEngineOutputData::getOutput(const SoEngine *engine, int index) const
{
  assert(index >= 0 && index < this->outputlist.getLength());
  char * outputptr = (char *)engine;
  outputptr += this->outputlist[index]->ptroffset;
  return (SoEngineOutput *)outputptr;
}

/*!
  Returns the internal index value of \a output in \a engine. If \a output
  is not part of \a engine, it returns -1.
*/
int
SoEngineOutputData::getIndex(const SoEngine *engine,
                             const SoEngineOutput *output) const
{
  char * vbase = (char *)engine;
  char * voutput = (char *)output;
  int ptroffset = voutput - vbase;

  for (int i = 0; i < this->outputlist.getLength(); i++) {
    if (this->outputlist[i]->ptroffset == ptroffset) return i;
  }
  return -1;
}

/*!
  Returns the type of the output at \a index.
*/
const SoType &
SoEngineOutputData::getType(int index) const
{
  return this->outputlist[index]->type;
}

/*!
  FIXME: doc
*/
SbBool
SoEngineOutputData::readDescriptions(SoInput *in, SoEngine *engine) const
{
  COIN_STUB();
  return FALSE;
}

/*
  FIXME: doc.
*/
void
SoEngineOutputData::writeDescriptions(SoOutput *out, SoEngine *engine) const
{
  COIN_STUB();
}
