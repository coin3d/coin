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

#include "SoOutput_Writer.h"
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

//
// abstract interface class
//

SoOutput_Writer::SoOutput_Writer(void)
{
}

SoOutput_Writer::~SoOutput_Writer()
{
}

FILE * 
SoOutput_Writer::getFilePointer(void)
{
  return NULL;
}


//
// standard stdio FILE writer
//

SoOutput_FileWriter::SoOutput_FileWriter(FILE * fp, const SbBool shouldclose)
{
  this->fp = fp;
  this->shouldclose = shouldclose;
}

SoOutput_FileWriter::~SoOutput_FileWriter()
{
  if (this->shouldclose) {
    assert(this->fp);
    fclose(this->fp);
  }
}


SoOutput_Writer::WriterType
SoOutput_FileWriter::getType(void) const
{
  return REGULAR_FILE;
}

size_t
SoOutput_FileWriter::write(const char * buf, size_t numbytes, const SbBool binary)
{
  assert(this->fp);
  return fwrite(buf, 1, numbytes, this->fp);
}

FILE * 
SoOutput_FileWriter::getFilePointer(void)
{
  return this->fp;
}

size_t 
SoOutput_FileWriter::bytesInBuf(void)
{
  return ftell(this->fp);
}


//
// membuffer writer
//

SoOutput_MemBufferWriter::SoOutput_MemBufferWriter(void * buffer,
                                                   const size_t len,
                                                   SoOutputReallocCB * reallocFunc,
                                                   int32_t offset)
{
  this->buf = (char*) buffer;
  this->bufsize = len;
  this->reallocfunc = reallocFunc;
  this->startoffset = this->offset = offset;
}

SoOutput_MemBufferWriter::~SoOutput_MemBufferWriter()
{
}

SoOutput_Writer::WriterType
SoOutput_MemBufferWriter::getType(void) const
{
  return MEMBUFFER;
}

size_t
SoOutput_MemBufferWriter::write(const char * constc, size_t length, const SbBool binary)
{
  // Needs a \0 at the end if we're writing in ASCII.
  int writelen = binary ? length : length + 1;

  if (this->makeRoomInBuf(writelen)) {
    char * writeptr = this->buf + this->offset;
    (void)memcpy(writeptr, constc, length);
    writeptr += length;
    this->offset += length;
    if (!binary) *writeptr = '\0'; // Terminate.
    return length;
  }
  return 0;
}

size_t 
SoOutput_MemBufferWriter::bytesInBuf(void)
{
  return this->offset;
}

SbBool
SoOutput_MemBufferWriter::makeRoomInBuf(size_t bytes)
{
  if ((this->offset + bytes) > this->bufsize) {
    if (this->reallocfunc) {
      this->bufsize = SbMax(this->offset + bytes, 2 * this->bufsize);
      this->buf = (char*) this->reallocfunc(this->buf, this->bufsize);
      if (this->buf) return TRUE;
    }
    return FALSE;
  }
  return TRUE;
}


#ifdef HAVE_ZLIB
//
// zlib writer
//

SoOutput_GZFileWriter::SoOutput_GZFileWriter(FILE * fp, const SbBool shouldclose)
{
  this->gzfp = NULL;

  int fd = fileno(fp);
  if (fd >= 0 && !shouldclose) fd = dup(fd);

  if (fd >= 0) {
    this->gzfp = gzdopen(fd, "wb");
    if (!this->gzfp) {
      SoDebugError::postWarning("SoOutput_GZFileWriter::SoOutput_GZFileWriter", 
                                "Unable to open file for writing.");    
    }
  }
  else {
    SoDebugError::postWarning("SoOutput_GZFileWriter::SoOutput_GZFileWriter", 
                              "Unable to create file descriptor from stream.");
    
  }
}

SoOutput_GZFileWriter::~SoOutput_GZFileWriter()
{
  if (this->gzfp) {
    gzclose(this->gzfp);
  }
}


SoOutput_Writer::WriterType
SoOutput_GZFileWriter::getType(void) const
{
  return GZFILE;
}

size_t
SoOutput_GZFileWriter::write(const char * buf, size_t numbytes, const SbBool binary)
{
  if (this->gzfp) {
    return gzwrite(this->gzfp, (void*)buf, numbytes);
  }
  return 0;
}

size_t 
SoOutput_GZFileWriter::bytesInBuf(void)
{
  if (this->gzfp) {
    return gztell(this->gzfp);
  }
  return 0;
}

#endif // HAVE_ZLIB


#ifdef HAVE_BZIP2
//
// bzip2 writer
//

SoOutput_BZ2FileWriter::SoOutput_BZ2FileWriter(FILE * fp, const SbBool shouldclose)
{
  this->fp = shouldclose ? fp : NULL;
  this->writecounter = 0;

  int bzerror = BZ_OK;
  // FIXME: add compression level parameter. Don't just use 5.
  // pederb, 2003-05-09
  this->bzfp = BZ2_bzWriteOpen(&bzerror, fp, 5, 0, 0);
  if (this->bzfp && (bzerror != BZ_OK)) {
    SoDebugError::postWarning("SoOutput_BZ2FileWriter::SoOutput_BZF2ileWriter", 
                              "Unable to open file for writing.");    
    BZ2_bzWriteClose(&bzerror, this->bzfp, 0, NULL, NULL);
    this->bzfp = NULL;
  }
}

SoOutput_BZ2FileWriter::~SoOutput_BZ2FileWriter()
{
  if (this->bzfp) {
    int bzerror = BZ_OK;
    BZ2_bzWriteClose(&bzerror, this->bzfp, 0, NULL, NULL);
    if (bzerror != BZ_OK) {
      SoDebugError::postWarning("SoOutput_BZ2FileWriter::~SoOutput_BZ2FileWriter", 
                                "Error when closing bzip2 file.");    
      
    }
  }
  if (this->fp) fclose(fp);
}


SoOutput_Writer::WriterType
SoOutput_BZ2FileWriter::getType(void) const
{
  return BZ2FILE;
}

size_t
SoOutput_BZ2FileWriter::write(const char * buf, size_t numbytes, const SbBool binary)
{
  if (this->bzfp) {
    int bzerror = BZ_OK;
    BZ2_bzWrite(&bzerror, this->bzfp, (void*) buf, numbytes);
    
    if (bzerror != BZ_OK) {
      assert(bzerror == BZ_IO_ERROR);
      SoDebugError::postWarning("SoOutput_BZ2FileWriter::write", 
                                "I/O error while writing.");    
      BZ2_bzWriteClose(&bzerror, this->bzfp, 0, NULL, NULL);
      this->bzfp = NULL;
      return 0;
    }
    this->writecounter += numbytes;
    return numbytes;
  }
  return 0;
}

size_t 
SoOutput_BZ2FileWriter::bytesInBuf(void)
{
  return this->writecounter;
}

#endif // HAVE_BZIP2

