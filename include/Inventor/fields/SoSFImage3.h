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

#ifndef COIN_SOSFIMAGE3_H
#define COIN_SOSFIMAGE3_H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec3s.h>

class COIN_DLL_API SoSFImage3 : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_CONSTRUCTOR_HEADER(SoSFImage3);
  SO_SFIELD_REQUIRED_HEADER(SoSFImage3);

public:
  static void initClass(void);

  const unsigned char *getValue(SbVec3s &size, int &nc) const;
  void setValue(const SbVec3s &size, const int nc,
                const unsigned char *bytes);

  int operator==(const SoSFImage3 & field) const;
  int operator!=(const SoSFImage3 & field) const { 
    return ! operator == (field); 
  }

  unsigned char * startEditing(SbVec3s &size, int &nc);
  void finishEditing(void);

private:
  virtual SbBool readValue(SoInput *in);
  virtual void writeValue(SoOutput *out) const;

  class SbImage *image;
};

#endif // !COIN_SOSFIMAGE3_H
