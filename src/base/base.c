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

#if HAVE_CONFIG_H
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

