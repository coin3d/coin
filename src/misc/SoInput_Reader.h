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

#include <stdio.h>

class SoInput_Reader {
public:
  SoInput_Reader(void) { }
  virtual ~SoInput_Reader() { }

  enum ReaderType {
    REGULAR_FILE,
    MEMBUFFER
  };
  
  virtual ReaderType getType(void) const = 0;
  virtual int readBuffer(char * buf, const size_t readlen) = 0;
};

class SoInput_FileReader : public SoInput_Reader {
public:
  SoInput_FileReader(const char * const filename, FILE * filepointer) {
    this->fp = filepointer;
    this->filename = filename;
  }
  virtual ~SoInput_FileReader() {

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

  virtual ReaderType getType(void) const {
    return REGULAR_FILE;
  }
  virtual int readBuffer(char * buf, const size_t readlen) {
    return fread(buf, 1, readlen, this->fp);
  }

public:
  SbString filename;
  FILE * fp;

};

class SoInput_MemBufferReader : public SoInput_Reader {
public:
  SoInput_MemBufferReader(void * bufPointer, size_t bufSize) {
    this->buf = (char*) bufPointer;
    this->buflen = bufSize;
    this->bufpos = 0;
  }
  virtual ~SoInput_MemBufferReader() { }

  virtual ReaderType getType(void) const {
    return MEMBUFFER;
  }
  virtual int readBuffer(char * buf, const size_t readlen) {
    size_t len = this->buflen - this->bufpos;
    if (len > readlen) len = readlen;

    memcpy(buf, this->buf + this->bufpos, len);
    this->bufpos += len;
    
    return len;
  }

public:
  char * buf;
  size_t buflen;
  size_t bufpos;
};


#endif // COIN_SOINPUT_READER_H
