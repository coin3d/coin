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

#ifndef __SOMFPATH_H__
#define __SOMFPATH_H__

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>

class SoPath;
class SoNotList;


class SoMFPath : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFPath, SoPath *, SoPath *);

public:
  static void initClass(void);

  virtual void notify(SoNotList * list);
  virtual void fixCopy(SbBool copyConnections);
  virtual SbBool referencesCopy(void) const;

private:
  virtual void countWriteRefs(SoOutput * out) const;
  virtual void convertTo(SoField * dest) const;
};

#endif // !__SOMFPATH_H__
