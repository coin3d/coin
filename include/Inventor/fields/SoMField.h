#ifndef COIN_SOMFIELD_H
#define COIN_SOMFIELD_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/fields/SoField.h>

class SoInput;
class SoOutput;

class COIN_DLL_API SoMField : public SoField {
  typedef SoField inherited;

public:
  virtual ~SoMField();

  static SoType getClassTypeId(void);

  int getNum(void) const;
  void setNum(const int num);

  virtual void deleteValues(int start, int num = -1);
  virtual void insertSpace(int start, int num);

  SbBool set1(const int index, const char * const valuestring);
  void get1(const int index, SbString & valuestring);

  static void initClass(void);

  virtual void enableDeleteValues(void);
  virtual SbBool isDeleteValuesEnabled(void);

protected:
  SoMField(void);
  virtual void makeRoom(int newnum);

#ifndef DOXYGEN_SKIP_THIS // Internal methods.
  virtual int fieldSizeof(void) const = 0;
  virtual void * valuesPtr(void) = 0;
  virtual void setValuesPtr(void * ptr) = 0;
  virtual void allocValues(int num);
#endif // DOXYGEN_SKIP_THIS

  int num;
  int maxNum;
  SbBool userDataIsUsed;

private:
  virtual void deleteAllValues(void) = 0;
  virtual void copyValue(int to, int from) = 0;
  virtual SbBool readValue(SoInput * in);
  virtual SbBool read1Value(SoInput * in, int idx) = 0;
  virtual void writeValue(SoOutput * out) const;
  virtual void write1Value(SoOutput * out, int idx) const = 0;
  virtual SbBool readBinaryValues(SoInput * in, int num);
  virtual void writeBinaryValues(SoOutput * out) const;
  virtual int getNumValuesPerLine(void) const;

  static SoType classTypeId;
};

// inline methods

inline int
SoMField::getNum(void) const
{
  this->evaluate();
  return this->num;
}

#endif // !COIN_SOMFIELD_H
