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

#ifndef __SOMFIELD_H__
#define __SOMFIELD_H__

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSField.h>

class SoInput;
class SoOutput;


class SoMField : public SoField {
  typedef SoField inherited;

public:
  virtual ~SoMField();

  static SoType getClassTypeId(void);

  int getNum(void) const;
  void setNum(const int num);

  virtual void deleteValues(int start, int num = -1);
  virtual void insertSpace(int start, int num);

  SbBool set1(const int index, const char * const valueString);
  void get1(const int index, SbString & valueString);

  static void initClass(void);

protected:
  SoMField(void);
  virtual void makeRoom(int newNum);
  virtual int fieldSizeof(void) const;
  virtual void * valuesPtr(void);
  virtual void setValuesPtr(void * ptr);
  virtual void allocValues(int num);

  int num;
  int maxNum;

private:
  virtual void deleteAllValues(void) = 0;
  virtual void copyValue(int to, int from) = 0;
  virtual SbBool readValue(SoInput * in);
  virtual SbBool read1Value(SoInput * in, int idx) = 0;
  virtual void writeValue(SoOutput * out) const;
  virtual void write1Value(SoOutput * out, int idx) const = 0;
  virtual SbBool readBinaryValues(SoInput * in, int numToRead);
  virtual void writeBinaryValues(SoOutput * out) const;
  virtual int getNumValuesPerLine(void) const;
  virtual void convertTo(SoField * dest) const;

  static SoType classTypeId;
};

#endif // !__SOMFIELD_H__
