#ifndef COIN_SOMFPATH_H
#define COIN_SOMFPATH_H

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

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/lists/SbList.h>

class SoPath;
class SoNotList;
class SoNode;

class COIN_DLL_API SoMFPath : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFPath, SoPath *, SoPath *);

public:
  static void initClass(void);

  virtual void notify(SoNotList * l);
  virtual void fixCopy(SbBool copyconnections);
  virtual SbBool referencesCopy(void) const;

  virtual void deleteValues(int start, int num = -1);
  virtual void insertSpace(int start, int num);

private:
  virtual void countWriteRefs(SoOutput * out) const;

  SbList<SoNode *> pathheads;
};

#endif // !COIN_SOMFPATH_H
