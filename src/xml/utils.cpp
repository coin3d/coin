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

#include "utils.h"

#include <Inventor/C/basic.h>
#include <Inventor/C/XML/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

char *
cc_xml_load_file(const char * path)
{
  FILE * fd = fopen(path, "rb");
  if ( !fd ) return NULL;
  fseek(fd, 0, SEEK_END);
  const long bufsize = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  char * buffer = new char [ bufsize + 1 ];
  long pos = 0, bytes;
  while ( pos != bufsize ) {
    bytes = fread(buffer + pos, 1, bufsize - pos, fd);
    if ( bytes < 0 ) {
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
cc_xml_strndup(const char * str, int len)
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
