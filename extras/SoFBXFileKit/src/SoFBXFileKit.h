#ifndef COIN_SOFBXFILEKIT_H
#define COIN_SOFBXFILEKIT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "win_api.h"
#include <Inventor/annex/ForeignFiles/SoForeignFileKit.h>

namespace { class SoFBXFileKitP; }

class FBX_DLL_API SoFBXFileKit : public SoForeignFileKit {
  typedef SoForeignFileKit inherited;

  SO_KIT_HEADER(SoFBXFileKit);

public:
  static void initClass(void);
  SoFBXFileKit(void);

  static SbBool identify(const char * filename);
  virtual SbBool canReadFile(const char * filename = NULL) const;
  virtual SbBool readFile(const char * filename);
  virtual SoSeparator *convert();

  virtual SbBool canWriteFile(const char * filename = NULL) const;
  virtual SbBool writeFile(const char * filename);

protected:
  virtual ~SoFBXFileKit(void);

private:
  SoFBXFileKitP * pimpl;

};

#endif // !COIN_SOFBXFILEKIT_H
