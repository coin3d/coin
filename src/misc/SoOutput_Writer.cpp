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
#include <string.h>
#include <assert.h>

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
