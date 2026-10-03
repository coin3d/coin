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

/* See comment block at top of win32api.h header file for the
   rationale behind this wrapper. */

/*************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_WIN32_API

#include "glue/win32api.h"

/* ********************************************************************** */

#include <cassert>
#include <cstdlib>
#include <windows.h>

#include <Inventor/C/errors/debugerror.h>

/* ********************************************************************** */

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
    cc_debugerror_post("cc_win32_print_error",
                       "FormatMessage() failed! "
                       "(callerfuncname=='%s', apifuncname=='%s', lasterror==0x%x)",
                       callerfuncname, apifuncname, lasterror);
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

/* ********************************************************************** */

static int WINAPI
coin_GetTextFace(HDC hdc, /* handle to device context */
                 int nCount, /* length of buffer receiving typeface name */
                 LPTSTR lpFaceName) /* pointer to buffer receiving typeface name */
{
  int copied = GetTextFace(hdc, nCount, lpFaceName);

  if (copied == 0) {
    cc_string apicall;
    DWORD err = GetLastError();
    cc_string_construct(&apicall);
    cc_string_sprintf(&apicall,
                      "GetTextFace(hdc==%p, nCount==%d, lpFaceName==%p)",
                      hdc, nCount, lpFaceName);
    cc_win32_print_error("coin_GetTextFace", cc_string_get_text(&apicall), err);
    assert(FALSE && "unexpected error");
    cc_string_clean(&apicall);
  }

  return copied;
}

/* ********************************************************************** */

static void WINAPI
coin_LocalFree(HLOCAL hMem) /* handle to local memory object */
{
  const HLOCAL ptr = LocalFree(hMem);
  if (ptr != NULL) {
    cc_win32_print_error("coin_LocalFree", "LocalFree()", GetLastError());
    assert(FALSE && "unexpected error");
  }
}

/* ********************************************************************** */

static HGDIOBJ WINAPI
coin_SelectObject(HDC hdc, HGDIOBJ hgdiobj)
{
  HGDIOBJ previous;
  DWORD d = GetObjectType(hgdiobj);
  if (d == 0) {
    cc_win32_print_error("coin_SelectObject", "GetObjectType()", GetLastError());
    assert(FALSE && "unhandled error");
  }

  previous = SelectObject(hdc, hgdiobj);
  if (((d == OBJ_REGION) && (previous == HGDI_ERROR)) ||
      ((d == OBJ_REGION) && (previous == NULL))) {
    cc_win32_print_error("coin_SelectObject", "SelectObject()", GetLastError());
    
    /* not sure about this one, suddenly start assert'ing on
       SelectObject() failures may be too much of a shock for
       existing code... but eventually, it should go in:     (mortene) */
    /* assert(FALSE && "unhandled error"); */
  }
  return previous;
}

/* ********************************************************************** */

static int WINAPI
coin_GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject)
{
  int ret = GetObject(hgdiobj, cbBuffer, lpvObject);
  if (ret == 0) {
    cc_win32_print_error("coin_GetObject", "GetObject()", GetLastError());
    assert(FALSE && "unhandled error");
  }
  return ret;
}

/* ********************************************************************** */

/* singleton access to the structure, so we can initialize it at first
   use */
const struct cc_win32_api *
cc_win32(void)
{
  static BOOL init = FALSE;
  static struct cc_win32_api instance;

  if (!init) {
    init = TRUE;

    /* set up all function pointers */
    instance.GetTextFace = coin_GetTextFace;
    instance.LocalFree = coin_LocalFree;
    instance.SelectObject = coin_SelectObject;
    instance.GetObject = coin_GetObject;
  }

  return &instance;
}

/* ********************************************************************** */

#endif /* HAVE_WIN32_API */
