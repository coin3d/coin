#ifndef COIN_SOOUTPUT_H
#define COIN_SOOUTPUT_H

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

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <stdio.h>

class SbDict;
class SbName;
class SoBase;
class SoOutputP;
class SoProto;
class SoField;
class SoFieldContainer;

typedef void * SoOutputReallocCB(void * ptr, size_t newSize);

class COIN_DLL_API SoOutput {
public:
  enum Stage { COUNT_REFS, WRITE };
  // Bitwise flags for the annotations.
  enum Annotations { ADDRESSES = 0x01, REF_COUNTS = 0x02 };

  SoOutput(void);
  SoOutput(SoOutput * dictOut);
  virtual ~SoOutput();

  virtual void setFilePointer(FILE * newFP);
  virtual FILE * getFilePointer(void) const;
  virtual SbBool openFile(const char * const fileName);
  virtual void closeFile(void);

  virtual void setBuffer(void * bufPointer, size_t initSize,
                         SoOutputReallocCB * reallocFunc, int32_t offset = 0);
  virtual SbBool getBuffer(void * & bufPointer, size_t & nBytes) const;
  virtual size_t getBufferSize(void) const;
  virtual void resetBuffer(void);
  virtual void setBinary(const SbBool flag);
  virtual SbBool isBinary(void) const;
  virtual void setHeaderString(const SbString & str);
  virtual void resetHeaderString(void);
  virtual void setFloatPrecision(const int precision);

  void setStage(Stage stage);
  Stage getStage(void) const;

  void incrementIndent(const int levels = 1);
  void decrementIndent(const int levels = 1);

  virtual void write(const char c);
  virtual void write(const char * s);
  virtual void write(const SbString & s);
  virtual void write(const SbName & n);
  virtual void write(const int i);
  virtual void write(const unsigned int i);
  virtual void write(const short s);
  virtual void write(const unsigned short s);
  virtual void write(const float f);
  virtual void write(const double d);
  virtual void writeBinaryArray(const unsigned char * c, const int length);
  virtual void writeBinaryArray(const int32_t * const l, const int length);
  virtual void writeBinaryArray(const float * const f, const int length);
  virtual void writeBinaryArray(const double * const d, const int length);

  virtual void indent(void);
  virtual void reset(void);
  void setCompact(SbBool flag);
  SbBool isCompact(void) const;
  void setAnnotation(uint32_t bits);
  uint32_t getAnnotation(void);

  static SbString getDefaultASCIIHeader(void);
  static SbString getDefaultBinaryHeader(void);

  int addReference(const SoBase * base);
  int findReference(const SoBase * base) const;
  void setReference(const SoBase * base, int refid);

  void addDEFNode(SbName name);
  SbBool lookupDEFNode(SbName name);
  void removeDEFNode(SbName name);

  void pushProto(SoProto * proto);
  SoProto * getCurrentProto(void) const;
  void popProto(void);
  
  void addRoute(SoFieldContainer * from, const SbName & fromfield,
                SoFieldContainer * to, const SbName & tofield);
  void resolveRoutes(void);
  
protected:
  SbBool isToBuffer(void) const;
  size_t bytesInBuf(void) const;
  SbBool makeRoomInBuf(size_t nBytes);
  void convertShort(short s, char * to);
  void convertInt32(int32_t l, char * to);
  void convertFloat(float f, char * to);
  void convertDouble(double d, char * to);
  void convertShortArray(short * from, char * to, int len);
  void convertInt32Array(int32_t * from, char * to, int len);
  void convertFloatArray(float * from, char * to, int len);
  void convertDoubleArray(double * from, char * to, int len);

  static SbString padHeader(const SbString & inString);

  SbBool wroteHeader;

private:
  SoOutputP * pimpl;

  void constructorCommon(void);

  void checkHeader(void);
  void writeBytesWithPadding(const char * const p, const size_t nr);
  
  friend class SoBase; // Need to be able to remove items from dict.
  void removeSoBase2IdRef(const SoBase * base);
};

#endif // !COIN_SOOUTPUT_H
