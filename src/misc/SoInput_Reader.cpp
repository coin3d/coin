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
#include <Inventor/errors/SoDebugError.h>

#include <string.h>
#include <assert.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_UNISTD_H
#include <unistd.h> // dup()
#endif // HAVE_UNISTD_H

#ifdef HAVE_IO_H
#include <io.h> // Win32 dup()
#endif // HAVE_IO_H

#ifdef HAVE_ZLIB
#include "gzmemio.h"
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
  fflush(fp); // needed since we fetch the file descriptor later

#ifdef HAVE_BZIP2
  if (header[0] == 'B' && header[1] == 'Z') {
    int bzerror = BZ_OK;
    BZFILE * bzfp = BZ2_bzReadOpen(&bzerror,  fp, 0, 0, NULL, 0);
    if ((bzerror == BZ_OK) && (bzfp != NULL)) {
      reader = new SoInput_BZ2FileReader(fullname.getString(), bzfp);
    }
  }
#endif // HAVE_BZIP2
#ifdef HAVE_ZLIB
  if ((reader == NULL) && (header[0] == 0x1f) && (header[1] == 0x8b)) {
    int fd = fileno(fp);
    // need to use dup() if we didn't open the file since gzdclose
    // will close it
    if (fd >= 0 && fullname.getLength() && fullname != "<stdin>") fd = dup(fd);
    if (fd >= 0) {
      gzFile gzfp = gzdopen(fd, "rb");
      if (gzfp) {
        reader = new SoInput_GZFileReader(fullname.getString(), gzfp);
      }
    }
    else {
      SoDebugError::postWarning("SoInput_Reader::createReader",
                                "Unable to create file descriptor from stream.");
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
// gzip readers
//
#ifdef HAVE_ZLIB

//
// gzipped membuffer class
//

SoInput_GZMemBufferReader::SoInput_GZMemBufferReader(void * bufPointer, size_t bufSize)
{
  this->gzmfile = gzm_open((uint8_t *)bufPointer, bufSize);
}

SoInput_GZMemBufferReader::~SoInput_GZMemBufferReader()
{
  gzm_close(this->gzmfile);
}

SoInput_Reader::ReaderType
SoInput_GZMemBufferReader::getType(void) const
{
  return GZMEMBUFFER;
}

int
SoInput_GZMemBufferReader::readBuffer(char * buf, const size_t readlen)
{
  return gzm_read(this->gzmfile, buf, readlen);
}

//
// gzFile class
//

SoInput_GZFileReader::SoInput_GZFileReader(const char * const filename, gzFile fp)
{
  this->gzfp = fp;
  this->filename = filename;
}

SoInput_GZFileReader::~SoInput_GZFileReader()
{
  assert(this->gzfp);
  gzclose(this->gzfp);
}

SoInput_Reader::ReaderType
SoInput_GZFileReader::getType(void) const
{
  return GZFILE;
}

int
SoInput_GZFileReader::readBuffer(char * buf, const size_t readlen)
{
  return gzread(this->gzfp, (void*) buf, readlen);
}

const SbString &
SoInput_GZFileReader::getFilename(void)
{
  return this->filename;
}

#endif // HAVE_ZLIB


//
// bzFile class
//
#ifdef HAVE_BZIP2

SoInput_BZ2FileReader::SoInput_BZ2FileReader(const char * const filename, BZFILE * fp)
{
  this->bzfp = fp;
  this->filename = filename;
}

SoInput_BZ2FileReader::~SoInput_BZ2FileReader()
{
  if (this->bzfp) {
    int bzerror = BZ_OK;
    BZ2_bzReadClose(&bzerror, this->bzfp);
  }
}

SoInput_Reader::ReaderType
SoInput_BZ2FileReader::getType(void) const
{
  return BZ2FILE;
}

int
SoInput_BZ2FileReader::readBuffer(char * buf, const size_t readlen)
{
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
}

const SbString &
SoInput_BZ2FileReader::getFilename(void)
{
  return this->filename;
}

#endif // HAVE_BZIP2
