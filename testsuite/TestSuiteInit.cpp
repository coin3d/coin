/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// The purpose of this file is threefold:
// - to initialize Coin before the unit tests are run.
//   (the static initializer class)
// - to compile into the link unit that provides the main() function.
//   (implicitly happens when BOOST_TEST_MODULE is defined before including
//    boost/test/unit_test.hpp)
// - to compile in the whole of Boost.Test so the unit-tests files can
//   include just the declarations (faster) without needing to link against
//   an external Boost.Test library (complicates the build system).
//   (by specifying the include path with the "included" directory)

#define BOOST_TEST_MODULE Coin_TestSuite
#include <boost/test/included/unit_test.hpp>

#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>

#include <TestSuiteUtils.h>

class Initializer {
public:
  Initializer(void) {
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();
    SIM::Coin3D::Coin::TestSuite::Init();
  }
};

static Initializer staticInitializer;
