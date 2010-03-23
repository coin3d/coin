#ifndef COIN_SBBASIC_H
#define COIN_SBBASIC_H

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

#include <Inventor/C/basic.h>

/* ********************************************************************** */
/* Trap people trying to use Inventor headers while compiling C source code.
 * (we get support mail about this from time to time)
 */
#ifndef __cplusplus
#error You are not compiling C++ - maybe your source file is named <file>.c
#endif

/* ********************************************************************** */
/* Include these for Open Inventor compatibility reasons (they are not
 * actually used in Coin.)
 */
#define SoEXTENDER
#define SoINTERNAL

/* ********************************************************************** */

/* Some useful inline template functions:
 *   SbAbs(Val)              - returns absolute value
 *   SbMin(Val1, Val2)       - returns minimum value
 *   SbMax(Val1, Val2)       - returns maximum value
 *   SbClamp(Val, Min, Max)  - returns clamped value
 *   SbSwap(Val1, Val2)      - swaps the two values (no return value)
 *   SbSqr(val)              - returns (val)²
 */

template <class Type>
inline Type SbAbs( Type Val ) {
  return (Val < 0) ? 0 - Val : Val;
}

template <class Type>
inline Type SbMax( const Type A, const Type B ) {
  return (A < B) ? B : A;
}

template <class Type>
inline Type SbMin( const Type A, const Type B ) {
  return (A < B) ? A : B;
}

template <class Type>
inline Type SbClamp( const Type Val, const Type Min, const Type Max ) {
  return (Val < Min) ? Min : (Val > Max) ? Max : Val;
}

template <class Type>
inline void SbSwap( Type & A, Type & B ) {
  Type T; T = A; A = B; B = T;
}

template <class Type>
inline Type SbSqr(const Type val) {
  return val * val;
}

/* *********************************************************************** */

// SbDividerChk() - checks if divide-by-zero is attempted, and emits a
// warning if so for debug builds.  inlined like this to not take much
// screenspace in inline functions.

// cc_debugerror_post() is not attempted resolved before the template is
// used, hence the missing Inventor/errors/SoDebugError.h #include. This
// "trick" does only work *portably* for functions in the global namespace.

template <typename Type>
inline void SbDividerChk(const char * funcname, Type divider) {
#ifndef NDEBUG
  if (!(divider != static_cast<Type>(0)))
    cc_debugerror_post(funcname, "divide by zero error.", divider);
#endif // !NDEBUG
}

/* ********************************************************************** */

/* COMPILER BUG WORKAROUND:

   We've had reports that Sun CC v4.0 is (likely) buggy, and doesn't
   allow a statement like

     SoType SoNode::classTypeId = SoType::badType();

   As a hack we can however get around this by instead writing it as

     SoType SoNode::classTypeId;

   ..as the SoType variable will then be initialized to bitpattern
   0x0000, which equals SoType::badType(). We can *however* not do
   this for the Intel C/C++ compiler, as that does *not* init to the
   0x0000 bitpattern (which may be a separate bug -- I haven't read
   the C++ spec closely enough to decide whether that relied on
   unspecified behavior or not).

   The latest version of the Intel compiler has been tested to still
   have this problem, so I've decided to re-install the old code, but
   in this form:

     SoType SoNode::classTypeId STATIC_SOTYPE_INIT;

   ..so it is easy to revert if somebody complains that the code
   reversal breaks their old Sun CC 4.0 compiler -- see the #define of
   STATIC_SOTYPE_INIT below.

   If that happens, we should work with the reporter, having access to
   the buggy compiler, to make a configure check which sets the
   SUN_CC_4_0_SOTYPE_INIT_BUG #define in include/Inventor/C/basic.h.in.

   (Note that the Sun CC compiler has moved on, and a later version
   we've tested, 5.4, does not have the bug.)

   20050105 mortene.
*/

#define SUN_CC_4_0_SOTYPE_INIT_BUG 0 /* assume compiler is ok for now */

#if SUN_CC_4_0_SOTYPE_INIT_BUG
#define STATIC_SOTYPE_INIT
#else
#define STATIC_SOTYPE_INIT = SoType::badType()
#endif

/* ********************************************************************** */

#endif /* !COIN_SBBASIC_H */
