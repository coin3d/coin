/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SBBASIC_H__
#define __SBBASIC_H__

// *************************************************************************

#include <Inventor/confdep.h>

// *************************************************************************

/*!
  \defgroup base Base classes
 */
/*!
  \defgroup general General classes
*/
/*!
  \defgroup events Event classes
*/
/*!
  \defgroup projectors Projector classes
*/
/*!
  \defgroup sensors Sensor classes
*/
/*!
  \defgroup fields Field classes
*/
/*!
  \defgroup nodes Nodes
*/
/*!
  \defgroup lists List classes
*/
/*!
  \defgroup vrml2 VRML 2.0
*/



/*!
  \class SbBool SbBasic.h Inventor/SbBasic.h
  \brief SbBool is a compiler portable boolean type.
  \ingroup base

  SbBool is meant to be a compiler portable way of defining a boolean
  type, since there's older compilers out there that doesn't
  support the \c bool keyword.

  SbBool is not really a class, just a \c typedef.
 */

typedef int SbBool;

#ifdef _WIN32 // FIXME: this should be a HAVE_M_PI_DEFINITION check. 19990808 mortene.
#define M_PI 3.14159265358979323846
#endif // _WIN32

#ifndef FALSE
#define FALSE 0
#define TRUE  1
#endif // ! FALSE

// Some useful inline template (instead of macro) functions:
//   SbAbs( Val )              - returns absolute value
//   SbMin( Val1, Val2 )       - returns minimum value
//   SbMax( Val1, Val2 )       - returns maximum value
//   SbClamp( Val, Min, Max )  - returns clamped value
//   SbSwap( Val1, Val2 )      - swaps the two values (no return value)
//
// FIXME: make Doxygen doc for these methods. 19990425 mortene.

template <class Type>
inline Type SbAbs( Type Val ) {
    return (Val < 0) ? 0 - Val : Val;
}; // SbAbs()

template <class Type>
inline Type SbMax( const Type A, const Type B ) {
    return (A < B) ? B : A;
}; // SbMax()

template <class Type>
inline Type SbMin( const Type A, const Type B ) {
    return (A < B) ? A : B;
}; // SbMin()

template <class Type>
inline Type SbClamp( const Type Val, const Type Min, const Type Max ) {
    return (Val < Min) ? Min : (Val > Max) ? Max : Val;
}; // SbClamp()

template <class Type>
inline void SbSwap( Type & A, Type & B ) {
    Type T; T = A; A = B; B = T;
} // SbSwap()

// *************************************************************************

// Make sure we don't compile in the assert() code in the release
// version. Note: no direct testing on the NDEBUG flag should be made
// in the Coin library, use the COIN_DEBUG flag instead.
#ifndef COIN_DEBUG
#define NDEBUG 1
#endif // !COIN_DEBUG

// *************************************************************************

// A unique identifier to recognize whether or not we're running under
// Systems in Motion's Coin library.
#define __COIN__

// These are available for adding or omitting features based on Coin
// verson numbers in "client" sources.
#define COIN_VERSION_MAJOR 0
#define COIN_VERSION_MINOR 9
#define COIN_VERSION_MICRO 0
#define COIN_VERSION_STR "0.9.0"

#endif // !__SBBASIC_H__
