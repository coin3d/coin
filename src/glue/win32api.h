#ifndef COIN_WIN32API_H
#define COIN_WIN32API_H

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

/* This is an internal set of convenience wrappers around Win32 API
   functions that are unnecessary robust for our purpose.

   To simplify our client code within the library, we therefore catch
   error conditions, report as exact information about the error as
   possible, and then assert on them within the wrappers. The client
   code can therefore ignore possible problems.
*/

/*************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifndef HAVE_WIN32_API
/* Just in case we can get in trouble for declaring functions that are
   not implemented. */
#error Do not include this file unless the contents will actually be used.
#endif /* HAVE_WIN32_API */

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H

/*************************************************************************/

#include <Inventor/C/basic.h>

/*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  typedef int (WINAPI * GetTextFace_t)(HDC, int, LPTSTR);
  typedef void (WINAPI * LocalFree_t)(HLOCAL);
  typedef void (WINAPI * GetVersionEx_t)(LPOSVERSIONINFO);
  typedef HGDIOBJ (WINAPI * SelectObject_t)(HDC, HGDIOBJ);
  typedef int (WINAPI * GetObject_t)(HGDIOBJ, int, LPVOID);
 
  struct cc_win32_api {
    GetTextFace_t GetTextFace;
    LocalFree_t LocalFree;
    GetVersionEx_t GetVersionEx;
    SelectObject_t SelectObject;
    GetObject_t GetObject;
  };

  /* Force singleton access. */
  const struct cc_win32_api * cc_win32(void);

  /* Convenient wrapping of converting and printing out a Win32 API
     error code as a text string. The "lasterror" argument should be
     the result from GetLastError(). */
  void cc_win32_print_error(const char * callerfuncname,
                            const char * apifuncname,
                            DWORD lasterror);
  
#ifdef __cplusplus
}
#endif

#endif /* !COIN_WIN32API_H */
