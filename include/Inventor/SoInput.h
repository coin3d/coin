#ifndef COIN_SOINPUT_H
#define COIN_SOINPUT_H

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
#include <Inventor/SbBasic.h>
#include <Inventor/SbDict.h>
#include <Inventor/lists/SbList.h>
#include <stdio.h> // FILE
#ifndef COIN_INTERNAL
 // For Open Inventor compatibility.
 #include <Inventor/SoDB.h>
#endif // COIN_INTERNAL

class SoBase;
class SbString;
class SbTime;
class SbName;
class SbStringList;
class SoInput_FileInfo;
class SoProto;
class SoField;
class SoFieldContainer;

class COIN_DLL_API SoInput {
public:
  SoInput(void);
  SoInput(SoInput * dictIn);

  void addProto(SoProto * proto);

  void pushProto(SoProto * proto);
  SoProto * getCurrentProto(void) const;
  void popProto(void);

  void addRoute(const SbName & fromnode, const SbName & fromfield,
                const SbName & tonode, const SbName & tofield);
  SbBool checkISReference(SoFieldContainer * container, const SbName & fieldname, 
                          SbBool & readok);
  
  virtual ~SoInput(void);

  virtual void setFilePointer(FILE * newFP);
  virtual SbBool openFile(const char * fileName, SbBool okIfNotFound = FALSE);
  virtual SbBool pushFile(const char * fileName);
  virtual void closeFile(void);
  virtual SbBool isValidFile(void);
  virtual SbBool isValidBuffer(void);
  virtual FILE * getCurFile(void) const;
  virtual const char * getCurFileName(void) const;
  virtual void setBuffer(void * bufpointer, size_t bufsize);
  virtual size_t getNumBytesRead(void) const;
  virtual SbString getHeader(void);
  virtual float getIVVersion(void);
  virtual SbBool isBinary(void);

  virtual SbBool get(char & c);
  virtual SbBool getASCIIBuffer(char & c);
  virtual SbBool getASCIIFile(char & c);
  virtual SbBool readHex(uint32_t & l);
  virtual SbBool read(char & c);
  virtual SbBool read(char & c, SbBool skip);
  virtual SbBool read(SbString & s);
  virtual SbBool read(SbName & n, SbBool validIdent = FALSE);
  virtual SbBool read(int & i);
  virtual SbBool read(unsigned int & i);
  virtual SbBool read(short & s);
  virtual SbBool read(unsigned short & s);
  virtual SbBool read(float & f);
  virtual SbBool read(double & d);
  virtual SbBool readBinaryArray(unsigned char * c, int length);
  virtual SbBool readBinaryArray(int32_t * l, int length);
  virtual SbBool readBinaryArray(float * f, int length);
  virtual SbBool readBinaryArray(double * d, int length);
  virtual SbBool eof(void) const;

  SbBool isFileVRML1(void);
  SbBool isFileVRML2(void);
  virtual void resetFilePointer(FILE * fptr);

  virtual void getLocationString(SbString & string) const;
  virtual void putBack(const char c);
  virtual void putBack(const char * str);
  virtual void addReference(const SbName & name, SoBase * base,
                            SbBool addToGlobalDict = TRUE);
  virtual void removeReference(const SbName & name);
  virtual SoBase * findReference(const SbName & name) const;

  static void addDirectoryFirst(const char * dirName);
  static void addDirectoryLast(const char * dirName);
  static void addEnvDirectoriesFirst(const char * envVarName,
                                     const char * separator = ":\t ");
  static void addEnvDirectoriesLast(const char * envVarName,
                                    const char * separator = ":\t ");
  static void removeDirectory(const char * dirName);
  static void clearDirectories(void);
  static const SbStringList & getDirectories(void);

  static void init(void);

  static SbString getPathname(const char * const filename);
  static SbString getPathname(const SbString & s);
  static SbString getBasename(const char * const filename);
  static SbString getBasename(const SbString & s);

  static SbString searchForFile(const SbString & basename,
                                const SbStringList & directories,
                                const SbStringList & subdirectories);


protected:
  virtual SbBool popFile(void);
  void setIVVersion(float version);
  FILE * findFile(const char * fileName, SbString & fullName);
  void initFile(FILE * newFP, const char * fileName, SbString * fullName,
                SbBool openedHere, SbDict * refDict = (SbDict *) NULL);
  SbBool checkHeader(SbBool bValidateBufferHeader = FALSE);
  SbBool fromBuffer(void) const;
  SbBool skipWhiteSpace(void);
  size_t freeBytesInBuf(void) const;
  SbBool readInteger(int32_t & l);
  SbBool readUnsignedInteger(uint32_t & l);
  SbBool readReal(double & d);
  SbBool readUnsignedIntegerString(char * str);
  int readDigits(char * str);
  int readHexDigits(char * str);
  int readChar(char * str, char charToRead);

  SbBool makeRoomInBuf(size_t nBytes);
  void convertShort(char * from, short * s);
  void convertInt32(char * from, int32_t * l);
  void convertFloat(char * from, float * f);
  void convertDouble(char * from, double * d);
  void convertShortArray(char * from, short * to, int len);
  void convertInt32Array(char * from, int32_t * to, int len);
  void convertFloatArray(char * from, float * to, int len);
  void convertDoubleArray(char * from, double * to, int len);
  SbBool isFileURL(const char * url);
  char * URLToFile(char * out_buf, const char * in_buf);
  SbBool IsURL(const char * c_strng);

  static void setDirectories(SbStringList * dirs);

private:
  friend class SoDB;

  static void clean(void);
  void constructorsCommon(void);

  static void addDirectoryIdx(const int idx, const char * dirName);
  static void addEnvDirectoriesIdx(int startidx, const char * envVarName,
                                   const char * separator);
  static SbStringList * dirsearchlist;

  SbList<SoInput_FileInfo *> filestack;
  SoInput_FileInfo * getTopOfStack(void) const;

  SbDict refdict;
};

#endif // !COIN_SOINPUT_H
