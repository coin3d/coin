#ifndef COIN_SOPATTERN_H
#define COIN_SOPATTERN_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/fields/SoSFString.h>
#include <Inventor/nodes/SoSubNode.h>


class COIN_DLL_API SoPattern : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoPattern);

public:
  static void initClass(void);
  SoPattern(void);

  SoSFString category;
  SoSFString name;

  enum FilterType { INCLUSIVE_FILTER, EXCLUSIVE_FILTER };

  static SbBool loadPatterns(const char * filename,
                             int numfilters = 0, 
                             const SbString * filternames = NULL,
                             FilterType filtertype = INCLUSIVE_FILTER);

  static void addPattern(const char * category, const char * name,
                         const unsigned char * bytes);

  static void getPattern(const char * category, const char * name,
                         unsigned char *& bytes);

  static void getPatternNames(int & number, SbString *& names,
                              const char * category = NULL);

protected:
  virtual ~SoPattern();

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);

private:
  class SoPatternP * pimpl;
};

#endif // !COIN_SOPATTERN_H
