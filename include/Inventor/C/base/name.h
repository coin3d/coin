#ifndef CC_NAME_H
#define CC_NAME_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

COIN_DLL_API cc_name * cc_name_construct(void);
COIN_DLL_API cc_name * cc_name_clone(cc_name * name);
COIN_DLL_API void cc_name_destruct(cc_name * name);

/*
SbName(const char * nameString);
SbName(const SbString & str);
*/

COIN_DLL_API const char * cc_name_get_string(cc_name * name);
COIN_DLL_API unsigned int cc_name_get_length(cc_name * name);

COIN_DLL_API int cc_name_equals(cc_name * name, cc_name * name2);
COIN_DLL_API int cc_name_equals_string(cc_name * name, const char * string);

/*
static SbBool isIdentStartChar(const char c);
static SbBool isIdentChar(const char c);
static SbBool isBaseNameStartChar(const char c);
static SbBool isBaseNameChar(const char c);

int operator ! (void) const;
*/

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#endif /* ! CC_NAME_H */
