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
#include <assert.h>

SoOutput_Writer::SoOutput_Writer(void)
{
}

SoOutput_Writer::~SoOutput_Writer()
{
}





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

int 
SoOutput_FileWriter::write(char * buf, int numbytes)
{
  assert(this->fp);
  return fwrite(buf, 1, numbytes, this->fp);
}

SoOutput_MemBufferWriter::SoOutput_MemBufferWriter(void * buffer, 
                                                   const size_t len,
                                                   SoOutputReallocCB * reallocFunc, 
                                                   int32_t offset)
{
  this->buf = (char*) buffer;
  this->buflen = len;
  this->bufrealloc = reallocFunc;
  this->offset = offset;
}

SoOutput_MemBufferWriter::~SoOutput_MemBufferWriter()
{
}

SoOutput_Writer::WriterType 
SoOutput_MemBufferWriter::getType(void) const
{
  return MEMBUFFER;
}

int 
SoOutput_MemBufferWriter::write(char * buf, int numbytes)
{
  return 0;
}

