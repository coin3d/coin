/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include "CoinTest.h"

/**
 * Verify Coin's GLU runtime-loader contract. In particular, macOS provides
 * GLU as part of the OpenGL framework, so a configured build must resolve the
 * tessellator entry points without requiring a separately linked GLU library.
 */
#define COIN_INTERNAL
#include "config.h"
#include "glue/GLUWrapper.h"

BOOST_AUTO_TEST_SUITE(GLUWrapper_TestSuite)

BOOST_AUTO_TEST_CASE(runtimeLoader)
{
  const GLUWrapper_t * wrapper = GLUWrapper();
  BOOST_REQUIRE(wrapper != NULL);

#ifdef GLU_IS_PART_OF_GL
  BOOST_REQUIRE_EQUAL(wrapper->available, 1);
  BOOST_REQUIRE(wrapper->gluNewTess != NULL);
  BOOST_REQUIRE(wrapper->gluDeleteTess != NULL);

  coin_GLUtessellator * tessellator = wrapper->gluNewTess();
  BOOST_REQUIRE(tessellator != NULL);
  wrapper->gluDeleteTess(tessellator);
#else
  if (wrapper->available) {
    BOOST_CHECK(wrapper->gluNewTess != NULL);
    BOOST_CHECK(wrapper->gluDeleteTess != NULL);
  }
#endif
}

BOOST_AUTO_TEST_SUITE_END()
