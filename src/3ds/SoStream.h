#ifndef COIN_SOSTREAM_H
#define COIN_SOSTREAM_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/system/inttypes.h>
#include <stdio.h>

typedef void* SoStreamReallocCB(void *ptr, size_t newSize);

class SoInput;


class SoStream {
public:

  // Warning: Do not use chars together with << and >> operators, because some probably
  // old compilers (like gcc 2.96) treats probably internaly chars as int32_t instead of
  // (u)int8_t (sizeof still returns 1). This causes char is written to the stream as
  // four bytes instead of one. Also do not try to append overloaded << and >> char
  // operators, because it makes it imposible to compile the code on some compilers.

  inline SoStream& operator >> (int8_t &value)       { readInt8(value); return *this; }
  inline SoStream& operator << (const int8_t value)  { writeInt8(value); return *this; }
  inline SoStream& operator >> (uint8_t &value)      { readUInt8(value); return *this; }
  inline SoStream& operator << (const uint8_t value) { writeUInt8(value); return *this; }

  inline SoStream& operator >> (int16_t &value)       { readInt16(value); return *this; }
  inline SoStream& operator << (const int16_t value)  { writeInt16(value); return *this; }
  inline SoStream& operator >> (uint16_t &value)      { readUInt16(value); return *this; }
  inline SoStream& operator << (const uint16_t value) { writeUInt16(value); return *this; }

  inline SoStream& operator >> (int32_t &value)       { readInt32(value); return *this; }
  inline SoStream& operator << (const int32_t value)  { writeInt32(value); return *this; }
  inline SoStream& operator >> (uint32_t &value)      { readUInt32(value); return *this; }
  inline SoStream& operator << (const uint32_t value) { writeUInt32(value); return *this; }

  inline SoStream& operator >> (float &value)      { readFloat(value); return *this; }
  inline SoStream& operator << (const float value) { writeFloat(value); return *this; }

  inline SoStream& operator << (const double value) { writeDouble(value); return *this; }
  inline SoStream& operator >> (double &value)      { readDouble(value); return *this; }

  inline SoStream& operator >> (SoStream &stream)             { readStream(stream); return *this; }
  inline SoStream& operator << (const SoStream &stream) { writeStream(stream); return *this; }

// sbstring
// sbname
// arrays...

  inline bool readChar(char &value)  { return readUInt8(*((uint8_t*)&value)); }
  inline bool writeChar(const char value)  { return writeUInt8(value); }
  inline bool readSChar(signed char &value)  { return readInt8(*((int8_t*)&value)); }
  inline bool writeSChar(const signed char value)  { return writeInt8(value); }
  inline bool readUChar(unsigned char &value)  { return readUInt8(*((uint8_t*)&value)); }
  inline bool writeUChar(const unsigned char value)  { return writeUInt8(value); }

  virtual bool readInt8(int8_t &value);
  virtual bool writeInt8(const int8_t value);
  virtual bool readUInt8(uint8_t &value);
  virtual bool writeUInt8(const uint8_t value);

  virtual bool readInt16(int16_t &value);
  virtual bool writeInt16(const int16_t value);
  virtual bool readUInt16(uint16_t &value);
  virtual bool writeUInt16(const uint16_t value);

  virtual bool readInt32(int32_t &value);
  virtual bool writeInt32(const int32_t value);
  virtual bool readUInt32(uint32_t &value);
  virtual bool writeUInt32(const uint32_t value);

  virtual bool readFloat(float &value);
  virtual bool writeFloat(const float value);

  virtual bool readDouble(double &value);
  virtual bool writeDouble(const double value);

  virtual bool readZString(char *buf, int bufSize);
  virtual bool writeZString(const char *const buf);

  virtual size_t readBuffer(void *buf, size_t bufSize);
  virtual size_t writeBuffer(void *buf, size_t bufSize);

  virtual bool readFromStream(SoStream &stream);
  virtual bool writeToStream(SoStream &stream);
  virtual size_t readFromStream(SoStream &stream, size_t bytes);
  virtual size_t writeToStream(SoStream &stream, size_t bytes);

  virtual bool readStream(SoStream &stream);
  virtual bool writeStream(const SoStream &stream);


  virtual void setPos(size_t pos);
  virtual size_t getPos() const;
  virtual size_t getSize() const;


  enum StreamType { CLOSED, MEMORY, FILE_STREAM, SO_INPUT_WRAP };
  enum StreamEndianOrdering { BIG_ENDIAN_STREAM, LITTLE_ENDIAN_STREAM, HOST_DEP };

  SoStream();
  SoStream(size_t streamSize);
  SoStream(FILE *fp);
  SoStream(const char *const fileName);
  virtual ~SoStream();

  virtual StreamType getStreamType() const;
  virtual void closeStream();
  virtual void wrapSoInput(SoInput *input);

  virtual void setFilePointer(FILE *newFP);
  virtual FILE* getFilePointer() const;
  virtual bool openFile(const char *const fileName);
  virtual void closeFile();

  virtual void setBuffer(void *buf, size_t size);
  virtual bool getBuffer(void *&buf, size_t &size) const;
  virtual size_t getBufferSize() const;
  virtual void resetBuffer();
  virtual void emptyBuffer(size_t streamSize = 0);

  virtual void loadBufferFromFile(FILE *fp);
  virtual void loadBufferFromFile(const char *const fileName);
  virtual void storeBufferToFile(FILE *fp);
  virtual void storeBufferToFile(const char *const fileName);

  virtual void setBinary(const bool flag);
  virtual bool isBinary() const;
  virtual void setAccessRights(bool readEnabled, bool writeEnabled);
  virtual bool isReadable() const;
  virtual bool isWriteable() const;
  virtual void setEndianOrdering(const StreamEndianOrdering value);
  virtual StreamEndianOrdering getEndianOrdering() const;
  static StreamEndianOrdering getHostEndianOrdering();

  virtual void setBadBit();
  virtual void clearBadBit();
  virtual bool isBad() const;

protected:
  virtual size_t readBinaryArray(void *buf, size_t bufSize);
  virtual size_t writeBinaryArray(void *buf, size_t bufSize);
  virtual bool getChar(char &c);
  virtual void ungetChar(const char c);
  virtual void putChar(const char c);

  virtual bool reallocBuffer(size_t newSize);

  bool readDigInt(char *s, const char *e);
  bool readHexInt(char *s, const char *e);
private:
  StreamType streamType;
  bool binaryStream;
  bool badBit;
  union {
    FILE *filep;
    char *buffer;
    SoInput *soinput;
  };
  size_t bufferSize;
  size_t bufferAllocSize;
  size_t bufPos;
  bool readable, writeable;
  StreamEndianOrdering endianOrdering;
  bool needEndianConversion;
  SoStreamReallocCB *reallocCallback;

  void commonInit();
  void updateNeedEndianConversion();
};


#endif /* !COIN_SOSTREAM_H */
