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

#ifndef __SOMFSTRING_H__
#define __SOMFSTRING_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbString.h>


class SoMFString : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFString, SbString, const SbString &);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const char * const strings[]);
  void setValue(const char * const string);
  void deleteText(const int fromLine, const int fromChar,
                  const int toLine, const int toChar);

  // SbString values need to have their constructors called.
  virtual void allocValues(int num);

private:
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFSTRING_H__
