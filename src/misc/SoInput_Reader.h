#ifndef COIN_SOINPUT_READER_H
#define COIN_SOINPUT_READER_H

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

#include <Inventor/SbString.h>
#include <stdio.h>

class SoInput_Reader {
public:
  SoInput_Reader(void);
  virtual ~SoInput_Reader();

  enum ReaderType {
    REGULAR_FILE,
    MEMBUFFER,
    GZFILE
  };
  virtual ReaderType getType(void) const = 0;
  virtual int readBuffer(char * buf, const size_t readlen) = 0;

  virtual const SbString & getFilename(void);
  virtual FILE * getFilePointer(void);

public:
  SbString dummyname;
};

class SoInput_FileReader : public SoInput_Reader {
public:
  SoInput_FileReader(const char * const filename, FILE * filepointer);
  virtual ~SoInput_FileReader();

  virtual ReaderType getType(void) const;
  virtual int readBuffer(char * buf, const size_t readlen);

public:
  SbString filename;
  FILE * fp;

};

class SoInput_MemBufferReader : public SoInput_Reader {
public:
  SoInput_MemBufferReader(void * bufPointer, size_t bufSize);
  virtual ~SoInput_MemBufferReader();

  virtual ReaderType getType(void) const;
  virtual int readBuffer(char * buf, const size_t readlen);

public:
  char * buf;
  size_t buflen;
  size_t bufpos;
};

class SoInput_GZFileReader : public SoInput_Reader {
public:
  SoInput_GZFileReader(const char * const filename, void * fp);
  virtual ~SoInput_GZFileReader();

  virtual ReaderType getType(void) const;
  virtual int readBuffer(char * buf, const size_t readlen);

public:
  void * gzfp;
  SbString filename;
};

#endif // COIN_SOINPUT_READER_H
