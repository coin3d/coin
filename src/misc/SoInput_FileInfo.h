#ifndef COIN_SOINPUT_FILEINFO_H
#define COIN_SOINPUT_FILEINFO_H

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

#include <Inventor/SoInput.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbName.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include "SoInput_Reader.h"

class SoInput_FileInfo {
public:
  SoInput_FileInfo(SoInput_Reader * reader);
  ~SoInput_FileInfo();

  SbBool doBufferRead(void);
  size_t getNumBytesParsedSoFar(void) const;

  SbBool getChunkOfBytes(unsigned char * ptr, size_t length);
  SbBool get(char & c);

  void putBack(const char c);
  void putBack(const char * const str);

  SbBool skipWhiteSpace(void);

  // Returns TRUE if an attempt at reading the file header went
  // without hitting EOF. Check this->ivversion != 0.0f to see if the
  // header parse actually succeeded.
  SbBool readHeader(SoInput * input);
  SbBool isMemBuffer(void) {
    // if reader == NULL, it means that we're reading from stdin
    if (this->reader == NULL) return FALSE;
    return 
      (this->getReader()->getType() == SoInput_Reader::MEMBUFFER) ||
      (this->getReader()->getType() == SoInput_Reader::GZMEMBUFFER);
  }
  
  SbBool isBinary(void) {
    return this->isbinary;
  }
  float ivVersion(void) {
    return this->ivversion;
  }
  SbBool isFileVRML1(void) {
    return this->vrml1file;
  }
  SbBool isFileVRML2(void) {
    return this->vrml2file;
  }
  void setIvVersion(const float v) {
      this->ivversion = v;
  }
  const SbString & ivHeader(void) {
    return this->header;
  }
  unsigned int lineNr(void) {
    return this->linenr;
  }
  FILE * ivFilePointer(void) {
    // if reader == NULL, it means that we're reading from stdin
    if (this->reader == NULL) return coin_get_stdin();
    return this->getReader()->getFilePointer();
  }
  const SbString & ivFilename(void) {
    // if reader == NULL, it means that we're reading from stdin
    if (this->reader == NULL) return this->stdinname;
    return this->getReader()->getFilename();
  }
  SbBool isEndOfFile(void) {
    return this->eof;
  }
  void applyPostCallback(SoInput * soinput) {
    if (this->postfunc) this->postfunc(this->userdata, soinput);
  }

  void addRoute(const SbName & fromnode, const SbName & fromfield,
                const SbName & tonode, const SbName & tofield) {
    this->routelist.append(fromnode);
    this->routelist.append(fromfield);
    this->routelist.append(tonode);
    this->routelist.append(tofield);
  }

  void addProto(SoProto * proto) {
    this->protolist.append(proto);
  }

  void pushProto(SoProto * proto) {
    this->protostack.push(proto);
  }
  void popProto(void) {
    this->protostack.pop();
  }
  SoProto * getCurrentProto(void) {
    const int n = this->protostack.getLength();
    if (n) return this->protostack[n-1];
    return NULL;
  }

  SbBool isSpace(const char c) {
    // For vrml97, comma is treated as whitespace. Added this function
    // and converted isspace() calls to calls to this function.
    // 2001-10-24, pederb.
    //
    // From the VRML97 specification:
    //   The carriage return (0x0d), linefeed (0x0a), space (0x20), tab
    //   (0x09), and comma (0x2c) characters are whitespace characters
    //   wherever they appear outside of quoted SFString or MFString
    //   fields. Any number of whitespace characters and comments may be
    //   used to separate the syntactic entities of a VRML file.
    //
    //
    /////
    //
    // ANSI C isspace() takes the current locale into account. Under
    // MSWindows, this can lead to "interesting" artifacts, like a
    // case with RR tracked down and fixed by <thammer@sim.no> where a
    // character (was it ü?) with ASCII value > 127 made isspace()
    // return non-nil on a German system. So we're using our own
    // locale-independent isspace() implementation instead.
    return coin_isspace(c) || (this->vrml2file && c == ',');
  }

  void connectRoutes(class SoInput * in);
  void unrefProtos(void);

private:

  SoInput_Reader * getReader(void); 
  SoInput_Reader * reader;

  unsigned int linenr;

  // Data about the file's header.
  SbString header;
  float ivversion;
  SoDBHeaderCB * prefunc, * postfunc;
  void * userdata;
  SbBool isbinary;

  char * readbuf;
  int readbufidx;
  int readbuflen;
  size_t totalread;
  SbList<char> backbuffer; // Used as a stack (SbList provides push() and pop()).
  int lastputback; // The last character put back into the stream.
  int lastchar; // Last read character.
  SbBool headerisread, eof;
  SbBool vrml1file;
  SbBool vrml2file;

  SbList <SbName> routelist;
  SbList <SoProto*> protolist;
  SbList <SoProto*> protostack;

  SbString stdinname; // needed for ivFilename()
};

#endif // COIN_SOINPUT_FILEINFO_H
