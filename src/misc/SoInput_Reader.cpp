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

#ifdef HAVE_BZIP2
#include <bzlib.h>
#endif // HAVE_BZIP2

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

// creates the correct reader based on the file type in fp (will
// examine the file header). If fullname is empty, it's assumed that
// file FILE pointer is passed from the user, and that we cannot
// necessarily find the file handle.
SoInput_Reader * 
SoInput_Reader::createReader(FILE * fp, const SbString & fullname)
{
  SoInput_Reader * reader = NULL;

  unsigned char header[4];
  long offset = ftell(fp);
  int siz = fread(header, 1, 4, fp);
  (void) fseek(fp, offset, SEEK_SET);

#ifdef HAVE_BZIP2
  if (header[0] == 'B' && header[1] == 'Z') {
    int bzerror = BZ_OK;
    BZFILE * bzfp = BZ2_bzReadOpen(&bzerror,  fp, 0, 0, NULL, 0);
    if ((bzerror == BZ_OK) && (bzfp != NULL)) {
      reader = new SoInput_BZFileReader(fullname.getString(), (void*) bzfp);
    }
  }
#endif // HAVE_BZIP2
#ifdef HAVE_ZLIB
  if (fullname.getLength() && (reader == NULL) && (header[0] == 0x1f) && (header[1] == 0x8b)) {
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
  assert(0 && "should never get here");
  return 0;
#endif // ! HAVE_ZLIB
}

const SbString &
SoInput_GZFileReader::getFilename(void)
{
  return this->filename;
}


//
// bzFile class
//

SoInput_BZFileReader::SoInput_BZFileReader(const char * const filename, void * fp)
{
  this->bzfp = fp;
  this->filename = filename;
}

SoInput_BZFileReader::~SoInput_BZFileReader()
{
#ifdef HAVE_BZIP2
  if (this->bzfp) {
    int bzerror = BZ_OK;
    BZ2_bzReadClose(&bzerror, this->bzfp);
  }
#endif // HAVE_BZIP2
}

SoInput_Reader::ReaderType
SoInput_BZFileReader::getType(void) const
{
  return BZ2FILE;
}

int
SoInput_BZFileReader::readBuffer(char * buf, const size_t readlen)
{
#ifdef HAVE_BZIP2
  if (this->bzfp == NULL) return -1;

  int bzerror = BZ_OK;
  int ret = BZ2_bzRead(&bzerror, this->bzfp,
                       buf, readlen);
  if ((bzerror != BZ_OK) && (bzerror != BZ_STREAM_END)) {
    ret = -1;
    BZ2_bzReadClose(&bzerror, this->bzfp);
    this->bzfp = NULL;
  }
  return ret;
#else // HAVE_BZIP2
  assert(0 && "should never get here");
  return 0;
#endif // ! HAVE_BZIP2
}

const SbString &
SoInput_BZFileReader::getFilename(void)
{
  return this->filename;
}
