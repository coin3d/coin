#ifndef COIN_WIN32API_H
#define COIN_WIN32API_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/* This is an internal set of convenience wrappers around Win32 API
   functions that are unnecessary robust for our purpose.

   To simplify our client code within the library, we therefore catch
   error conditions, report as exact information about the error as
   possible, and then assert on them within the wrappers. The client
   code can therefore ignore possible problems.
*/

/*************************************************************************/

#ifndef COIN_INTERNAL
#error Only for internal use.
#endif /* COIN_INTERNAL */

/* FIXME: this should really test a configure check for the presence
   of the Win32 API. 20030530 mortene. */
#ifndef _WIN32
/* Just in case we can get in trouble for declaring functions that are
   not implemented. */
#error Do not include this file unless the contents will actually be used.
#endif /* _WIN32 */

/*************************************************************************/

#include <Inventor/C/basic.h>

/*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  typedef int (WINAPI * GetTextFace_t)(HDC, int, LPTSTR);

  struct cc_win32_api {
    GetTextFace_t GetTextFace;
  };

  /* Force singleton access. */
  const struct cc_win32_api * cc_win32(void);

  /* Convenient wrapping of converting and printing out an error code
     as a text string. */
  void cc_win32_print_error(const char * callerfuncname, const char * apifuncname, DWORD lasterror);
  
#ifdef __cplusplus
}
#endif

#endif /* !COIN_WIN32API_H */
