/**************************************************************************\
 *
 *  This file is part of the SIM Scenery library.
 *  Copyright (C) 2000-2007 by Systems in Motion.  All rights reserved.
 *
 *  This software is proprietary to and embodies the confidential 
 *  technology of Systems in Motion.  Possession, use, or copying of this
 *  software and media is authorized only pursuant to a valid written
 *  license from Systems in Motion or an authorized sublicensor. 
 *
 *  For more information, contact SIM <http://www.sim.no/> by means of:
 *  Mail: Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, Norway;
 *  Email: <sales@sim.no>; Voice: +47 23 27 25 10; Fax: +47 23 27 25 11.
 *
\**************************************************************************/

#include <Inventor/C/basic.h>
#include <Inventor/C/XML/utils.h>
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
  char * buffer = (char *) malloc(bufsize + 1);
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
  fclose(fd);				// close opened file
  return buffer;
}

// *************************************************************************

char *
cc_xml_strndup(const char * string, int len)
{
  char * buf = (char *) malloc(len + 1);
  assert(buf != NULL);
  memcpy(buf, string, len);
  buf[len] = '\0';
  return buf;
}

char *
cc_xml_strdup(const char * string)
{
  return cc_xml_strndup(string, strlen(string));
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
