/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stddef.h>

#ifdef _WIN32 /* FIXME: use a proper check for the basic Win32 API. 20020502 mortene. */
#define HAVE_WINDOWS_H 1
#define HAVE_WIN32_FORMATMESSAGE 1
#define HAVE_WIN32_LOCALFREE 1
#endif /* _WIN32 */

#if HAVE_WINDOWS_H
#include <windows.h>
#endif /* HAVE_WINDOWS_H */
#include <Inventor/C/base/basep.h>

char *
cc_internal_w32_getlasterrorstring(int32_t lasterror)
{
  char * lpMsgBuf = NULL;
#if HAVE_WIN32_FORMATMESSAGE
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  (DWORD)lasterror,
                  /* Default language */
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  &lpMsgBuf,
                  0,
                  NULL);
#endif /* HAVE_WIN32_FORMATMESSAGE */
  return (char *)lpMsgBuf;
}

void
cc_internal_w32_freelasterrorstring(char * str)
{
#if HAVE_WIN32_LOCALFREE
  LocalFree((LPVOID)str);
#endif /* HAVE_WIN32_LOCALFREE */
}

