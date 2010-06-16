#ifndef COIN_SOOUTPUT_WRITER_H
#define COIN_SOOUTPUT_WRITER_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

// *************************************************************************

#include <Inventor/SoOutput.h>
#include <stdio.h>

// *************************************************************************

class SoOutput_Writer {
public:
  SoOutput_Writer(void);
  virtual ~SoOutput_Writer();

  // add more enums as more writers are added
  enum WriterType {
    REGULAR_FILE,
    MEMBUFFER,
    GZFILE,
    BZ2FILE
  };

  // default method returns NULL. Should return the FILE pointer if
  // the Writer uses stdio to write.
  virtual FILE * getFilePointer(void);

  // must be overloaded to return the number of bites written so far
  virtual size_t bytesInBuf(void) = 0;

  // must be overloaded to return the writer type
  virtual WriterType getType(void) const = 0 ;

  // must be overloaded to write numbytes bytes to buf. Should
  // return the number of bytes actually written.
  virtual size_t write(const char * buf, size_t numbytes, const SbBool binary) = 0;

  static SoOutput_Writer * createWriter(FILE * fp,
                                        const SbBool shouldclose,
                                        const SbName & compmethod,
                                        const float level);

};

// class for stdio writing
class SoOutput_FileWriter : public SoOutput_Writer {
public:
  SoOutput_FileWriter(FILE * fp, const SbBool shouldclose);
  virtual ~SoOutput_FileWriter();

  virtual size_t bytesInBuf(void);
  virtual WriterType getType(void) const;
  virtual size_t write(const char * buf, size_t numbytes, const SbBool binary);
  virtual FILE * getFilePointer(void);

public:
  FILE * fp;
  SbBool shouldclose;
};

// class for membuffer writing
class SoOutput_MemBufferWriter : public SoOutput_Writer {
public:
  SoOutput_MemBufferWriter(void * buffer,
                           const size_t len,
                           SoOutputReallocCB * reallocFunc,
                           size_t offset);
  virtual ~SoOutput_MemBufferWriter();

  virtual size_t bytesInBuf(void);
  virtual WriterType getType(void) const;
  virtual size_t write(const char * buf, size_t numbytes, const SbBool binary);

public:

  SbBool makeRoomInBuf(size_t bytes);

  char * buf;
  size_t bufsize;
  SoOutputReallocCB * reallocfunc;
  size_t offset;
  size_t startoffset;
};

// class for zlib writing
class SoOutput_GZFileWriter : public SoOutput_Writer {
public:
  SoOutput_GZFileWriter(FILE * fp, const SbBool shouldclose, const float level);
  virtual ~SoOutput_GZFileWriter();

  virtual size_t bytesInBuf(void);
  virtual WriterType getType(void) const;
  virtual size_t write(const char * buf, size_t numbytes, const SbBool binary);

public:
  void * gzfp;
};

class SoOutput_BZ2FileWriter : public SoOutput_Writer {
public:
  SoOutput_BZ2FileWriter(FILE * fp, const SbBool shouldclose, const float level);
  virtual ~SoOutput_BZ2FileWriter();

  virtual size_t bytesInBuf(void);
  virtual WriterType getType(void) const;
  virtual size_t write(const char * buf, size_t numbytes, const SbBool binary);

public:
  void * bzfp;
  FILE * fp;
  size_t writecounter;
};

#endif // COIN_SOOUTPUT_WRITER_H
