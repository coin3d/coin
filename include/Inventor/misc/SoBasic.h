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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_BASIC_H_
#define _SO_BASIC_H_

#include <Inventor/SbBasic.h>

// SO__QUOTE()  - use the preprocessor to quote a string
// SO__CONCAT() - use the preprocessor to concatenate two strings

#if defined(__STDC__) || defined(__ANSI_CPP__) || \
    defined(__BEOS__) || defined(_MSC_VER) // ANSI C
#define SO__QUOTE(str)           #str
#define SO__CONCAT(str1, str2)   str1##str2
#else // Non-ANSI C
#define SO__QUOTE(str)           "str"
#define SO__CONCAT(str1, str2)   str1/**/str2
#endif // Non-ANSI C

#endif // ! _SO_BASIC_H_
