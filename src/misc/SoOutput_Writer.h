#ifndef COIN_SOOUTPUT_WRITER_H
#define COIN_SOOUTPUT_WRITER_H

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

#include <Inventor/SoOutput.h>
#include <stdio.h>

class SoOutput_Writer {
public:
  SoOutput_Writer(void);
  virtual ~SoOutput_Writer();
  
  enum WriterType {
    REGULAR_FILE,
    MEMBUFFER,
    GZFILE
  };

  virtual WriterType getType(void) const = 0 ;
  virtual int write(char * buf, int numbytes) = 0;
};

class SoOutput_FileWriter : public SoOutput_Writer {
public:
  SoOutput_FileWriter(FILE * fp, const SbBool shouldclose);
  virtual ~SoOutput_FileWriter();

  virtual WriterType getType(void) const;
  virtual int write(char * buf, int numbytes);

public:
  FILE * fp;
  SbBool shouldclose;
};

class SoOutput_MemBufferWriter : public SoOutput_Writer {
public:
  SoOutput_MemBufferWriter(void * buffer, 
                           const size_t len,
                           SoOutputReallocCB * reallocFunc, 
                           int32_t offset);
  virtual ~SoOutput_MemBufferWriter();

  virtual WriterType getType(void) const;
  virtual int write(char * buf, int numbytes);

public:
  char * buf;
  size_t buflen;
  SoOutputReallocCB * bufrealloc;
  int32_t offset;
};

#endif // COIN_SOOUTPUT_WRITER_H
