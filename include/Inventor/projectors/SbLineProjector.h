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

#ifndef COIN_SBLINEPROJECTOR_H
#define COIN_SBLINEPROJECTOR_H

#include <Inventor/projectors/SbProjector.h>

#include <Inventor/SbVec3f.h>
#include <Inventor/SbLine.h>


class COIN_DLL_API SbLineProjector : public SbProjector {
  typedef SbProjector inherited;

public:
  SbLineProjector(void);
  virtual SbProjector * copy(void) const;

  virtual SbVec3f project(const SbVec2f & point);
  void setLine(const SbLine & line);
  const SbLine & getLine(void) const;
  virtual SbVec3f getVector(const SbVec2f & viewpos1,
                            const SbVec2f & viewpos2);
  virtual SbVec3f getVector(const SbVec2f & viewpos);
  void setStartPosition(const SbVec2f & viewpos);
  void setStartPosition(const SbVec3f & point);

protected:
  SbLine line;
  SbVec3f lastPoint;
};

#endif // !COIN_SBLINEPROJECTOR_H
