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

#include "SoInput_Reader.h"
#include <string.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif // HAVE_ZLIB

//
// abstract class
//

SoInput_Reader::SoInput_Reader(void)
  : dummyname("")
{
}

SoInput_Reader::~SoInput_Reader()
{
}

const SbString &
SoInput_Reader::getFilename(void)
{
  return this->dummyname;
}

FILE *
SoInput_Reader::getFilePointer(void)
{
  return NULL;
}


//
// standard FILE * class
//

SoInput_FileReader::SoInput_FileReader(const char * const filename, FILE * filepointer)
{
  this->fp = filepointer;
  this->filename = filename;
}

SoInput_FileReader::~SoInput_FileReader()
{
  // Close files which are not a memory buffer nor the stdin and
  // which we do have a filename for (if we don't have a filename,
  // the FILE ptr was just passed in through setFilePointer() and
  // is the library programmer's responsibility).
  if (this->fp &&
      (this->filename != "<stdin>") &&
      (this->filename.getLength())) {
    fclose(this->fp);
  }
}

SoInput_Reader::ReaderType
SoInput_FileReader::getType(void) const
{
  return REGULAR_FILE;
}

int
SoInput_FileReader::readBuffer(char * buf, const size_t readlen)
{
  return fread(buf, 1, readlen, this->fp);
}

const SbString & 
SoInput_FileReader::getFilename(void)
{
  return this->filename;
}

FILE * 
SoInput_FileReader::getFilePointer(void)
{
  return this->fp;
}

//
// standard membuffer class
//

SoInput_MemBufferReader::SoInput_MemBufferReader(void * bufPointer, size_t bufSize)
{
  this->buf = (char*) bufPointer;
  this->buflen = bufSize;
  this->bufpos = 0;
}

SoInput_MemBufferReader::~SoInput_MemBufferReader()
{
}

SoInput_Reader::ReaderType
SoInput_MemBufferReader::getType(void) const
{
  return MEMBUFFER;
}

int
SoInput_MemBufferReader::readBuffer(char * buf, const size_t readlen)
{
  size_t len = this->buflen - this->bufpos;
  if (len > readlen) len = readlen;

  memcpy(buf, this->buf + this->bufpos, len);
  this->bufpos += len;

  return len;
}

//
// gzFile class
//

SoInput_GZFileReader::SoInput_GZFileReader(const char * const filename, void * fp)
{
  this->gzfp = fp;
  this->filename = filename;
}

SoInput_GZFileReader::~SoInput_GZFileReader()
{
#ifdef HAVE_ZLIB
  assert((gzFile) this->gzfp);
  gzclose(this->gzfp);
#endif // HAVE_ZLIB
}

SoInput_Reader::ReaderType
SoInput_GZFileReader::getType(void) const
{
  return GZFILE;
}

int
SoInput_GZFileReader::readBuffer(char * buf, const size_t readlen)
{
#ifdef HAVE_ZLIB
  return gzread((gzFile)this->gzfp, (void*) buf, readlen);
#else // HAVE_ZLIB
  return 0;
#endif // ! HAVE_ZLIB
}

const SbString & 
SoInput_GZFileReader::getFilename(void)
{
  return this->filename;
}
