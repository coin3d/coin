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

#include "SoInput_FileInfo.h"
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/misc/SoProto.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif // HAVE_ZLIB

#ifdef HAVE_BZIP2
#include <bzlib.h>
#endif // HAVE_BZIP2

const unsigned int READBUFSIZE = 65536;

SoInput_FileInfo::SoInput_FileInfo(SoInput_Reader * reader)
{
  this->reader = reader;
  this->readbuf = new char[READBUFSIZE];
  this->readbuflen = 0;
  this->readbufidx = 0;

  this->header = NULL;
  this->headerisread = FALSE;
  this->ivversion = 0.0f;
  this->linenr = 1;
  this->totalread = 0;
  this->lastputback = -1;
  this->lastchar = -1;
  this->eof = FALSE;
  this->isbinary = FALSE;
  this->vrml1file = FALSE;
  this->vrml2file = FALSE;
  this->prefunc = NULL;
  this->postfunc = NULL;
}

SoInput_FileInfo::~SoInput_FileInfo()
{
  delete[] this->readbuf;
  delete this->reader;
}

SbBool
SoInput_FileInfo::doBufferRead(void)
{
  // Make sure that we really do need to read more bytes.
  assert(this->backbuffer.getLength() == 0);
  assert(this->readbufidx == this->readbuflen);
  assert(this->reader);

  int len = this->reader->readBuffer(this->readbuf, READBUFSIZE);
  if (len <= 0) {
    this->readbufidx = 0;
    this->readbuflen = 0;
    this->eof = TRUE;
#if 0 // debug
    SoDebugError::postInfo("doBufferRead", "met Mr End-of-file");
#endif // debug
    return FALSE;
  }

  this->totalread += this->readbufidx;
  this->readbufidx = 0;
  this->readbuflen = len;
  return TRUE;
}

size_t
SoInput_FileInfo::getNumBytesParsedSoFar(void) const
{
  return this->totalread + this->readbufidx - this->backbuffer.getLength();
}

SbBool
SoInput_FileInfo::getChunkOfBytes(unsigned char * ptr, size_t length)
{
  // Suck out any bytes from the backbuffer first.
  while ((this->backbuffer.getLength() > 0) && (length > 0)) {
    *ptr++ = this->backbuffer.pop();
    length--;
  }

  do {
    // Grab bytes from the buffer.
    while ((this->readbufidx < this->readbuflen) && (length > 0)) {
      *ptr++ = this->readbuf[this->readbufidx++];
      length--;
    }

    // Fetch more bytes if necessary.
    if ((length > 0) && !this->eof) this->doBufferRead();

  } while (length && !this->eof);

  return !this->eof;
}

SbBool
SoInput_FileInfo::get(char & c)
{
  if (this->backbuffer.getLength() > 0) {
    c = this->backbuffer.pop();
  }
  else if (this->readbufidx >= this->readbuflen) {
    // doBufferRead() also does the right thing (i.e. sets the EOF
    // flag for the stream) if we're reading from memory.
    if (!this->doBufferRead()) {
      c = (char) EOF;
      return FALSE;
    }

    c = this->readbuf[this->readbufidx++];
  }
  else {
    c = this->readbuf[this->readbufidx++];
  }

  // NB: the line counting is not working 100% if we start putting
  // back and re-reading '\r\n' sequences.
  if ((c == '\r') || ((c == '\n') && (this->lastchar != '\r')))
    this->linenr++;
  this->lastchar = c;
  this->lastputback = -1;

  return TRUE;
}

void
SoInput_FileInfo::putBack(const char c)
{
  // Decrease line count if we put back an end-of-line character.
  // This should take care of Unix-, MSDOS/MSWin- and MacOS-style
  // generated files. NB: the line counting is not working 100% if
  // we start putting back and re-reading multiple parts of '\r\n'
  // sequences.
  if (!this->isbinary && ((c == '\r') || (c == '\n'))) this->linenr--;

  this->lastputback = (int)c;
  this->lastchar = -1;

  if (this->readbufidx > 0 && this->backbuffer.getLength() == 0) {
    this->readbufidx--;
    // Make sure we write back the same character which was read..
    assert(c == this->readbuf[this->readbufidx]);
  }
  else {
    this->backbuffer.push(c);
  }

  this->eof = FALSE;
}

void
SoInput_FileInfo::putBack(const char * const str)
{
  assert(!this->isbinary);

  int n = strlen(str);
  if (!n) return;

  // Decrease line count if we put back any end-of-line
  // characters. This should take care of Unix-, MSDOS/MSWin- and
  // MacOS-style generated files. What a mess.
  for (int i=0; i < n; i++) {
    if ((str[i] == '\r') || ((str[i] == '\n') &&
                             (this->lastputback != (int)'\r')))
      this->linenr--;
    this->lastputback = (int)str[i];
  }

  this->lastchar = -1;

  if (n <= this->readbufidx && this->backbuffer.getLength() == 0) {
    this->readbufidx -= n;
#if COIN_DEBUG
    for (int i = 0; i < n; i++) {
      assert(this->readbuf[this->readbufidx+i] == str[i]);
    }
#endif // COIN_DEBUG

  }
  else
    for (int i = n - 1; i >= 0; i--) this->backbuffer.push(str[i]);

  this->eof = FALSE;
}

SbBool
SoInput_FileInfo::skipWhiteSpace(void)
{
  const char COMMENT_CHAR = '#';

  while (TRUE) {
    char c;
    SbBool gotchar;
    while ((gotchar = this->get(c)) && this->isSpace(c));

    if (!gotchar) return FALSE;

    if (c == COMMENT_CHAR) {
      while ((gotchar = this->get(c)) && (c != '\n') && (c != '\r'));
      if (!gotchar) return FALSE;
      if (c == '\r') {
        gotchar = this->get(c);
        if (!gotchar) return FALSE;
        if (c != '\n') this->putBack(c);
      }
    }
    else {
      this->putBack(c);
      break;
    }
  }
  return TRUE;
}

// Returns TRUE if an attempt at reading the file header went
// without hitting EOF. Check this->ivversion != 0.0f to see if the
// header parse actually succeeded.

// The SoInput parameter is used in the precallback
SbBool
SoInput_FileInfo::readHeader(SoInput * soinput)
{
  if (this->headerisread) return this->eof ? FALSE : TRUE;
  this->headerisread = TRUE;

  this->header = "";
  this->ivversion = 0.0f;
  this->vrml1file = FALSE;
  this->vrml2file = FALSE;

  char c;
  if (!this->get(c)) return FALSE;

  if (c != '#') {
    this->putBack(c);
    return TRUE;
  }

  this->header += c;

  while (this->get(c) && (c != '\n') && (c != '\r')) this->header += c;
  if (this->eof) return FALSE;

  if (!SoDB::getHeaderData(this->header, this->isbinary, this->ivversion,
                           this->prefunc, this->postfunc, this->userdata,
                           TRUE)) {
    this->ivversion = 0.0f;
  }
  else {
    SbString vrml1string("#VRML V1.0 ascii");
    SbString vrml2string("#VRML V2.0 utf8");

    if (strncmp(vrml1string.getString(), this->header.getString(),
                vrml1string.getLength()) == 0) {
      this->vrml1file = TRUE;
    }
    else if (strncmp(vrml2string.getString(), this->header.getString(),
                     vrml2string.getLength()) == 0) {
      this->vrml2file = TRUE;
    }
    if (this->prefunc) this->prefunc(this->userdata, soinput);
  }
  return TRUE;
}

void
SoInput_FileInfo::connectRoutes(SoInput * in)
{
  const SbName * routeptr = this->routelist.getArrayPtr();
  const int n = this->routelist.getLength();
  for (int i = 0; i < n; i += 4) {
    SbName fromnodename = routeptr[i];
    SbName fromfieldname = routeptr[i+1];
    SbName tonodename = routeptr[i+2];
    SbName tofieldname = routeptr[i+3];

    SoNode * fromnode = SoNode::getByName(fromnodename);
    SoNode * tonode = SoNode::getByName(tonodename);

    if (!fromnode || !tonode) {
      SoReadError::post(in,
                        "Unable to create ROUTE from %s.%s to %s.%s. "
                        "Couldn't find both node references.",
                        fromnodename.getString(), fromfieldname.getString(),
                        tonodename.getString(), tofieldname.getString());
    }
    else {
      (void)SoBase::connectRoute(in, fromnodename, fromfieldname,
                                 tonodename, tofieldname);
    }
  }
}

// Unrefernce all protos
void
SoInput_FileInfo::unrefProtos(void)
{
  const int n = this->protolist.getLength();
  for (int i = 0; i < n; i++) {
    this->protolist[i]->unref();
  }
  this->protolist.truncate(0);
}

//
// tests fp and/or fullname, and opens the correct reader
//
SoInput_Reader * 
SoInput_FileInfo::getReader(FILE * fp, const SbString & fullname)
{
  SoInput_Reader * reader = NULL;
#ifdef HAVE_BZIP2
  int bzerror = BZ_OK;
  BZFILE * bzfp = BZ2_bzReadOpen(&bzerror,  fp, 0, 0, NULL, 0);
  if ((bzerror == BZ_OK) && (bzfp != NULL)) {
    reader = new SoInput_BZFileReader(fullname.getString(), (void*) bzfp);
  }
#endif // HAVE_BZIP2
#ifdef HAVE_ZLIB
  if (reader == NULL) {
    gzFile gzfp = gzopen(fullname.getString(), "rb");
    if (gzfp) {
      fclose(fp); // close original file handle
      reader = new SoInput_GZFileReader(fullname.getString(), (void*)gzfp);
    }
  }
#endif // HAVE_ZLIB
  if (reader == NULL) {
    reader = new SoInput_FileReader(fullname.getString(), fp);
  }
  return reader;
}
