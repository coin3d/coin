#ifndef COIN_SOINPUT_READER_H
#define COIN_SOINPUT_READER_H

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

#include <Inventor/SbString.h>
#include <stdio.h>

// *************************************************************************

class SoInput_Reader {
public:
  SoInput_Reader(void);
  virtual ~SoInput_Reader();

  // add a new enum for each reader that is implemented
  enum ReaderType {
    REGULAR_FILE,
    MEMBUFFER,
    GZFILE,
    BZ2FILE,
    GZMEMBUFFER
  };

  // must be overloaded to return type
  virtual ReaderType getType(void) const = 0;

  // must be overloaded to read data. Should return number of bytes
  // read or 0 if eof
  virtual size_t readBuffer(char * buf, const size_t readlen) = 0;

  // should be overloaded to return filename. Default method returns
  // an empty string.
  virtual const SbString & getFilename(void);

  // default method returns NULL. Should only be overloaded if the
  // reader uses FILE * to read data.
  virtual FILE * getFilePointer(void);

  static SoInput_Reader * createReader(FILE * fp, const SbString & fullname);

public:
  SbString dummyname;
};

class SoInput_FileReader : public SoInput_Reader {
public:
  SoInput_FileReader(const char * const filename, FILE * filepointer);
  virtual ~SoInput_FileReader();

  virtual ReaderType getType(void) const;
  virtual size_t readBuffer(char * buf, const size_t readlen);

  virtual const SbString & getFilename(void);
  virtual FILE * getFilePointer(void);

public:
  SbString filename;
  FILE * fp;

};

class SoInput_MemBufferReader : public SoInput_Reader {
public:
  SoInput_MemBufferReader(const void * bufPointer, size_t bufSize);
  virtual ~SoInput_MemBufferReader();

  virtual ReaderType getType(void) const;
  virtual size_t readBuffer(char * buf, const size_t readlen);

public:
  char * buf;
  size_t buflen;
  size_t bufpos;
};

class SoInput_GZMemBufferReader : public SoInput_Reader {
public:
  SoInput_GZMemBufferReader(const void * bufPointer, size_t bufSize);
  virtual ~SoInput_GZMemBufferReader();

  virtual ReaderType getType(void) const;
  virtual size_t readBuffer(char * buf, const size_t readlen);

public:
  void * gzmfile;
  const void * buf;
};


class SoInput_GZFileReader : public SoInput_Reader {
public:
  SoInput_GZFileReader(const char * const filename, void * fp);
  virtual ~SoInput_GZFileReader();

  virtual ReaderType getType(void) const;
  virtual size_t readBuffer(char * buf, const size_t readlen);

  virtual const SbString & getFilename(void);

public:
  void * gzfp;
  SbString filename;
};

class SoInput_BZ2FileReader : public SoInput_Reader {
public:
  SoInput_BZ2FileReader(const char * const filename, void * fp);
  virtual ~SoInput_BZ2FileReader();

  virtual ReaderType getType(void) const;
  virtual size_t readBuffer(char * buf, const size_t readlen);

  virtual const SbString & getFilename(void);

public:
  void * bzfp;
  SbString filename;
};

#endif // COIN_SOINPUT_READER_H
