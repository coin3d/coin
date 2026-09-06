/**************************************************************************\
* Copyright (c) Kongsberg Oil & Gas Technologies AS
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

#include <Inventor/SbVec4ub.h>
#include <Inventor/SbVec4us.h>
#include <Inventor/SbVec4ui32.h>
#include <Inventor/SbColor4f.h>

#include <TestSuiteMisc.h>

std::ostream &
operator << (std::ostream & os, const SbVec4ub & vec)
{
  os << (int) vec[0];
  os << " ";
  os << (int) vec[1];
  os << " ";
  os << (int) vec[2];
  os << " ";
  os << (int) vec[3];
  return os;
}

std::ostream &
operator << (std::ostream & os, const SbVec4us & vec)
{
  os << vec[0];
  os << " ";
  os << vec[1];
  os << " ";
  os << vec[2];
  os << " ";
  os << vec[3];
  return os;
}

std::ostream &
operator << (std::ostream & os, const SbVec4ui32 & vec)
{
  os << vec[0];
  os << " ";
  os << vec[1];
  os << " ";
  os << vec[2];
  os << " ";
  os << vec[3];
  return os;
}

std::ostream &
operator << (std::ostream & os, const SbColor4f & col4)
{
  os << col4[0];
  os << " ";
  os << col4[1];
  os << " ";
  os << col4[2];
  os << " ";
  os << col4[3];
  return os;
}

BOOST_AUTO_TEST_CASE(floatEquals_negativeZero)
{
  // -0.0f and 0.0f are numerically equal but have different raw bit
  // patterns (sign bit set vs. clear). floatEquals()'s ULP comparison
  // (above in TestSuiteMisc.h) relies on remapping negative floats'
  // raw bits into the same lexicographic order as positive ones,
  // which requires reading those bits through a *signed* integer type
  // so the "< 0" check can detect the sign bit at all -- with an
  // unsigned type that check can never trigger, no remapping happens,
  // and the two zeros' raw bit patterns end up ~2^31 apart instead of
  // equal.
  BOOST_CHECK_MESSAGE(floatEquals(-0.0f, 0.0f, 1u),
                      "-0.0f and 0.0f must compare almost-equal");

  // Sanity: a genuinely large difference must still compare unequal.
  BOOST_CHECK_MESSAGE(!floatEquals(0.0f, 1.0f, 4u),
                      "clearly different values must not compare equal");
}
