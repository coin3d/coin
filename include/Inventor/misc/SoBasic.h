/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef __SOBASIC_H__
#define __SOBASIC_H__

#include <Inventor/SbBasic.h>

// FIXME: I vaguely remember seeing a check for "quote" functionality
// in an Autoconf macro. If correct, fix up this code. 19991106 mortene.

// SO__QUOTE(str)        - use the preprocessor to quote a string.
// SO__CONCAT(str1,str2) - use the preprocessor to concatenate two strings.

#if defined(__STDC__) || defined(__ANSI_CPP__) || \
    defined(__BEOS__) || defined(_MSC_VER) // ANSI C
#define SO__QUOTE(str)           #str
#define SO__CONCAT(str1, str2)   str1##str2
#else // Non-ANSI C
#define SO__QUOTE(str)           "str"
#define SO__CONCAT(str1, str2)   str1/**/str2
#endif // Non-ANSI C


// FIXME: make an autoconf macro which can do this check. (And look up
// if (and how) the current function name is specified during parsing
// for other compilers than GCC). 19991213 mortene.
#if (!defined __GNUC__ || __GNUC__ < 2 || __GNUC_MINOR__ < (defined __cplusplus ? 6 : 4))
#define COIN_STUB_FUNC ((const char *) 0)
#else
#define COIN_STUB_FUNC __PRETTY_FUNCTION__
#endif

// Implemented in SoError.cpp.
extern void coin_stub(const char *, unsigned int, const char *);

// COIN_STUB() - used to notify about missing functionality at runtime.

#define COIN_STUB() \
  do { coin_stub(__FILE__, __LINE__, COIN_STUB_FUNC); } while (0)

// FIXME: __FILE__ and __LINE__ are ANSI C, I believe, but we should
// probably do an autoconf check for them anyway. 19991213 mortene.

#endif // !__SOBASIC_H__
