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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* FIXME: this should really test a configure check for the presence
   of the Win32 API. 20030530 mortene. */
#ifdef _WIN32

#include <assert.h>
#include <stdlib.h>

#include <windows.h>

#include <Inventor/C/glue/win32api.h>
#include <Inventor/C/errors/debugerror.h>

/* internal helper function */
void
cc_win32_print_error(const char * callerfuncname, const char * apifuncname,
                     DWORD lasterror)
{
  char * outputstr = NULL;
  LPTSTR buffer = NULL;
  BOOL result = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                              FORMAT_MESSAGE_FROM_SYSTEM |
                              FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL,
                              lasterror,
                              0,
                              (LPTSTR)&buffer,
                              0,
                              NULL);
  if (!result) {
    cc_debugerror_post("cc_win32_print_error", "FormatMessage() failed!");
    return;
  }

#ifdef UNICODE
  { /* Narrow from wide characters to 8-bit characters. */
    size_t len;
    len = wcstombs(NULL, buffer, 0);
    outputstr = (char *)LocalAlloc(0, len + 1);
    if (!outputstr) {
      cc_debugerror_post("cc_win32_print_error",
                         "LocalAlloc() failed! (errorcode %d)",
                         GetLastError());
      goto exitfunc;
    }

    if (wcstombs(outputstr, buffer, len + 1) != len) {
      cc_debugerror_post("cc_win32_print_error",
                         "UNICODE to mbs conversion failed!");
      outputstr[0] ='\0';
      goto exitfunc;
    }
  }
#else /* !UNICODE */
  outputstr = (char *)buffer;
#endif /* !UNICODE */

  cc_debugerror_post(callerfuncname, "%s failed: '%s'", apifuncname, outputstr);

#ifdef UNICODE
exitfunc:
#endif /* UNICODE */

  /* Don't call coin_LocalFree() here, to make sure we don't get a
     recursive call back here again. */
  if (buffer && LocalFree(buffer) != NULL) {
    cc_debugerror_post("cc_win32_print_error",
                       "LocalFree() failed! (errorcode %d)",
                       GetLastError());
  }
  if (outputstr && (outputstr != (char *)buffer)) {
    if (LocalFree(outputstr) != NULL) {
      cc_debugerror_post("cc_win32_print_error",
                         "LocalFree() failed! (errorcode %d)",
                         GetLastError());
    }
  }
}


static int WINAPI
coin_GetTextFace(HDC hdc, /* handle to device context */
                 int nCount, /* length of buffer receiving typeface name */
                 LPTSTR lpFaceName) /* pointer to buffer receiving typeface name */
{
  const int copied = GetTextFace(hdc, nCount, lpFaceName);
  if (copied == 0) {
    cc_win32_print_error("coin_GetTextFace", "GetTextFace()", GetLastError());
    assert(FALSE && "unexpected error");
  }
  return copied;
}

static void WINAPI
coin_LocalFree(HLOCAL hMem) /* handle to local memory object */
{
  const HLOCAL ptr = LocalFree(hMem);
  if (ptr != NULL) {
    cc_win32_print_error("coin_LocalFree", "LocalFree()", GetLastError());
    assert(FALSE && "unexpected error");
  }
}

/* singleton access to the structure, so we can initialize it at first
   use */
const struct cc_win32_api *
cc_win32(void)
{
  static struct cc_win32_api * instance = NULL;

  if (instance) { return instance; }

  /* FIXME: one-time mem leak, clean up at exit. 20030530 mortene. */
  instance = (struct cc_win32_api *)malloc(sizeof(struct cc_win32_api));
  assert(instance != NULL);

  /* set up all function pointers */
  instance->GetTextFace = coin_GetTextFace;
  instance->LocalFree = coin_LocalFree;

  return instance;
}

#endif /* _WIN32 */
