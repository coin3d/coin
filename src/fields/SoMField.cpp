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

/*!
  \class SoMField SoMField.h Inventor/fields/SoMField.h
  \brief The SoMField class ...
  \ingroup fields

  FIXME: write class doc
*/

#include <Inventor/fields/SoMField.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>
#include <malloc.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var int SoMField::num
  FIXME: write doc
*/
/*!
  \var int SoMField::maxNum
  FIXME: write doc
*/


SoType SoMField::classTypeId = SoType::badType();

/*!
  FIXME: write function documentation
*/
int
SoMField::fieldSizeof(void) const
{
  assert(0);
  return 0; // should never be called
}

/*!
  FIXME: write function documentation
*/
void *
SoMField::valuesPtr(void)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoMField::setValuesPtr(void *)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SoType
SoMField::getClassTypeId(void)
{
  return SoMField::classTypeId;
}

/*!
  Does initialization common for all objects of the
  SoMField class. This includes setting up the
  type system, among other things.
*/
void
SoMField::initClass(void)
{
  PRIVATE_FIELD_INIT_CLASS(SoMField, "MField", inherited, NULL);
}

/*!
  Constructor.
*/
SoMField::SoMField(void)
{
  this->maxNum = this->num = 0;
}

/*!
  Destructor.
*/
SoMField::~SoMField()
{
}

/*!
  FIXME: write function documentation
*/
void
SoMField::makeRoom(int newNum)
{
  assert(newNum >= 0);
  if (newNum != num) this->allocValues(newNum);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMField::set1(const int index, const char * const valueString)
{
  SoInput in;

  in.setBuffer((void *)valueString, strlen(valueString));

  this->read1Value(&in, index);
  this->touch();
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoMField::get1(const int index, SbString & valueString)
{
  SoOutput out;
  void * buffer=malloc(10);
  size_t size;

  out.setHeaderString("");
  out.setBuffer(buffer, 10, SoField::reallocOutputBuf);

  this->evaluate();
  this->write1Value(&out, index);

  out.getBuffer(buffer, size);
  valueString=(char *)buffer;

  out.resetHeaderString();
  free(buffer);
}

/*!
  Read and set values for this field from input stream \a in.
*/
SbBool
SoMField::readValue(SoInput * in)
{
  if (in->isBinary()) {
    int numtoread;
    if (!in->read(numtoread)) return FALSE;
    // FIXME: sanity check on number of values to read. 19990713 mortene.
    if (numtoread) this->makeRoom(numtoread);
    return this->readBinaryValues(in, numtoread);
  }

  char c;
  int curIndex = 0;

  if (in->read(c) && c == '[') {
    if (in->read(c) && c == ']') {}
    else {
      in->putBack(c);

      while (TRUE) {
        if (curIndex >= num) this->makeRoom(curIndex + 1);
#if 0 // FIXME: temporary disabled, should move progress code to SoInput. 19980913 mortene.
        if (curIndex && (curIndex%500 == 0)) {
          if (!SoDB::progress(in)) return FALSE;
        }
#endif
        if (!this->read1Value(in, curIndex++) || !in->read(c)) {
          SoReadError::post(in, "Couldn't read value %d of field",
                            curIndex);
          return FALSE;
        }

        if (c == ',') {
          if (in->read(c)) {
            if (c == ']') break;
            else in->putBack(c);
          }
        }
        else if (c == ']')
          break;
        else
          in->putBack(c);
      }
    }

    if (curIndex < num) this->makeRoom(curIndex);
  }

  else {
    in->putBack(c);
    this->makeRoom(1);
    if (!this->read1Value(in, 0)) return FALSE;
  }

  return TRUE;
}

/*!
  Write all field values to \a out.
*/
void
SoMField::writeValue(SoOutput * out) const
{
  if (out->isBinary()) {
    this->writeBinaryValues(out);
    return;
  }

  SbBool indented = FALSE;

  const int num = this->getNum();
  if ((num > 1) || (num == 0)) out->write("[ ");

  for (int i=0; i < num; i++) {
    this->write1Value(out, i);

    if (i != num-1) {
      if (((i+1) % this->getNumValuesPerLine()) == 0) {
        out->write(",\n");
        if (!indented) {
          out->incrementIndent();
          indented = TRUE;
        }
        out->indent();
        // for alignment
        out->write("  ");
      }
      else {
        out->write(", ");
      }
    }
  }
  if ((num > 1) || (num == 0)) out->write(" ]");

  if (indented) out->decrementIndent();
}

/*!
  Read \a numToRead binary format values from \a in into this field.
*/
SbBool
SoMField::readBinaryValues(SoInput * in, int numToRead)
{
  assert(in->isBinary());

  for (int i=0; i < numToRead; i++) if (!this->read1Value(in, i)) return FALSE;
  return TRUE;
}

/*!
  Write all values of field to \a out in binary format.
*/
void
SoMField::writeBinaryValues(SoOutput * out) const
{
  assert(out->isBinary());

  const int num = this->getNum();
  out->write(num);
  for (int i=0; i < num; i++) this->write1Value(out, i);
}

/*!
  FIXME: write function documentation
*/
int
SoMField::getNumValuesPerLine(void) const
{
  return 1;
}

/*!
  FIXME: write function documentation
*/
int
SoMField::getNum(void) const
{
  this->evaluate();
  return this->num;
}

/*!
  FIXME: write function documentation
*/
void
SoMField::setNum(const int num)
{
  this->allocValues(num);
}

/*!
  FIXME: write function documentation
*/
void
SoMField::deleteValues(int /* start */, int /* num */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMField::insertSpace(int /* start */, int /* num */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoMField::allocValues(int number)
{
  assert(number >= 0);

  if (number == 0) {
    delete (unsigned char *) this->valuesPtr();
    this->setValuesPtr(NULL);
    this->maxNum = 0;
  }
  else if (number > maxNum || number < num) {
    int fsize = this->fieldSizeof();
    if (this->valuesPtr()) {

      // I think this will handle both cases quite gracefully
      // 1) number > maxNum, 2) number < num
      while (number > maxNum) maxNum<<=1;
      while ((maxNum >> 1) >= number) maxNum >>= 1;

#if 0 // debug
      SoDebugError::postInfo("SoMField::allocValues",
                             "%d --> %d, name: '%s', ptr: %p",
                             number, maxNum, getTypeId().getName().getString(),
                             this);
#endif // debug


      unsigned char * newblock = new unsigned char[maxNum * fsize];

      memcpy(newblock, this->valuesPtr(), fsize * SbMin(this->num, number));

      delete (unsigned char *) this->valuesPtr();
      this->setValuesPtr(newblock);
    }
    else {
      this->setValuesPtr(new unsigned char[number * fsize]);
      this->maxNum = number;
    }
  }

  SbBool valchanged = number < this->num ? TRUE : FALSE;
  this->num = number;
  if (valchanged) this->valueChanged();
}

void
SoMField::convertTo(SoField * /* dest */) const
{
  // Should never be called, all subclasses with conversion
  // functionality need to reimplement this method.
  assert(0);
}
