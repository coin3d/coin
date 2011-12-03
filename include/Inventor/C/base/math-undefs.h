#ifndef COIN_MATH_UNDEFS_H
#define COIN_MATH_UNDEFS_H

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

#ifndef COIN_INTERNAL
#error This header is only included while building Coin, and this is not while building Coin it seems.
#endif // !COIN_INTERNAL

/* *********************************************************************** */

/*
  The float-version of the math functions below did not come about
  until C99, so we don't want to use them internally, for portability
  reasons.

  This block was originally part of Inventor/C/basic.h, but since the
  #undef's were mangled by the config.status process, it did not really
  work as intended.  20070518 larsa
*/

#undef cosf
#define cosf(x) NO_SINGLEPREC /* whatever that'll give us a compile error... */
#undef sinf
#define sinf(x) NO_SINGLEPREC
#undef tanf
#define tanf(x) NO_SINGLEPREC
#undef powf
#define powf(x) NO_SINGLEPREC
#undef sqrtf
#define sqrtf(x) NO_SINGLEPREC
#undef acosf
#define acosf(x) NO_SINGLEPREC
#undef asinf
#define asinf(x) NO_SINGLEPREC
#undef atanf
#define atanf(x) NO_SINGLEPREC
#undef atan2f
#define atan2f(x) NO_SINGLEPREC

/* *********************************************************************** */

#endif // !COIN_MATH_UNDEFS_H
