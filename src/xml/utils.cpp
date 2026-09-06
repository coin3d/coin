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

#include "utils.h"

#include <Inventor/C/basic.h>
#include <Inventor/C/XML/types.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cctype>

char *
cc_xml_load_file(const char * path)
{
  FILE * fd = fopen(path, "rb");
  if ( !fd ) return NULL;
  fseek(fd, 0, SEEK_END);
  const long filesize = ftell(fd);
  if ( filesize < 0 ) {
    // ftell() failed (e.g. fd does not support seeking) -- without
    // this check, the negative value below would wrap around to a
    // huge size_t in the pos/bufsize comparisons and the fread() size
    // argument further down, both writing past a zero-byte buffer.
    fclose(fd);
    return NULL;
  }
  const size_t bufsize = static_cast<size_t>(filesize);
  fseek(fd, 0, SEEK_SET);
  char * buffer = new char [ bufsize + 1 ];
  size_t pos = 0, bytes;
  while ( pos != bufsize ) {
    bytes = fread(buffer + pos, 1, bufsize - pos, fd);
    if ( bytes == 0 ) {
      // fprintf(stderr, "fread() returned %d\n", bytes);
    } else {
      pos += bytes;
    }
  }
  buffer[bufsize] = '\0';
  fclose(fd); // close opened file
  return buffer;
}

// *************************************************************************

char *
cc_xml_strndup(const char * str, size_t len)
{
  char * buf = new char [ len + 1 ];
  assert(buf != NULL);
  memcpy(buf, str, len);
  buf[len] = '\0';
  return buf;
}

char *
cc_xml_strdup(const char * str)
{
  return cc_xml_strndup(str, strlen(str));
}

// *************************************************************************

/* since true/false is returned, stricmp() was an unfortunate name */
int
cc_xml_strieq(const char * s1, const char * s2)
{
  while ( *s1 && *s2 )
    if ( tolower(*s1++) != tolower(*s2++) ) return FALSE;
  if ( *s1 || *s2 ) return FALSE;
  return TRUE;
}

#if 0
int
sc_whitespace_p(const char * string)
{
  assert(string != NULL);
  while ( *string ) {
    switch ( *string ) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      break;
    default:
      return FALSE;
    }
    string++;
  }
  return TRUE;
}
#endif

#ifdef COIN_TEST_SUITE

#include <cstring>
#include <Inventor/C/basic.h>

// utils.h is a private header not on the testsuite's include path
// (and this component's public declaration surface is this
// extern "C" prototype, unchanged since it's what utils.h itself
// declares), so declare it directly rather than trying to reach the
// header from here. COIN_DLL_API is required here on Windows: this
// generated test file is compiled into CoinTests.exe, a separate
// binary from Coin.dll, so the symbol must be declared dllimport to
// resolve at link time (it must also be declared COIN_DLL_API at its
// real declaration in utils.h so the DLL build exports it).
extern "C" COIN_DLL_API char * cc_xml_load_file(const char * path);

// Regression test for cc_xml_load_file()'s handling of the size
// returned by ftell(): a call on an ordinary, seekable file should
// still round-trip the file's exact contents. (The failure mode this
// guards against -- ftell() returning -1 on a non-seekable stream,
// like a pipe -- needs a POSIX FIFO to reproduce and is covered
// separately by testsuite/reproducers/xml-load-file-ftell-error/,
// since fork()/mkfifo() aren't available on all platforms this
// test-suite runs on.)
BOOST_AUTO_TEST_CASE(load_file_roundtrip)
{
  const char * path = "coin_test_cc_xml_load_file_tmp.txt";
  const char content[] = "some file content\nwith more than one line\n";

  FILE * f = fopen(path, "wb");
  BOOST_REQUIRE_MESSAGE(f != NULL, "could not create temporary test file");
  fwrite(content, 1, sizeof(content) - 1, f);
  fclose(f);

  char * result = cc_xml_load_file(path);
  remove(path);

  BOOST_REQUIRE_MESSAGE(result != NULL, "cc_xml_load_file() unexpectedly returned NULL");
  BOOST_CHECK_MESSAGE(strcmp(result, content) == 0,
                      "cc_xml_load_file() did not return the exact file contents");
  delete [] result;
}

#endif // COIN_TEST_SUITE
