//$ TEMPLATE SFNodeAndEngine(_TYPENAME_, _Typename_)

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOSF_TYPENAME__H
#define COIN_SOSF_TYPENAME__H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>

class So_Typename_;


class COIN_DLL_API SoSF_Typename_ : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSF_Typename_, So_Typename_ *, So_Typename_ *);

public:
  static void initClass(void);

  virtual void fixCopy(SbBool copyconnections);
  virtual SbBool referencesCopy(void) const;

private:
  virtual void countWriteRefs(SoOutput * out) const;

  // For accessing the readValue() and writeValue() methods.
  friend class SoMF_Typename_;
};

#endif // !COIN_SOSF_TYPENAME__H
