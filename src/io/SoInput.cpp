/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoInput SoInput.h Inventor/SoInput.h
  \brief The SoInput class is an abstraction of file import functionality.
  \ingroup general

  This class takes care of most of the chores of doing data import in Coin.
  It puts a layer of abstraction over the read operations to make it
  transparent for the rest of the Coin code whether or not we're reading
  from a file, from a memory buffer or from stdin.

  SoInput also takes care of checking file validity, aswell as handling
  information about features in the various file formats which are
  supported.

  Example usage:
  \code
  int
  main(void)
  {
    SoDB::init();

    SoInput in;
    if (in.openFile("fab_model.iv")) {
      SoSeparator * root = SoDB::readAll(&in);
      if (root) {
        // Do stuff with the imported scene graph here
        // ...
        return 0;
      }
    }

    return 1;
  }
  \endcode


  Important note: there are several public and protected methods which
  makes it possible to pass in or get returned FILE* structures in
  this class. Do \e not use these methods when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.


  This class supports one environment variable called
  COIN_SOINPUT_SEARCH_GLOBAL_DICT.  When set to "1", the global
  dictionary is searched after the current file dictionary when
  resolving USE keywords. This makes it possible to reference nodes in
  other files. The reason for introducing this feature is that the
  SoFile node rereads the file whenever the name field changes.  The
  first time the file is read, it's possible to reference nodes in the
  parent file, using the USE keyword. But, when the file is reread
  this is no longer possible, since the reading now starts at the
  SoFile node, with an empty dictionary.

  \sa SoOutput, SoDB
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H
#ifdef HAVE_SYS_TYPES_H
/* According to Coin user Ralf Corsepius, at least SunOS4 needs to
   include sys/types.h before netinet/in.h. There have also been a
   problem report for FreeBSD which seems to indicate that the same
   dependency exists on that platform aswell. */
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H
#include <ctype.h>

#include <Inventor/C/glue/zlib.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/SbName.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/misc/SoProto.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/threads/SbStorage.h>
#include <coindefs.h> // COIN_STUB(), COIN_OBSOLETED()

#include "SoInput_FileInfo.h"

// This (POSIX-compliant) macro is missing from the Win32 API header
// files for MSVC++ 6.0.
#ifndef S_ISDIR
 // The _S_IFDIR bitpattern is not in the POSIX standard, but MSVC++
 // header files has it.
 #ifdef _S_IFDIR
 #define S_ISDIR(s) (s & _S_IFDIR)
 #else // Ai.
 #error Can neither find nor make an S_ISDIR macro to test stat structures.
 #endif // !_S_IFDIR
#endif // !S_ISDIR

// *************************************************************************

SbStringList * SoInput::dirsearchlist = NULL;

static SbStorage * soinput_tls = NULL;

struct soinput_tls_data {
  SbStringList * searchlist;
  int instancecount;
};

static void
soinput_construct_tls_data(void * closure)
{
  soinput_tls_data * data = (soinput_tls_data*) closure;
  data->searchlist = new SbStringList;
  data->instancecount = 0;
}

static void
soinput_destruct_tls_data(void * closure)
{
  soinput_tls_data * data = (soinput_tls_data*) closure;
  
  int n = data->searchlist->getLength();
  for (int i = 0; i < n; i++) {
    delete (*data->searchlist)[i];
  }
  delete data->searchlist;
}

// *************************************************************************

/*!
  Constructor. If no filepointer is set, input will be read from stdin.
 */
SoInput::SoInput(void)
{
  this->constructorsCommon();
}

/*!
  A constructor which copies the reference mappings from \a dictIn upon
  initialization.
 */
SoInput::SoInput(SoInput * dictIn)
{
  this->constructorsCommon();
  this->refdict = dictIn->refdict;
}

/*!
  \COININTERNAL
  Do common tasks for the constructors.
*/
void
SoInput::constructorsCommon(void)
{
  /* It is not possible to "pass" C library data from the application
     to a MSWin .DLL, so this is necessary to get hold of the stderr
     FILE*. Just using fprintf(stderr, ...) or fprintf(stdout, ...)
     directly might result in a crash when Coin has been compiled as a
     .DLL. */
  this->setFilePointer(coin_get_stdin());

  soinput_tls_data * data = (soinput_tls_data *)soinput_tls->get();
  if (data->instancecount == 0) {
    const SbStringList & dir = *SoInput::dirsearchlist;
    for (int i = 0; i < dir.getLength(); i++) {
      data->searchlist->append(new SbString(dir[i]->getString()));
    }
  }
  data->instancecount++;
}

/*!
  Destructor. Runs SoInput::closeFile() to close any open files.
*/
SoInput::~SoInput(void)
{
  this->closeFile();

  soinput_tls_data * data = (soinput_tls_data *)soinput_tls->get();
  data->instancecount--;
  if (data->instancecount == 0) {
    for (int i = 0; i < data->searchlist->getLength(); i++) {
      delete (*data->searchlist)[i];
    }
    data->searchlist->truncate(0);
  }
}

/*!
  Adds a ROUTE from /a fromnode's \fromfield, to \a tonode's
  tofield. This makes it possible to defines ROUTEs in files
  before the \a fromnode or \a tonode is parsed.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
void
SoInput::addRoute(const SbName & fromnode, const SbName & fromfield,
                  const SbName & tonode, const SbName & tofield)
{
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  if (info) {
    info->addRoute(fromnode, fromfield,
                   tonode, tofield);
  }
}

/*!
  Searches for PROTO named \a name. This function will only return
  PROTOs that have been read by the current SoInput instance.

  \since Coin 2.3
*/
SoProto * 
SoInput::findProto(const SbName & name)
{
  SoInput_FileInfo * info = this->getTopOfStack();
  if (info) {
    return info->findProto(name);
  }
  return NULL;
}

/*!
  Adds a Proto which should be active in the current scope.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
void
SoInput::addProto(SoProto * proto)
{
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  if (info) {
    proto->ref(); // the PROTO is unref'ed when the file is popped
    info->addProto(proto);
  }
}

/*!
  Pushed a Proto onto the Proto stack. The Proto stack is used during
  VRML2 file parsing.

  \COIN_FUNCTION_EXTENSION

  \sa popProto()
  \since Coin 2.0
*/
void
SoInput::pushProto(SoProto * proto)
{
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  if (info) {
    info->pushProto(proto);
  }
}

/*!
  Pops a Proto off the Proto stack.

  \COIN_FUNCTION_EXTENSION

  \sa pushProto()
  \since Coin 2.0
*/
void
SoInput::popProto(void)
{
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  if (info) {
    info->popProto();
  }
}

/*!
  Returns the Proto at the top of the Proto stack.

  \COIN_FUNCTION_EXTENSION

  \sa pushProto()
  \since Coin 2.0
*/
SoProto *
SoInput::getCurrentProto(void) const
{
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  if (info) {
    return info->getCurrentProto();
  }
  return NULL;
}

/*!
  Checks if the next bytes in \a in is the IS keyword. Returns \c TRUE
  if the IS keyword was found, \a readok will be set to \c FALSE if
  some error occured while searching for the IS keyword.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SbBool
SoInput::checkISReference(SoFieldContainer * container,
                          const SbName & fieldname, SbBool & readok)
{
  readok = TRUE;
  SoProto * proto = this->getCurrentProto();
  SbBool foundis = FALSE;
  if (proto) {
    // The reason for this specific parsing code is that we need
    // to put back whitespace when the IS keyword isn't found.
    // SoInput::read(SbName) skips whitespace automatically...
    // pederb, 2001-10-26
    SbString putback;
    const int STATE_WAIT_I = 0;
    const int STATE_EXPECT_S = 1;
    const int STATE_EXPECT_SPACE = 2;
    const int STATE_FOUND = 3;
    const int STATE_NOTFOUND = 4;
    int state = STATE_WAIT_I;
    do {
      char c;
      readok = this->read(c, FALSE);
      putback += c;
      if (readok) {
        switch (state) {
        case STATE_WAIT_I:
          if (c == 'I') state = STATE_EXPECT_S;
          else if (!coin_isspace(c)) state = STATE_NOTFOUND;
          break;
        case STATE_EXPECT_S:
          if (c == 'S') state = STATE_EXPECT_SPACE;
          else state = STATE_NOTFOUND;
          break;
        case STATE_EXPECT_SPACE:
          if (coin_isspace(c)) state = STATE_FOUND;
          else state = STATE_NOTFOUND;
          break;
        default:
          assert(0 && "should not happen");
          break;
        }
      }
    } while (readok && state != STATE_FOUND && state != STATE_NOTFOUND);

    if (state == STATE_FOUND) {
      foundis = TRUE;
      SbName iname;
      readok = this->read(iname, TRUE);
      if (readok) {
        assert(container->isOfType(SoNode::getClassTypeId()));
        proto->addISReference((SoNode*) container, fieldname, iname);
      }
    }
    else {
      assert(state == STATE_NOTFOUND);
      this->putBack(putback.getString());
      foundis = FALSE;
    }
  }
  return foundis;
}

/*!
  Uses the given file pointer as the input file. The input stack of
  files will be emptied first. Closing a file passed in by it's file
  pointer is the library user's responsibility.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.

  Note that it is not allowed to pass a FILE* to a gzip-compressed
  file under Mac OS X.

  \sa openFile(), closeFile()
*/
void
SoInput::setFilePointer(FILE * newFP)
{
  this->closeFile();

  const char * name = (newFP == coin_get_stdin()) ? "<stdin>" : "";
  SoInput_Reader * reader = NULL;

  // delay creating the reader if we're reading from
  // stdin. SoInput_FileInfo will create it when we know that we're
  // actually going to read from stdin
  if (newFP != coin_get_stdin()) {
    reader = SoInput_Reader::createReader(newFP, SbString(name));
  }
  SoInput_FileInfo * newfile = new SoInput_FileInfo(reader);
  if (newfile) this->filestack.insert(newfile, 0);
}

/*!
  Open the given file, and make it the only one in the file stack. If
  \a okIfNotFound is \c FALSE, show a warning message if the file could
  not be opened.

  Returns \c TRUE if file could be opened for reading, \c FALSE
  otherwise.

  Note: even if your attempt at opening a file is unsuccessful,
  the SoInput instance will \e not default to reading from stdin after
  a call has been made to this method.

  \sa setFilePointer(), pushFile(), closeFile()
*/
SbBool
SoInput::openFile(const char * fileName, SbBool okIfNotFound)
{
  this->closeFile();

  SbString fullname;
  FILE * fp = this->findFile(fileName, fullname);
  if (fp) {
    SoInput_Reader * reader = SoInput_Reader::createReader(fp, fullname);
    assert(reader);
    SoInput_FileInfo * newfile = new SoInput_FileInfo(reader);
    this->filestack.insert(newfile, 0);

    SoInput::addDirectoryFirst(SoInput::getPathname(fullname).getString());
    return TRUE;
  }

  if (!okIfNotFound) { SoReadError::post(this, fullname.getString()); }

  return FALSE;
}

/*!
  Push a new file on top of the stack. Further read operations will take
  place with the new file until it reaches end-of-file, upon when the
  file will be popped of the stack and we'll continue with the next file
  in the stack.

  Returns \c TRUE if the file could be opened for reading, \c FALSE
  otherwise.

  \sa openFile()
*/
SbBool
SoInput::pushFile(const char * filename)
{
  SbString fullname;
  FILE * fp = this->findFile(filename, fullname);
  if (fp) {
    SoInput_Reader * reader = SoInput_Reader::createReader(fp, fullname);
    SoInput_FileInfo * newfile = new SoInput_FileInfo(reader);
    this->filestack.insert(newfile, 0);

    SoInput::addDirectoryFirst(SoInput::getPathname(fullname).getString());
    return TRUE;
  }

  SoReadError::post(this, fullname.getString());

  return FALSE;
}

/*!
  Removes all files from the internal stack. Any files opened by
  openFile() or pushFile() will also be closed, but not the files
  coming from setFilePointer() -- which are the responsibility of the
  caller.

  You usually don't need to call this method explicitly, as it will
  automatically be triggered by the SoInput destructor to close any
  open files when the SoInput instance goes out of scope.

  \sa setFilePointer(), openFile(), pushFile()
*/
void
SoInput::closeFile(void)
{
  // Remove all entries, including the default <stdin>.
  while (this->filestack.getLength() > 0) {
    if (!this->fromBuffer() && (this->getCurFile() != coin_get_stdin())) {
      SbString s = SoInput::getPathname(this->getTopOfStack()->ivFilename());
      if (s.getLength()) SoInput::removeDirectory(s.getString());
    }
    delete this->getTopOfStack();
    this->filestack.remove(0);
  }
}

/*!
  Checks to see if the current file is a valid file which can be read
  by the Coin library.

  When reading files through e.g. SoDB::readAll(), this is
  automatically checked for you, so application code should usually
  not need to use this method.
*/
SbBool
SoInput::isValidFile(void)
{
  if (this->getTopOfStack() == NULL) return FALSE;

  // Abstract away the stupidity of providing both isValidFile() and
  // isValidBuffer().
  if (this->getTopOfStack()->isMemBuffer()) return this->isValidBuffer();

  float ver = this->getIVVersion();
  if (ver != 0.0f) return TRUE;
  return FALSE;
}

/*!
  Returns \c TRUE if the current text buffer represents a valid
  iv-file which can be read by the Coin library.

  When reading files through e.g. SoDB::readAll(), this is
  automatically checked for you, so application code should usually
  not need to use this method.
*/
SbBool
SoInput::isValidBuffer(void)
{
  if (this->getTopOfStack() == NULL) return FALSE;

  // Abstract away the stupidity of providing both isValidFile() and
  // isValidBuffer().
  if (!this->getTopOfStack()->isMemBuffer()) return this->isValidFile();

  // Set "validate header" argument to FALSE, as memory buffers should
  // be possible to read even with no header.
  return this->checkHeader(FALSE);
}

/*!
  Returns file pointer of the file on top of the input stack. If the
  "file" is actually a memory buffer, returns \c NULL.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.

  \sa getCurFileName()
*/
FILE *
SoInput::getCurFile(void) const
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  return fi->isMemBuffer() ? NULL : fi->ivFilePointer();
}

/*!
  Returns the name of the file on top of the input stack. \c NULL will
  be returned if the toplevel "file" is a memory buffer.

  \sa getCurFile()
*/
const char *
SoInput::getCurFileName(void) const
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  return fi->isMemBuffer() ? NULL : fi->ivFilename().getString();
}

/*!
  Sets up the input stream for reading from the strings pointed to by a
  NULL-terminated array of string pointers.  It is intended for reading
  memory-inlined scene graphs.

  The rationale for this function is that there is a compiler portability
  problem with compiling large, static string buffers.  Some compilers
  have fixed max-lengths on this feature, just over 1KB, which obviously
  becomes a problem real fast.  This method lets you specify the buffer
  as an array of smaller string buffers, typically one string for each
  line in the Inventor file.

  \code
  SoNode *
  makeSceneGraph(void) {
    static const char * inlinescenegraph[] = {
      "#Inventor V2.1 ascii",
      "",
      "Separator {",
      "  Cube {",
      "  }",
      "}",
      NULL
    };
    SoInput in;
    if ( !in.setStringArray(inlinescenegraph) ) return NULL;
    return SoDB::readAll(&in);
  }
  \endcode

  \sa setBuffer()

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.1
*/
void
SoInput::setStringArray(const char * strings[])
{
  size_t bufsize = 0;
  int i;
  for (i = bufsize = 0; strings[i] != NULL; i++ )
    bufsize += strlen(strings[i]);
  char * buf = new char [bufsize + 1];
  for (i = bufsize = 0; strings[i] != NULL; i++ ) {
    const int len = strlen(strings[i]);
    memcpy(buf+bufsize, strings[i], len);
    bufsize += len;
  }
  this->setBuffer(buf, bufsize);
  SoInput_FileInfo * info = this->getTopOfStack();
  assert(info);
  info->setDeleteBuffer(buf);
}

/*!
  Sets up the input stream for reading from a memory buffer. Closes all
  open files in the file stack first.

  The type of the first argument of this method would have been better
  designed as "const char *", but this his how the original SGI
  Inventor API is, and we duplicate the method signature for
  compatibility reasons.
*/
void
SoInput::setBuffer(void * bufpointer, size_t bufsize)
{
  // FIXME: shouldn't the first argument to this method rather have
  // been a "const char *"? 20010821 mortene.

  this->closeFile();
  SoInput_Reader * reader = NULL;

  unsigned char * header = (unsigned char*) bufpointer;
  if ((bufsize >= 2) && (header[0] == 0x1f) && (header[1] == 0x8b)) {
    if (cc_zlibglue_available()) {
      reader = new SoInput_GZMemBufferReader(bufpointer, bufsize);
    }
    else {
      SoDebugError::postWarning("SoInput::setBuffer",
                                "Buffer seems to be in gzip format, but zlib support is "
                                "not available.");
    }
  }
  if (reader == NULL) {
    reader = new SoInput_MemBufferReader(bufpointer, bufsize);
  }
  SoInput_FileInfo * newfile = new SoInput_FileInfo(reader);
  this->filestack.insert(newfile, 0);
}

/*!
  Returns number of bytes read so far from the current file or memory
  buffer.
  You can only use this method while you're reading the file.
  When the reading has finished, the current file is popped off the
  file stack, and a more or less random number is returned. You
  should use the stat() system call to find the size of a file (or
  fseek() and ftell()).
*/
size_t
SoInput::getNumBytesRead(void) const
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  return fi->getNumBytesParsedSoFar();
}

/*!
  Returns header of current file.
*/
SbString
SoInput::getHeader(void)
{
  (void)this->checkHeader();
  SoInput_FileInfo * fi = this->getTopOfStack();
  
  if (fi) {
    return fi->ivHeader();
  }
  else {
    return SbString("");
  }
}

/*!
  Returns the library version needed to read a file with the given header.
  If the header of the current file is not valid, this method returns
  \a 0.0f.
*/
float
SoInput::getIVVersion(void)
{
  if (!this->checkHeader()) return 0.0f;
  return this->getTopOfStack()->ivVersion();
}

/*!
  Returns \c TRUE if the current file is in binary format.
*/
SbBool
SoInput::isBinary(void)
{
  if (!this->checkHeader()) return FALSE;
  return this->getTopOfStack()->isBinary();
}

/*!
  Get next character in current stream. Returns \c FALSE on end of file.

  \sa read()
*/
SbBool
SoInput::get(char & c)
{
  return (this->checkHeader() && // Strip off file header, if any.
          this->getTopOfStack()->get(c));
}

/*!
  Returns next character in current stream. Don't use this method -- SoInput
  is supposed to \e hide whether or not you are reading from file or memory.
  It is only included for compatibility reasons.
*/
SbBool
SoInput::getASCIIBuffer(char & c)
{
  return this->get(c);
}

/*!
  Returns next character in current stream. Don't use this method -- SoInput
  is supposed to \e hide whether or not you are reading from file or memory.
  It is only included for compatibility reasons.
*/
SbBool
SoInput::getASCIIFile(char & c)
{
  return this->get(c);
}

/*!
  Reads an  unsigned integer in hexidecimal format from the current stream.
  Note that no error checking is done to see if it actually is a hex
  format value.

  A number in hexadecimal format must have the "0x" prefix.

  Returns \c FALSE if end of file is encountered.
*/
SbBool
SoInput::readHex(uint32_t & l)
{
  assert(!this->isBinary());

  // FIXME: this is a tremendously stupid function. Should obsolete it.

  // FIXME: no checking for array overwriting. Dangerous. 19990625 mortene.
  char buffer[1024];
  char * bufptr = buffer;

  if (this->readChar(bufptr, '0')) {
    if (this->readChar(bufptr + 1, 'x')) {
      bufptr += 2 + this->readHexDigits(bufptr + 2);
    }
  }

  *bufptr = '\0';
  sscanf(buffer, "%x", &l);
  return TRUE;
}

/*!
  Skips whitespace and reads next character in input stream.
  Returns \c FALSE if encountering end of file.
*/
SbBool
SoInput::read(char & c)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  return (this->checkHeader() && fi->skipWhiteSpace() && fi->get(c));
}

/*!
  Reads next character in input stream, returns \c FALSE if encountering
  end of file. If \a skip is \c TRUE, skips whitespace before reading a
  character.
*/
SbBool
SoInput::read(char & c, SbBool skip)
{
  if (!this->checkHeader()) return FALSE;

  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  SbBool ok = TRUE;
  if (skip) ok = fi->skipWhiteSpace();
  return (ok && fi->get(c));
}


/*!
  Skips whitespace and extracts string. A string can be placed
  within quotes, in which case we will return all characters between
  the quotes (including spaces, tabs, newlines, etc). A quote character
  (i.e. ``"'') can also be part of the string, but must then be preceded
  by a ``\'' character.

  A string not contained in quotes is terminated by the first following
  whitespace character.

  Returns \c FALSE upon encountering end of file before the string is
  fully parsed, or any other error.
*/
SbBool
SoInput::read(SbString & s)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);

  if (!this->checkHeader()) return FALSE;

  ////////////////////
  // Binary read
  ////////////////////

  if (this->isBinary()) {
    // This is just a guess at a sensible limit, to help detect
    // corrupted files and to avoid those leading to attempts at
    // allocating gigabytes of memory.
    const unsigned int MAXSTRLEN = 10 * 1024;

    unsigned int slen;
    if (!this->read(slen)) { return FALSE; }
    if (slen == 0) { s = ""; return TRUE; }

    // Sanity check
    if (slen > MAXSTRLEN) {
      SoReadError::post(this, "String too long (%u characters) -- "
                        "file probably corrupt.", slen);
      return FALSE;
    }

    char buffer[MAXSTRLEN+4+1];
    if (!fi->getChunkOfBytes((unsigned char *)buffer, ((slen+3)/4)*4)) { return FALSE; }
    buffer[slen] = '\0';
    s = buffer;
    return TRUE;
  }


  ////////////////////
  // ASCII read
  ////////////////////

  if (!fi->skipWhiteSpace()) return FALSE;

  s.makeEmpty();

  char c;
  if (!fi->get(c)) return FALSE;

  SbBool quoted = (c == '\"');
  if (!quoted) fi->putBack(c);

  // FIXME: the following code could have been much cleaner if we'd
  // used SbString::operator+=(char) for each char instead of going
  // through the tmp buffer. SbString::operator+=(char) uses strlen()
  // for each invocation, though, so that'd give us rather nasty
  // algorithm-time on the SoInput::read(SbString) operations. The
  // correct fix is to first avoid the strlen() invocations in
  // SbString, then simplify this code. 20020506 mortene.

  char bufStore[256];
  int bytesLeft;
  unsigned int totallen = 0;
  do {
    char * buf = bufStore;
    bytesLeft = sizeof(bufStore) - 1;

    while (bytesLeft > 0) {
      if (!fi->get(*buf)) {
        if ((totallen == 0) || quoted) {
          if (quoted) {
            SoReadError::post(this, "Missing terminating quote-character (\")");
          }
          return FALSE;
        }
        break;
      }

      if (quoted) {
        if (*buf == '\"') break;

        if (*buf == '\\') {
          if (!fi->get(c)) return FALSE;
          // for VRML97, \\ and \" should be converted to \ and ".
          // for VRML 1.0 and Inventor, only \" should be converted.
          if (c == '\"' || (c == '\\' && this->isFileVRML2())) {
            *buf = c;
          }
          else {
            *buf = '\\';
            fi->putBack(c);
          }
        }
      }
      else if (fi->isSpace(*buf)) {
        fi->putBack(*buf);
        break;
      }

#if 0
      // FIXME: This code was commented away because of incompatibility
      // with Open Inventor. If further testing shows that this code
      // is not needed, it can be removed.
      //
      // 20040618 jornskaa

      else if (*buf == '[') {
        fi->putBack(*buf);
        break;
      }
      else if (*buf == ']') {
        fi->putBack(*buf);
        break;
      }
#endif

      buf++; totallen++;
      bytesLeft--;
    }

    *buf = '\0';
    s += bufStore;

  } while (bytesLeft == 0);

  return TRUE;
}

// helper function to handle VRML97
static SbBool
soinput_is_ident_start_char(SoInput * in, char c)
{
  unsigned char uc = (unsigned char) c;
  if (in->isFileVRML2()) {
    if (uc <= 0x20) return FALSE;
    if (uc >= 0x30 && uc <= 0x39) return FALSE;
    switch (uc) {
    case 0x22:
    case 0x23:
    case 0x27:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x7b:
    case 0x7d:
    case 0x7f:
      return FALSE;
    }
    return TRUE;
  }
  else return SbName::isIdentStartChar(c);
}

// helper function to handle VRML97
static SbBool
soinput_is_ident_char(SoInput * in, char c)
{
  unsigned char uc = (unsigned char) c;
  if (in->isFileVRML2()) {
    if (uc <= 0x20) return FALSE;
    switch (uc) {
    case 0x22:
    case 0x23:
    case 0x27:
    case 0x2c:
    case 0x2e:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x7b:
    case 0x7d:
    case 0x7f:
      return FALSE;
    }
    return TRUE;
  }
  else return SbName::isIdentChar(c);
}


/*!
  Read a name from the current stream and place it in \a n.

  If \a validIdent is \c TRUE the name needs to be a valid identifier
  (no reserved characters etc), while \a validIdent equal to \c FALSE
  means we'll just read characters for the next word until we hit
  whitespace or one of the "{"/"}" delimiters.

  Returns \c FALSE on encountering end of file before a full name has
  been read -- if \a validIdent is also \c FALSE. If \a validIdent is
  passed as \c TRUE, the return value will be \c FALSE if no valid name
  was found, but \e not necessarily on end of file.

*/
SbBool
SoInput::read(SbName & n, SbBool validIdent)
{
  if (!this->checkHeader()) return FALSE;
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);

  // Binary format.
  if (this->isBinary()) {
    SbString s;
    if (!this->read(s)) return FALSE;

    n = s;

    if (validIdent && s.getLength() > 0) {
      if (!soinput_is_ident_start_char(this, s[0])) return FALSE;
      for (int i = 1; i < s.getLength(); i++)
        if (!soinput_is_ident_char(this, s[i])) return FALSE;
    }

    return TRUE;
  }
  // ASCII format.
  else {
    if (!fi->skipWhiteSpace()) return FALSE;

    if (!validIdent) {
      SbString s;
      if (!this->read(s)) return FALSE;

#if 0
      // FIXME: This code was commented away because of incompatibility
      // with Open Inventor. If further testing shows that this code
      // is not needed, it can be removed. To remove trailing brackets
      // from the end of a name seems strange since it is a valid 
      // character if validIdent==FALSE.
      //
      // 20040618 jornskaa
      
      // Strip off any "{" or "}" characters.
      const char * cstr = s.getString();
      int idx = s.getLength()-1;
      if (idx >= 0) {
        assert(cstr && strlen(cstr) == (size_t)s.getLength());
        while (((cstr[idx] == '{') || (cstr[idx] == '}')) && (idx > 0)) idx--;
      }
      
      if (idx == s.getLength()-1) {
        // No trailing '{' or '}'.
        n = s;
      }
      else {
        // Trailing brackets; rip out correct part of string and put the
        // rest back in again.
        n = s.getSubString(0, idx);
        fi->putBack(&cstr[idx+1]);
      }
#else
      // FIXME: if the code above is reinserted, this block should be 
      // removed/commented.
      //
      // 20040618 jornskaa
      n = s;
#endif

    }
    else {
      SbString s;
      char buf[256];
      char * b = buf;
      char c;
      SbBool gotchar;

      if ((gotchar = fi->get(c)) && soinput_is_ident_start_char(this, c)) {
        *b++ = c;

        while ((gotchar = fi->get(c)) && soinput_is_ident_char(this, c)) {
          *b++ = c;
          if (b - buf == 255) {
            *b = '\0';
            s += buf;
            b = buf;
          }
        }
      }
      // This behavior is pretty silly, but this is how it is supposed
      // to work, apparently -- _not_ returning FALSE upon end-of-file.
      if (gotchar) fi->putBack(c);
      *b = '\0';
      s += buf;
      n = SbName(s);
#if 0 // debug
      SoDebugError::postInfo("SoInput::read",
                             "string read: ``%s''", s.getString());
#endif // debug

#if 0 // OBSOLETED, 19991116 mortene.
      if (b == buf) return FALSE;
#else
      if (s.getLength() == 0) return FALSE;
#endif
    }
  }

  return TRUE;
}

#define READ_NUM(reader, readType, num, type) \
  SoInput_FileInfo * fi = this->getTopOfStack(); \
  assert(fi); \
  if (!fi->skipWhiteSpace()) return FALSE; \
  readType _tmp; \
  if (!fi->reader(_tmp)) return FALSE; \
  num = (type) _tmp;

#define READ_INTEGER(num, type) \
READ_NUM(readInteger, int32_t, num, type)

#define READ_UNSIGNED_INTEGER(num, type) \
READ_NUM(readUnsignedInteger, uint32_t, num, type)

#define READ_REAL(num, type) \
READ_NUM(readReal, double, num, type)

/*!
  Read integer from current file or buffer position and place it in \a i.
  Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(int & i)
{
  if (this->isBinary()) {
    int32_t tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    i = tmp;
    return TRUE;
  }
  else {
    READ_INTEGER(i, int);
  }
  return TRUE;
}

/*!
  Read unsigned integer from current file or buffer position and place
  it in \a i. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(unsigned int & i)
{
  if (this->isBinary()) {
    int32_t tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    i = tmp;
    return TRUE;
  }
  else {
    READ_UNSIGNED_INTEGER(i, unsigned int);
  }
  return TRUE;
}

/*!
  Read short integer from current file or buffer position and place
  it in \a s. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(short & s)
{
  if (this->isBinary()) {
    int32_t tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    s = (short) tmp;
    return TRUE;
  }
  else {
    READ_INTEGER(s, short);
  }
  return TRUE;
}

/*!
  Read unsigned short integer from current file or buffer position and place
  it in \a s. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(unsigned short & s)
{
  if (this->isBinary()) {
    int32_t tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    s = (unsigned short) tmp;
    return TRUE;
  }
  else {
    READ_UNSIGNED_INTEGER(s, unsigned short);
  }
  return TRUE;
}

/*!
  Read float value from current file or buffer position and place
  it in \a f. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(float & f)
{
  if (this->isBinary()) {
    if (!this->readBinaryArray(&f, 1)) { return FALSE; }
  }
  else {
    READ_REAL(f, float);
  }
  if (!coin_finite((double)f)) {
    SoReadError::post(this,
                      "Detected non-valid floating point number, replacing "
                      "with 0.0f");
    f = 0.0f;
    // We don't return FALSE, thereby allowing the read process to
    // continue, as a convenience for the application programmer.
  }
  return TRUE;
}

/*!
  Read double value from current file or buffer position and place
  it in \a d. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(double & d)
{
  if (this->isBinary()) {
    if (!this->readBinaryArray(&d, 1)) { return FALSE; }
  }
  else {
    READ_REAL(d, double);
  }
  if (!coin_finite(d)) {
    SoReadError::post(this,
                      "Detected non-valid floating point number, replacing "
                      "with 0.0");
    d = 0.0;
    // We don't return FALSE, thereby allowing the read process to
    // continue, as a convenience for the application programmer.
  }
  return TRUE;
}

/*!
  Reads \a length characters from the current stream into \a c. Returns
  \c FALSE if end of file is encountered before the given number of bytes
  could be read.
 */
SbBool
SoInput::readBinaryArray(unsigned char * c, int length)
{
  return (this->checkHeader() &&
          this->getTopOfStack()->getChunkOfBytes(c, length));
}

/*!
  Reads \a length 32-bit integers from the current stream into \a l. Returns
  \c FALSE if end of file is encountered before the given number of integers
  could be read.
 */
SbBool
SoInput::readBinaryArray(int32_t * l, int length)
{
  assert(length > 0);
  if (!this->checkHeader() ||
      !this->getTopOfStack()->getChunkOfBytes((unsigned char *)l,
                                              length * sizeof(int32_t)))
    return FALSE;

  this->convertInt32Array((char *)l, l, length);
  return TRUE;
}

/*!
  Reads \a length floats from the current stream into \a f. Returns
  \c FALSE if end of file is encountered before the given number of
  binary float values could be read.
 */
SbBool
SoInput::readBinaryArray(float * f, int length)
{
  assert(length > 0);
  if (!this->checkHeader() ||
      !this->getTopOfStack()->getChunkOfBytes((unsigned char *)f,
                                              length * sizeof(float)))
    return FALSE;

  this->convertFloatArray((char *)f, f, length);

  return TRUE;
}

/*!
  Reads \a length double floats from the current stream into \a d.
  Returns \c FALSE if end of file is encountered before the given number of
  binary double float values could be read.
 */
SbBool
SoInput::readBinaryArray(double * d, int length)
{
  assert(length > 0);
  if (!this->checkHeader() ||
      !this->getTopOfStack()->getChunkOfBytes((unsigned char *)d,
                                              length * sizeof(double)))
    return FALSE;

  this->convertDoubleArray((char *)d, d, length);
  return TRUE;
}

/*!
  Returns \c TRUE if we are at the end of the current stream, otherwise
  \c FALSE.
 */
SbBool
SoInput::eof(void) const
{
  if (!this->getTopOfStack()) return TRUE;
  return this->getTopOfStack()->isEndOfFile();
}

/*!
  Places the current line number location and file name in a string
  which is to be used in the output upon read errors.
 */
void
SoInput::getLocationString(SbString & str) const
{
  if (this->filestack.getLength() == 0) {
    str = "";
    return;
  }

  const char * filename = this->getCurFileName();

  // FIXME: hack to cast away constness. Ugly. 19990713 mortene.
  if (((SoInput *)this)->isBinary()) {
    str.sprintf("\tOccurred at position %lu in binary file %s",
                (unsigned long int) this->getTopOfStack()->getNumBytesParsedSoFar(),
                filename ? filename : "<memory>");
  }
  else {
    str.sprintf("\tOccurred at line %3d in %s",
                this->getTopOfStack()->lineNr(),
                filename ? filename : "<memory>");
  }
}

/*!
  Insert character \a c in stream at the current position.
 */
void
SoInput::putBack(const char c)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  fi->putBack(c);
}

/*!
  Insert string \a str in stream at the current position.
 */
void
SoInput::putBack(const char * str)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  fi->putBack(str);
}

/*!
  Adds a name<->SoBase mapping to our dictionary.

  \sa findReference(), removeReference()
 */
void
SoInput::addReference(const SbName & name, SoBase * base,
                      SbBool /* addToGlobalDict */) // FIXME: why the unused arg? 20001024 mortene.
{
  SoProto * proto = this->getCurrentProto();
  if (proto) {
    proto->addReference(name, base);
  }
  else {
    this->refdict.enter((unsigned long)name.getString(), (void *) base);
  }
}

/*!
  Removes a name<->SoBase mapping from our dictionary.

  \sa findReference(), addReference()
 */
void
SoInput::removeReference(const SbName & name)
{
  SoProto * proto = this->getCurrentProto();
  if (proto) {
    proto->removeReference(name);
  }
  else {
    this->refdict.remove((unsigned long)name.getString());
  }
}

/*!
  Finds an SoBase derived object which has been mapped to \a name earlier
  during the import process.

  The Coin library will by default only search through the previously
  loaded nodes from the \e same file. By setting the environment
  variable \c COIN_SOINPUT_SEARCH_GLOBAL_DICT to "1", you can force
  the import process to search for USE-references through \e all nodes
  that has been loaded or otherwise instantiated.

  \sa addReference(), removeReference()
 */
SoBase *
SoInput::findReference(const SbName & name) const
{
  SoProto * proto = this->getCurrentProto();
  if (proto) {
    return proto->findReference(name);
  }
  else {
    void * base;

    if (this->refdict.find((unsigned long)name.getString(), base))
      return (SoBase *) base;

    static int COIN_SOINPUT_SEARCH_GLOBAL_DICT = -1;
    if (COIN_SOINPUT_SEARCH_GLOBAL_DICT < 0) {
      const char * env = coin_getenv("COIN_SOINPUT_SEARCH_GLOBAL_DICT");
      if (env) COIN_SOINPUT_SEARCH_GLOBAL_DICT = atoi(env);
      else COIN_SOINPUT_SEARCH_GLOBAL_DICT = 0;
    }

    if (COIN_SOINPUT_SEARCH_GLOBAL_DICT) {
      return SoBase::getNamedBase(name, SoNode::getClassTypeId());
    }
  }
  return NULL;
}

/*!
  Add a directory to the search list. The added directory will be searched
  before any of the other directories in the list.

  \sa addDirectoryLast(), addEnvDirectoriesFirst(), addEnvDirectoriesLast()
  \sa removeDirectory(), clearDirectories()
 */
void
SoInput::addDirectoryFirst(const char * dirName)
{
  SoInput::addDirectoryIdx(0, dirName);
}

/*!
  Add a directory to the search list. The added directory will be searched
  after any of the other directories in the list.

  \sa addDirectoryFirst(), addEnvDirectoriesFirst(), addEnvDirectoriesLast()
  \sa removeDirectory(), clearDirectories()
 */
void
SoInput::addDirectoryLast(const char * dirName)
{
  SoInput::addDirectoryIdx(-1, dirName);
}

/*!
  \COININTERNAL

  Add a directory to the search list at the specified index value. An index
  of -1 means append.
 */
void
SoInput::addDirectoryIdx(const int idx, const char * dirName)
{
  assert(idx > -2);
  SbStringList * dirs = SoInput::dirsearchlist;

  if (soinput_tls) {
    soinput_tls_data * data = (soinput_tls_data *)soinput_tls->get();
    if (data->instancecount) { dirs = data->searchlist; }
  }

  assert(idx <= dirs->getLength());
  // NB: note that it _should_ be possible to append/insert the same
  // directory name multiple times, as this is an easy way of
  // "stacking" names when doing recursive SoDB::readAll() calls or
  // using SoInput::pushFile(). So don't try to "fix" or change this
  // aspect of adding entries to the directory search list. --mortene

  SbString * ns = new SbString(dirName);
  if (idx == -1) dirs->append(ns);
  else if (idx != -1) dirs->insert(ns, idx);
}

/*!
  Add in front of the list all directories in the environment
  variable \a envVarName. The \a separator string should contain all
  characters to be interpreted as separators in the environment variable
  string list. The default separators are ':', tab and space.

  \sa addDirectoryFirst(), addDirectoryLast()
  \sa addEnvDirectoriesLast()
  \sa removeDirectory(), clearDirectories()
 */
void
SoInput::addEnvDirectoriesFirst(const char * envVarName,
                                const char * separator)
{
  SoInput::addEnvDirectoriesIdx(0, envVarName, separator);
}

/*!
  Add at the end of the list all directories in the environment
  variable \a envVarName. The \a separator string should contain all
  characters to be interpreted as separators in the environment variable
  string list. The default separators are ':', tab and space.

  \sa addDirectoryFirst(), addDirectoryLast()
  \sa addEnvDirectoriesFirst()
  \sa removeDirectory(), clearDirectories()
 */
void
SoInput::addEnvDirectoriesLast(const char * envVarName,
                               const char * separator)
{
  SoInput::addEnvDirectoriesIdx(SoInput::dirsearchlist->getLength(),
                                envVarName,
                                separator);
}

/*!
  \COININTERNAL

  Add environment variable directory list, starting at index \a startidx.
 */
void
SoInput::addEnvDirectoriesIdx(int startidx,
                              const char * envVarName,
                              const char * separator)
{
  const char * p = coin_getenv(envVarName);
  if (!p) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoInput::addEnvDirectoriesFirst",
                              "Tried to add directories from nonexistent "
                              "environment variable '%s'.",
                              envVarName);
#endif // COIN_DEBUG
    return;
  }

  // FIXME: the following "tokenizer" code could perhaps be factored
  // out and moved to e.g. SbStringList? 20030820 mortene.

  if (separator == NULL) {
    // use default separators if NULL
    separator = ":\t ";
  }
  const char * end = p + strlen(p);

  while (p < end) {
    const char * hit = strpbrk(p, separator);

    if (hit && hit != p) {
      SbString add = SbString(p).getSubString(0, hit-p-1);
      SoInput::addDirectoryIdx(startidx++, add.getString());
      p = hit+1;
    }
    else if (hit) { // more than one separator in a row
      p++;
    }
    else { // last entry
      SoInput::addDirectoryIdx(startidx, p);
      p = end;
    }
  }
}

/*!
  Remove the given directory from the directory search list.

  The search list is scanned from last to first for the directory name,
  so directories can easily be "stacked" by pushing with addDirectoryLast()
  and then removed with this method.

  \sa addDirectoryFirst(), addDirectoryLast()
  \sa addEnvDirectoriesFirst(), addEnvDirectoriesLast()
  \sa clearDirectories()
 */
void
SoInput::removeDirectory(const char * dirName)
{
  SbStringList * dirs = SoInput::dirsearchlist;

  if (soinput_tls) {
    soinput_tls_data * data = (soinput_tls_data *)soinput_tls->get();
    if (data->instancecount) { dirs = data->searchlist; }
  }

  // dirsearchlist might be null if user called SoDB::cleanup()
  if (dirs) {
    int idx = dirs->getLength() - 1;
    for (; idx >= 0; idx--) {
      if (*((*dirs)[idx]) == dirName) break;
    }
    
    if (idx >=0) {
      delete (*dirs)[idx]; // Dealloc SbString object
      dirs->remove(idx);
    }
#if COIN_DEBUG
    else {
      SoDebugError::postWarning("SoInput::removeDirectory",
                                "Tried to remove nonexistent directory '%s'"
                                " in directory search list.", dirName);
    }
#endif // COIN_DEBUG
  }
}

/*!
  Remove all entries from the directory search list.

  \sa addDirectoryFirst(), addDirectoryLast()
  \sa addEnvDirectoriesFirst(), addEnvDirectoriesLast()
  \sa removeDirectory()
 */
void
SoInput::clearDirectories(void)
{
  while (SoInput::dirsearchlist->getLength() > 0) {
    delete (*SoInput::dirsearchlist)[0];
    SoInput::dirsearchlist->remove(0);
  }
}

/*!
  Returns the list of directories which'll be searched upon loading
  Coin and VRML format files. Directory searches will be done whenever
  any external references appears in a file, for instance to texture images.
 */
const SbStringList &
SoInput::getDirectories(void)
{
  if (soinput_tls) {
    soinput_tls_data * data = (soinput_tls_data *)soinput_tls->get();
    if (data->instancecount) { return *data->searchlist; }
  }

  return *SoInput::dirsearchlist;
}

/*!
  Initialize static variables in class. Called from SoDB::init().
*/
void
SoInput::init(void)
{
  // Debugging for memory leaks will be easier if we also clean up the
  // static / one-off resource usage.
  coin_atexit((coin_atexit_f *)SoInput::clean, 0);

  // This will catch multiple initClass() calls (unless there's a
  // removeDirectories() in between them, which is unlikely to happen
  // inadvertently).
  assert(SoInput::dirsearchlist == NULL);

  SoInput::dirsearchlist = new SbStringList;
  SoInput::addDirectoryFirst(".");

  soinput_tls = new SbStorage(sizeof(soinput_tls_data),
                              soinput_construct_tls_data,
                              soinput_destruct_tls_data);
}

// Clean out static variables in class (to aid in searching for memory
// leaks).
void
SoInput::clean(void)
{
  SoInput::clearDirectories();
  delete SoInput::dirsearchlist;
  SoInput::dirsearchlist = NULL;

  delete soinput_tls;
}

/*!
  Finds and returns the part of the given filename which is the
  directory path name.
 */
SbString
SoInput::getPathname(const char * const filename)
{
  // FIXME: this method would be a prime candidate for the planned
  // SbFile class of filesystem abstractions to hide system
  // differences. 20011021 mortene.

#ifdef HAVE__SPLITPATH

  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];

  _splitpath(filename, drive, dir, NULL, NULL);

  SbString s(drive);
  s += dir;
  return s;

#else // HAVE__SPLITPATH

  const char * ptr = strrchr(filename, '/');
  if (ptr == NULL) return SbString("");

  SbString s = filename;
  return s.getSubString(0, ptr-filename);

#endif // !HAVE__SPLITPATH
}

/*!
  Finds and returns the part of the given filename which is the
  directory path name.
 */
SbString
SoInput::getPathname(const SbString & s)
{
  return SoInput::getPathname(s.getString());
}

/*!
  Finds and returns the part of the given filename which is the base name
  (i.e. the filename without the leading directory path).
 */
SbString
SoInput::getBasename(const char * const filename)
{
  // FIXME: this method would be a prime candidate for the planned
  // SbFile class of filesystem abstractions to hide system
  // differences. 20011021 mortene.

#ifdef HAVE__SPLITPATH

  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];

  _splitpath(filename, NULL, NULL, fname, ext);

  SbString s(fname);
  s += ext;
  return s;

#else // UNIX systems

  const char * ptr = strrchr(filename, '/');
  if (ptr == NULL) return SbString(filename);

  SbString s = filename;
  return s.getSubString(ptr - filename + 1, -1);

#endif // UNIX
}

/*!
  Finds and returns the part of the given filename which is the base name
  (i.e. the filename without the leading directory path).
 */
SbString
SoInput::getBasename(const SbString & s)
{
  return SoInput::getBasename(s.getString());
}

// internal method used for testing if a file exists
static SbBool
test_filename(const SbString & filename)
{
  FILE * fp = fopen(filename.getString(), "rb");
#if COIN_DEBUG && 0 // flip 1<->0 to turn texture search trace on or off
  SoDebugError::postInfo("test_filename", "file search: %s (%s)\n",
                         filename.getString(), fp ? "hit" : "miss");
#endif // !COIN_DEBUG

  if (fp != NULL) {
    fclose(fp);
    return TRUE;
  }
  return FALSE;
}

/*!
  Given a \a basename for a file and an array of \a directories to
  search, returns the full name of the file found.

  In addition to looking at the root of each directory in \a
  directories, all \a subdirectories is also searched for each item in
  \a directories.

  If no file matching \a basename could be found in any of the
  directories, returns an empty string.

  This method is a Coin extension, not part of the original Inventor
  API.
*/
SbString
SoInput::searchForFile(const SbString & basename,
                       const SbStringList & directories,
                       const SbStringList & subdirectories)
{
  int i;

  if (test_filename(basename)) return basename;

  SbString fullname = basename;

  SbBool trypath = TRUE;
  const char * strptr = basename.getString();
  const char * lastunixdelim = strrchr(strptr, '/');
  const char * lastdosdelim = strrchr(strptr, '\\');
  if (!lastdosdelim) {
    lastdosdelim = strrchr(strptr, ':');
    if (lastdosdelim) trypath = FALSE;
  }
  const char * lastdelim = SbMax(lastunixdelim, lastdosdelim);

  if (lastdelim && trypath) {
    SbString tmpstring;
    for (i = 0; i < directories.getLength(); i++) {
      SbString dirname(directories[i]->getString());
      int dirlen = dirname.getLength();

      if (dirlen > 0 &&
          dirname[dirlen-1] != '/' &&
          dirname[dirlen-1] != '\\' &&
          dirname[dirlen-1] != ':') {
        dirname += "/";
      }

      tmpstring.sprintf("%s%s", dirname.getString(),
                        fullname.getString());
      if (test_filename(tmpstring)) return tmpstring;
    }
  }

  SbString base = lastdelim ?
    basename.getSubString(lastdelim-strptr + 1, -1) :
    basename;

  for (i = 0; i < directories.getLength(); i++) {
    SbString dirname(directories[i]->getString());
    int dirlen = dirname.getLength();

    if (dirlen > 0 &&
        dirname[dirlen-1] != '/' &&
        dirname[dirlen-1] != '\\' &&
        dirname[dirlen-1] != ':') {
      dirname += "/";
    }
    fullname.sprintf("%s%s", dirname.getString(),
                     base.getString());
    if (test_filename(fullname)) return fullname;
    for (int j = 0; j < subdirectories.getLength(); j++) {
      fullname.sprintf("%s%s/%s", dirname.getString(),
                       subdirectories[j]->getString(),
                       base.getString());
      if (test_filename(fullname)) return fullname;
    }
  }
  // none found
  return SbString("");
}


/*!
  Changes the file format version number for the stream at the top of the
  stack.
 */
void
SoInput::setIVVersion(float version)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  fi->setIvVersion(version);
}

/*!
  This function has been obsoleted in Coin.
*/
void
SoInput::initFile(FILE * /* newFP */, const char * /* fileName */,
                  SbString * /* fullName */,
                  SbBool /* openedHere */, SbDict * /* refDict */)
{
  COIN_OBSOLETED();
}

/*!
  Returns \c TRUE if the current stream has had it's header parsed.
  If it hasn't, this method will attempt to read the header and returns
  \c TRUE if it could be done.

  If \a bValidateBufferHeader is \c TRUE, the returned flag will also
  be \c FALSE if the file stream header was not of a supported file
  format type.
*/
SbBool
SoInput::checkHeader(SbBool bValidateBufferHeader)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  if (!fi) {
#if COIN_DEBUG
    SoDebugError::post("SoInput::checkHeader", "no files on the stack");
#endif // COIN_DEBUG
    return FALSE;
  }

  // Auto pop on EOF if there's more than one file in the stack.
  if ((this->filestack.getLength() > 1) && fi->isEndOfFile()) {
    this->popFile();
    fi = this->getTopOfStack();
    if (!fi) return FALSE;
  }
  return fi->readHeader(this) && (!bValidateBufferHeader || fi->ivVersion() != 0.0f);
}

/*!
  Returns \c TRUE if the current input stream is fetching data from a
  memory buffer, or \c FALSE if the input is from a file.
*/
SbBool
SoInput::fromBuffer(void) const
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);
  return fi->isMemBuffer() ? TRUE : FALSE;
}

/*!
  Move file pointer past whitespace in the current file or buffer.
  Returns \c FALSE if end of file is encountered.
 */
SbBool
SoInput::skipWhiteSpace(void)
{
  return (this->checkHeader() && this->getTopOfStack()->skipWhiteSpace());
}

/*!
  Pop the topmost file off the stack. Returns \c FALSE if there was no
  files on the stack to pop.

  \sa pushFile(), openFile(), closeFile()
 */
SbBool
SoInput::popFile(void)
{
  if (this->filestack.getLength() == 0) return FALSE;

  SoInput_FileInfo * topofstack = this->getTopOfStack();

  // connect routes before applying post callbacks
  topofstack->connectRoutes(this);

  // unreference Proto definitions
  topofstack->unrefProtos();

  // apply post callback, even if we're not going to pop
  topofstack->applyPostCallback(this);
  if (this->filestack.getLength() == 1) return FALSE;

  if (topofstack->ivFilePointer()) {
    const char * filename = topofstack->ivFilename().getString();
    SbString path = SoInput::getPathname(filename);
    if (path.getLength()) SoInput::removeDirectory(path.getString());
  }
  delete topofstack;
  this->filestack.remove(0);
  return TRUE;
}

/*!
  This function has been obsoleted in Coin.
 */
size_t
SoInput::freeBytesInBuf(void) const
{
  COIN_OBSOLETED();
  return 0;
}

/*!
  Reads 32-bit signed integer value from the current stream. Returns
  \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::readInteger(int32_t & l)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  // FIXME: fixed size buffer for input of unknown
  // length. Ouch. 19990530 mortene.
  char str[32];
  char * s = str;

  if (this->readChar(s, '-') || this->readChar(s, '+'))
    s++;

  if (! this->readUnsignedIntegerString(s))
    return FALSE;

  // FIXME: check man page of strtol and exploit the functionality
  // provided better -- it looks like we are duplicating some of the
  // effort. 19990530 mortene.
  l = strtol(str, NULL, 0);

  return TRUE;
}

/*!
  Reads 32-bit unsigned integer value from the current stream. Returns
  \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::readUnsignedInteger(uint32_t & l)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  // FIXME: fixed size buffer for input of unknown
  // length. Ouch. 19990530 mortene.
  char str[32];
  if (! this->readUnsignedIntegerString(str))
    return FALSE;

  // FIXME: check man page of strtoul and exploit the functionality
  // provided better -- it looks like we are duplicating some of the
  // effort. 19990530 mortene.
  l = strtoul(str, NULL, 0);

  return TRUE;
}

/*!
  Reads a double-precision floating point value from the current stream.
  Returns \c FALSE if we hit end of file prematurely or if no valid string
  representing a value could be read.
 */
SbBool
SoInput::readReal(double & d)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  // FIXME: fixed size buffer for input of unknown
  // length. Ouch. 19990530 mortene.
  const int BUFSIZE = 2048;
  char str[BUFSIZE];
  int n;
  char * s = str;
  SbBool gotNum = FALSE;

  n = this->readChar(s, '-');
  if (n == 0)
    n = this->readChar(s, '+');
  s += n;

  if ((n = this->readDigits(s)) > 0) {
    gotNum = TRUE;
    s += n;
  }

  if (this->readChar(s, '.') > 0) {
    s++;

    if ((n = this->readDigits(s)) > 0) {
      gotNum = TRUE;
      s += n;
    }
  }

  if (! gotNum)
    return FALSE;

  n = this->readChar(s, 'e');
  if (n == 0)
    n = this->readChar(s, 'E');

  if (n > 0) {
    s += n;

    n = this->readChar(s, '-');
    if (n == 0)
      n = this->readChar(s, '+');
    s += n;

    if ((n = this->readDigits(s)) > 0)
      s += n;

    else
      return FALSE; }

  *s = '\0';

  // FIXME: using the wrapped atof() call like this might have serious
  // consequences for performance upon import of large
  // iv/wrl-files. This should be checked. 20030407 mortene.

  d = coin_atof(str);

  return TRUE;
}

/*!
  Reads a set of bytes from the stream making up an unsigned integer and
  puts them at \a str.

  Returns \c FALSE if no string representing an unsigned integer could be
  read.
 */
SbBool
SoInput::readUnsignedIntegerString(char * str)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  int minSize = 1;
  char * s = str;

  if (this->readChar(s, '0')) {
    if (this->readChar(s + 1, 'x')) {
      s += 2 + this->readHexDigits(s + 2);
      minSize = 3;
    }
    else
      s += 1 + this->readDigits(s + 1);
  }
  else
    s += this->readDigits(s);

  if (s - str < minSize)
    return FALSE;

  *s = '\0';

  return TRUE;
}

/*!
  Read decimal base digits from the current input stream into \a str and
  returns the number of characters read.
 */
int
SoInput::readDigits(char * str)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);

  char c, * s = str;

  while (fi->get(c)) {
    if (isdigit(c))
      *s++ = c;
    else {
      fi->putBack(c);
      break;
    }
  }

  return s - str;
}

/*!
  Read hexadecimal base digits from the current input stream into \a str and
  returns the number of characters read.
 */
int
SoInput::readHexDigits(char * str)
{
  // FIXME: write code for reading from binary stream? 19990628 mortene.
  assert(!this->isBinary());

  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);

  char c, * s = str;
  while (fi->get(c)) {

    if (isxdigit(c)) *s++ = c;
    else {
      fi->putBack(c);
      break;
    }
  }

  return s - str;
}

/*!
  Reads the next character in stream and compares it to \a charToRead.
  Returns \c 1 on success, \c 0 on failure. Failure may come from either
  end of file, or from the next character in the stream being different
  than \a charToRead.
 */
int
SoInput::readChar(char * s, char charToRead)
{
  SoInput_FileInfo * fi = this->getTopOfStack();
  assert(fi);

  int ret = 0;
  char c;
  if (fi->get(c)) {
    if (c == charToRead) {
      *s = c;
      ret = 1;
    }
    else {
      fi->putBack(c);
    }
  }

  return ret;
}

/*!
  This function has been obsoleted in Coin.
 */
SbBool
SoInput::makeRoomInBuf(size_t /* nBytes */)
{
  COIN_OBSOLETED();
  return FALSE;
}

/*!
  Convert the bytes at \a from (which must be a short integer in network
  format (i.e. most significant byte first)) to a short integer in native
  format which is put at the \a s pointer.
 */
void
SoInput::convertShort(char * from, short * s)
{
  // Convert MSB -> LSB, if necessary. Ugly hack.
  *s = (short) coin_ntoh_uint16(*((uint16_t *)from));
}

/*!
  Convert the bytes at \a from (which must be a 32-bit integer in network
  format (i.e. most significant byte first)) to a 32-bit integer in native
  format which is put at the \a l pointer.
 */
void
SoInput::convertInt32(char * from, int32_t * l)
{
  // Convert MSB -> LSB, if necessary. Ugly hack.
  // assert(sizeof(int32_t) == sizeof(unsigned long int)); // Fails on 64-bit archs?
  *l = (int32_t)coin_ntoh_uint32(*((uint32_t *)from));
}

/*!
  Convert the bytes at \a from (which must be a single-precision floating
  point number in network format) to a single-precision floating point
  number in native format which is put at the \a f pointer.
 */
void
SoInput::convertFloat(char * from, float * f)
{
  // Jesus H. Christ -- this unbelievably ugly hack actually kinda
  // works. Probably because the bitpatterns of the parts of float
  // numbers are standardized according to IEEE 754 (?).
  assert(sizeof(uint32_t) == sizeof(float));
  // FIXME: we now have coin_ntoh_float() and coin_hton_float()
  // functions in tidbits.c. 20021121 mortene.
  uint32_t fbitval = coin_ntoh_uint32(*((uint32_t *)from));
  (void)memcpy(f, &fbitval, sizeof(float));
}

/*!
  Convert the bytes at \a from (which must be a double-precision floating
  point number in network format) to a double-precision floating point
  number in native format which is put at the \a d pointer.
 */
void
SoInput::convertDouble(char * from, double * d)
{
  // This is so ugly it makes Madeleine Albright appear as Miss
  // Universe, but hey -- it works for me. I think.
  assert(sizeof(uint32_t) * 2 == sizeof(double));
  unsigned long int dbitvals[2] = {
    // FIXME: we now have coin_ntoh_float() and coin_hton_float()
    // functions in tidbits.c. 20021121 mortene.
    coin_ntoh_uint32(*((uint32_t *)from)),
    coin_ntoh_uint32(*((uint32_t *)(from + sizeof(double)/2))),
  };
  (void)memcpy(d, dbitvals, sizeof(double));
}

/*!
  Convert a block of short numbers in network format to native format.

  \sa convertShort()
 */
void
SoInput::convertShortArray(char * from, short * to, int len)
{
  for (int i=0; i < len; i++) {
    this->convertShort(from, to);
    from += sizeof(short);
    to++;
  }
}

/*!
  Convert a block of 32-bit integer numbers in network format to native format.

  \sa convertInt32()
 */
void
SoInput::convertInt32Array(char * from, int32_t * to, int len)
{
  for (int i=0; i < len; i++) {
    this->convertInt32(from, to);
    from += sizeof(int32_t);
    to++;
  }
}

/*!
  Convert a block of single-precision floating point numbers in
  network format to native format.

  \sa convertFloat()
 */
void
SoInput::convertFloatArray(char * from, float * to, int len)
{
  for (int i=0; i < len; i++) {
    this->convertFloat(from, to);
    from += sizeof(float);
    to++;
  }
}

/*!
  Convert a block of double-precision floating point numbers in
  network format to native format.

  \sa convertDouble()
 */
void
SoInput::convertDoubleArray(char * from, double * to, int len)
{
  for (int i=0; i < len; i++) {
    this->convertDouble(from, to);
    from += sizeof(double);
    to++;
  }
}

/*!
  Returns \c TRUE if current file is a VRML V1.0 file.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SoInput::isFileVRML1(void)
{
  (void) this->checkHeader();
  SoInput_FileInfo * fi = this->getTopOfStack();
  if (fi) return fi->isFileVRML1();
  return FALSE;
}

/*!
  Returns \c TRUE if current file is a VRML 2 / VRML97 file.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SoInput::isFileVRML2(void)
{
  (void) this->checkHeader();
  SoInput_FileInfo * fi = this->getTopOfStack();
  if (fi) return fi->isFileVRML2();
  return FALSE;
}

/*!
  This function has been obsoleted in Coin.
*/
void
SoInput::resetFilePointer(FILE * /* fptr */)
{
  COIN_OBSOLETED();
}

/*!
  This function is part of the TGS Inventor API, but is not
  implemented in Coin.
*/
SbBool
SoInput::isFileURL(const char * /* url */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  This function is part of the TGS Inventor API, but is not
  implemented in Coin.
*/
char *
SoInput::URLToFile(char * /* out_buf */, const char * /* in_buf */)
{
  COIN_STUB();
  return NULL;
}

/*!
  This function is part of the TGS Inventor API, but is not
  implemented in Coin.
*/
SbBool
SoInput::IsURL(const char * /* c_strng */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  Set up the directory search path directly with the list of directories
  in \a dirs. Note that "." should be part of the directory list if you
  want to handle files specified by a relative path.
 */
void
SoInput::setDirectories(SbStringList * dirs)
{
  // Dealloc SbString objects
  for (int i=0; i < SoInput::dirsearchlist->getLength(); i++)
    delete (*SoInput::dirsearchlist)[i];

  (*SoInput::dirsearchlist) = *dirs;
}

/*!
  \COININTERNAL

  Return the currently active stream.
*/
SoInput_FileInfo *
SoInput::getTopOfStack(void) const
{
  if (this->filestack.getLength() == 0) {
#if COIN_DEBUG
    SoDebugError::post("SoInput::getTopOfStack", "no files in stack");
#endif // COIN_DEBUG
    return NULL;
  }
  return this->filestack[0];
}

/*!
  Scan through the include directories in order and try to find a file
  with the given name in any one of them. If file is found, put the
  full name with complete path from current directory of process into
  \a fullname, open the file and return the file pointer.

  If the file could either not be found or not opened for reading,
  return \c NULL.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.
 */
FILE *
SoInput::findFile(const char * basename, SbString & fullname)
{
  fullname = "";

  if (strlen(basename) < 1) {
    SoDebugError::post("SoInput::findFile", "Was asked to find a file with no name!");
    return NULL;
  }

  const char * env = coin_getenv("COIN_DEBUG_SOINPUT_FINDFILE");
  const SbBool DEBUG_FILE_SEARCHING = env && (atoi(env) > 0);
  if (DEBUG_FILE_SEARCHING) {
    cc_string str;
    cc_string_construct(&str);
    SbBool ok = coin_getcwd(&str);
    if (!ok) {
      SoDebugError::post("SoInput::findFile",
                         "Couldn't get current working directory: %s",
                         cc_string_get_text(&str));
    }
    else {
      SoDebugError::postInfo("SoInput::findFile",
                             "Current working directory: '%s'",
                             cc_string_get_text(&str));
    }
    cc_string_clean(&str);
  }

  SbStringList sl = SoInput::getDirectories();

  // Make sure we try from cwd first.
  SbString relativepath("");
  sl.insert(&relativepath, 0);

  FILE * fp = NULL;
  for (int diridx = 0; diridx < sl.getLength(); diridx++) {
    SbString n = * sl[diridx];
    const int namelen = n.getLength();
    if ((namelen && n[namelen - 1] != '/' && n[namelen - 1] != '\\') &&
        (basename[0] != '/' && basename[0] != '\\')) {
      n += "/";
    }
    n += basename;

    struct stat buf;
    if ((stat(n.getString(), &buf) == 0) && !S_ISDIR(buf.st_mode)) {
      fp = fopen(n.getString(), "rb");
      if (fp != NULL) {
        if (DEBUG_FILE_SEARCHING) {
          SoDebugError::postInfo("SoInput::findFile", "successfully fopened '%s'", n.getString());
        }
        fullname = n;
        return fp;
      }
      else {
        fullname.sprintf("%s%sFound '%s' as '%s', but was unable to open it: '%s'",
                         fullname.getString(), fullname.getLength() ? "\n" : "",
                         basename, n.getString(), strerror(errno));
      }
    }
  }

  // If file was not found, list all directories where we looked for
  // it.
  SbBool foundbutcouldntopen = fullname.getLength() > 0;
  if (!foundbutcouldntopen) {
    cc_string str;
    cc_string_construct(&str);
    SbBool ok = coin_getcwd(&str);
    fullname.sprintf("Could not find '%s' in any of the "
                     "following directories (from cwd '%s'):",
                     basename, ok ? cc_string_get_text(&str) : "<unknown>");
    cc_string_clean(&str);

    for (int diridx = 0; diridx < sl.getLength(); diridx++) {
      fullname += "\n\t'";
      fullname += * sl[diridx];
      fullname += "'";
    }
  }

  return NULL;
}
