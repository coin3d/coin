/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/fields/SoField.h>
#include <coindefs.h> // COIN_STUB(), COIN_OBSOLETED()
#include <Inventor/SoDB.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/misc/SoProto.h>
#include <../tidbits.h> // coin_atexit()
#include <stdlib.h>
#include <string.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <sys/stat.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H
#if HAVE_SYS_TYPES_H
/* According to Coin user Ralf Corsepius, at least SunOS4 needs to
   include sys/types.h before netinet/in.h. There have also been a
   problem report for FreeBSD which seems to indicate that the same
   dependency exists on that platform aswell. */
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H
#include <ctype.h>
#include "../tidbits.h" // coin_getenv() & coin_ntoh*()

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


const unsigned int READBUFSIZE = 65536;
static const char IS_KEYWORD[] = "IS";

// *************************************************************************

class SoInput_FileInfo {
public:
  SoInput_FileInfo(const char * const filename, FILE * filepointer);
  SoInput_FileInfo(void * bufPointer, size_t bufSize);
  void commonConstructor(void);

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
    return this->ismembuffer;
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
  const SbString & ivFilename(void) {
    return this->filename;
  }

  FILE * ivFilePointer(void) {
    return this->fp;
  }
  unsigned int lineNr(void) {
    return this->linenr;
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
    // FIXME: isspace() takes the current locale into account. Under
    // MSWindows, this can lead to "interesting" artifacts, like a
    // case with RR tracked down and fixed by <thammer@sim.no> where a
    // character (was it ü?) with ASCII value > 127 made isspace()
    // return non-nil on a German system. We very likely need to audit
    // and fix our isspace() calls in the Coin sourcecode to behave in
    // the exact manner that we expect them to. 20020319 mortene.
    return isspace(c) || (this->vrml2file && c == ',');
  }

  void connectRoutes(void);
  void unrefProtos(void);

private:
  SbString filename;
  FILE * fp;
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
  SbBool ismembuffer;
  SbBool headerisread, eof;
  SbBool vrml1file;
  SbBool vrml2file;

  SbList <SbName> routelist;
  SbList <SoProto*> protolist;
  SbList <SoProto*> protostack;
};

SbStringList * SoInput::dirsearchlist = NULL;

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
  \internal
  Do common tasks for the constructors.
*/
void
SoInput::constructorsCommon(void)
{
  this->setFilePointer(stdin);
}

/*!
  Destructor.
 */
SoInput::~SoInput(void)
{
  this->closeFile();
}

/*!
  Adds a ROUTE from /a fromnode's \fromfield, to \a tonode's
  tofield. This makes it possible to defines ROUTEs in files
  before the \a fromnode or \a tonode is parsed.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-12
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
  Adds a Proto which should be active in the current scope.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-15
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

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-23

  \sa popProto()
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

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-23

  \sa pushProto()
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

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-23

  \sa pushProto()
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

  This method was not part of the SGI Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-23
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
          // FIXME: see note about isspace() in SoInput_FileInfo::isSpace().
          // 20020319 mortene.
          else if (!isspace(c)) state = STATE_NOTFOUND;
          break;
        case STATE_EXPECT_S:
          if (c == 'S') state = STATE_EXPECT_SPACE;
          else state = STATE_NOTFOUND;
          break;
        case STATE_EXPECT_SPACE:
          // FIXME: see note about isspace() in SoInput_FileInfo::isSpace().
          // 20020319 mortene.
          if (isspace(c)) state = STATE_FOUND;
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

  \sa openFile(), closeFile()
 */
void
SoInput::setFilePointer(FILE * newFP)
{
  this->closeFile();

  const char * name = newFP == stdin ? "<stdin>" : "";
  SoInput_FileInfo * newfile = new SoInput_FileInfo(name, newFP);

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

  \sa setFilePointer(), pushFile(), closeFile() */
SbBool
SoInput::openFile(const char * fileName, SbBool okIfNotFound)
{
  this->closeFile();

  SbString fullname;
  FILE * fp = this->findFile(fileName, fullname);
  if (fp) {
    SoInput_FileInfo * newfile =
      new SoInput_FileInfo(fullname.getString(), fp);
    this->filestack.insert(newfile, 0);

    SoInput::addDirectoryFirst(SoInput::getPathname(fullname).getString());
    return TRUE;
  }

  if (!okIfNotFound)
    SoReadError::post(this, "Couldn't open file '%s' for reading.", fileName);

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
  // Get rid of default stdin filepointer.
  if (this->filestack.getLength() == 1 &&
      this->filestack[0]->ivFilePointer() == stdin) this->closeFile();

  SbString fullname;
  FILE * fp = this->findFile(filename, fullname);
  if (fp) {
    SoInput_FileInfo * newfile =
      new SoInput_FileInfo(fullname.getString(), fp);

    this->filestack.insert(newfile, 0);

    SoInput::addDirectoryFirst(SoInput::getPathname(fullname).getString());
    return TRUE;
  }

  SoReadError::post(this, "Couldn't open file '%s' for reading.", filename);
  return FALSE;
}

/*!
  Removes all files from the internal stack. Any files opened by openFile() or
  pushFile() will also be closed, but not the files coming from
  setFilePointer() -- which are the responsibility of the caller.

  \sa setFilePointer(), openFile(), pushFile()
 */
void
SoInput::closeFile(void)
{
  // Remove all entries, including the default <stdin>.
  while (this->filestack.getLength() > 0) {
    if (!this->fromBuffer() && (this->getCurFile() != stdin)) {
      SbString s = SoInput::getPathname(this->getTopOfStack()->ivFilename());
      if (s.getLength()) SoInput::removeDirectory(s.getString());
    }
    delete this->getTopOfStack();
    this->filestack.remove(0);
  }
}

/*!
  Checks to see if the current file is a valid file which can be read by
  the Coin library.
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
  Returns \c TRUE if the current file is a valid file which can be read by
  the Coin library.
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
  Sets up the input stream for reading from a memory buffer. Closes all
  open files in the file stack first.
 */
void
SoInput::setBuffer(void * bufpointer, size_t bufsize)
{
  // FIXME: shouldn't the first argument to this method rather have
  // been a "const char *"? 20010821 mortene.

  this->closeFile();
  SoInput_FileInfo * newfile = new SoInput_FileInfo(bufpointer, bufsize);
  this->filestack.insert(newfile, 0);
}

/*!
  Returns number of bytes read so far from the current file or memory
  buffer.
 */
size_t
SoInput::getNumBytesRead(void) const
{
  return this->getTopOfStack()->getNumBytesParsedSoFar();
}

/*!
  Returns header of current file.
*/
SbString
SoInput::getHeader(void)
{
  this->checkHeader();
  return this->getTopOfStack()->ivHeader();
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
  whitespace character or left or right bracket (i.e. ``['' or ``]'').

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
    // FIXME: guess at a sensible limit. 19990711 mortene.
    const unsigned int MAXSTRLEN = 10 * 1024;
    unsigned int slen;

    if (!this->read(slen)) return FALSE;
    // Sanity check
    if (slen > MAXSTRLEN) {
      SoReadError::post(this, "String too long (%d characters)", slen);
      return FALSE;
    }

    char buffer[MAXSTRLEN+4+1];
    if (slen && !fi->getChunkOfBytes((unsigned char *)buffer, ((slen+3)/4)*4))
      return FALSE;
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

  char bufStore[256];
  int bytesLeft;
  do {
    char * buf = bufStore;
    bytesLeft = sizeof(bufStore) - 1;

    while (bytesLeft > 0) {
      if (!fi->get(*buf)) return FALSE;

      if (quoted) {
        if (*buf == '\"') break;

        if (*buf == '\\') {
          if (!fi->get(c)) return FALSE;
          if (c == '\"') *buf = '\"';
          else fi->putBack(c);
        }
      }
      else if (fi->isSpace(*buf)) {
        fi->putBack(*buf);
        break;
      }
      else if (*buf == '[') {
        fi->putBack(*buf);
        break;
      }
      else if (*buf == ']') {
        fi->putBack(*buf);
        break;
      }

      buf++;
      bytesLeft--;
    }

    *buf = '\0';
    s += bufStore;

  } while (bytesLeft == 0);

  return TRUE;
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
      if (!SbName::isIdentStartChar(s[0])) return FALSE;
      for (int i = 1; i < s.getLength(); i++)
        if (!SbName::isIdentChar(s[i])) return FALSE;
    }

    return TRUE;
  }
  // ASCII format.
  else {
    if (!fi->skipWhiteSpace()) return FALSE;

    if (!validIdent) {
      SbString s;
      if (!this->read(s)) return FALSE;

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
    }
    else {
      SbString s;
      char buf[256];
      char * b = buf;
      char c;
      SbBool gotchar;

      if ((gotchar = fi->get(c)) && SbName::isIdentStartChar(c)) {
        *b++ = c;

        while ((gotchar = fi->get(c)) && SbName::isIdentChar(c)) {
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
  if (!this->checkHeader()) return FALSE; \
  SoInput_FileInfo * fi = this->getTopOfStack(); \
  assert(fi); \
  if (!fi->skipWhiteSpace()) return FALSE; \
  readType _tmp; \
  if (!this->reader(_tmp)) return FALSE; \
  num = (type) _tmp; \
  return TRUE;

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
    s = tmp;
    return TRUE;
  }
  else {
    READ_INTEGER(s, short);
  }
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
    s = tmp;
    return TRUE;
  }
  else {
    READ_UNSIGNED_INTEGER(s, unsigned short);
  }
}

/*!
  Read float value from current file or buffer position and place
  it in \a f. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(float & f)
{
  if (this->isBinary()) {
    float tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    f = tmp;
    return TRUE;
  }
  else {
    READ_REAL(f, float);
  }
}

/*!
  Read double value from current file or buffer position and place
  it in \a d. Returns \c FALSE if we hit end of file prematurely.
 */
SbBool
SoInput::read(double & d)
{
  if (this->isBinary()) {
    double tmp;
    if (!this->readBinaryArray(&tmp, 1)) return FALSE;
    d = tmp;
    return TRUE;
  }
  else {
    READ_REAL(d, double);
  }
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
    str.sprintf("\tOccurred at position %d in binary file %s",
                this->getTopOfStack()->getNumBytesParsedSoFar(),
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
  this->getTopOfStack()->putBack(c);
}

/*!
  Insert string \a str in stream at the current position.
 */
void
SoInput::putBack(const char * str)
{
  this->getTopOfStack()->putBack(str);
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
  \internal

  Add a directory to the search list at the specified index value. An index
  of -1 means append.
 */
void
SoInput::addDirectoryIdx(const int idx, const char * dirName)
{
  assert(idx > -2);
  assert(idx <= SoInput::dirsearchlist->getLength());

  // NB: note that it _should_ be possible to append/insert the same
  // directory name multiple times, as this is an easy way of
  // "stacking" names when doing recursive SoDB::readAll() calls or
  // using SoInput::pushFile(). So don't try to "fix" or change this
  // aspect of adding entries to the directory search list. --mortene

  SbString * ns = new SbString(dirName);
  if (idx == -1) SoInput::dirsearchlist->append(ns);
  else if (idx != -1) SoInput::dirsearchlist->insert(ns, idx);
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
  \internal

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
  int idx = SoInput::dirsearchlist->getLength() - 1;
  for (; idx >= 0; idx--) {
    if (*((*SoInput::dirsearchlist)[idx]) == dirName) break;
  }

  if (idx >=0) {
    delete (*SoInput::dirsearchlist)[idx]; // Dealloc SbString object
    SoInput::dirsearchlist->remove(idx);
  }
#if COIN_DEBUG
  else {
    SoDebugError::postWarning("SoInput::removeDirectory",
                              "Tried to remove nonexistent directory '%s'"
                              " in directory search list.", dirName);
  }
#endif // COIN_DEBUG
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
  return *SoInput::dirsearchlist;
}

/*!
  Initialize static variables in class. Called from SoDB::init().
*/
void
SoInput::init(void)
{
#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage.
  coin_atexit((coin_atexit_f *)SoInput::clean);
#endif // COIN_DEBUG

  // This will catch multiple initClass() calls (unless there's a
  // removeDirectories() in between them, which is unlikely to happen
  // inadvertently).
  assert(SoInput::dirsearchlist == NULL);

  SoInput::dirsearchlist = new SbStringList;
  SoInput::addDirectoryFirst(".");
}

// Clean out static variables in class (to aid in searching for memory
// leaks).
void
SoInput::clean(void)
{
#if COIN_DEBUG
  SoInput::clearDirectories();
  delete SoInput::dirsearchlist;
#endif // COIN_DEBUG
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

  // FIXME: checking for system instead of feature is bad coding
  // style.  Use a configure check instead. 20011021 mortene.

#ifdef _WIN32

  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];

  _splitpath(filename, drive, dir, NULL, NULL);

  SbString s(drive);
  s += dir;
  return s;

#else // UNIX systems

  const char * ptr = strrchr(filename, '/');
  if (ptr == NULL) return SbString("");

  SbString s = filename;
  return s.getSubString(0, ptr-filename);

#endif // UNIX
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

  // FIXME: checking for system instead of feature is bad coding
  // style.  Use a configure check instead. 20011021 mortene.
#ifdef _WIN32

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
  topofstack->connectRoutes();

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
  char str[32];
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

  d = atof(str);

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
  uint32_t fbitval = coin_ntoh_uint32(*((uint32_t *)from));
  memcpy(f, &fbitval, sizeof(float));
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
    coin_ntoh_uint32(*((uint32_t *)from)),
    coin_ntoh_uint32(*((uint32_t *)(from + sizeof(double)/2))),
  };
  memcpy(d, dbitvals, sizeof(double));
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
  Convert a b ock of single-precision floating point numbers in
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
  Returns \e TRUE if current file is a VRML V1.0 file.

  This method is an extension versus the Open Inventor API.
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
  Returns \a TRUE if current file is a VRML97 file.
  This method is provided for backwards compatibility only,
  and will always return the same as isFileVRML97(), which
  we prefer that you use.

  This method is an extension versus the original SGI Open Inventor
  API.
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
  \internal

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
  // FIXME: test this code on MSWindows. If it doesn't work,
  // *fix*. 20010814 mortene.

#define DEBUG_FILE_SEARCHING 0

  int baselen = strlen(basename);
  if (!baselen) return NULL;

  SbStringList sl = SoInput::getDirectories();
  SbString relativepath("");
  sl.insert(&relativepath, 0); // So we'll try relative path from cwd first.

  int diridx = 0;
  FILE * fp = NULL;
  while (!fp && (diridx < sl.getLength())) {
    fullname = * sl[diridx++];
    int namelen = fullname.getLength();
    if ((namelen && fullname[namelen - 1] != '/') && (basename[0] != '/'))
      fullname += "/";
    fullname += basename;

#if COIN_DEBUG && DEBUG_FILE_SEARCHING // debug
    SoDebugError::postInfo("SoInput::findFile",
                           "%s", fullname.getString());
#endif // debug

    struct stat buf;
    if ((stat(fullname.getString(), &buf) == 0) && !S_ISDIR(buf.st_mode)) {
      fp = fopen(fullname.getString(), "rb");
#if COIN_DEBUG && DEBUG_FILE_SEARCHING // debug
      SoDebugError::postInfo("SoInput::findFile",
                             "fopen(%s)", fullname.getString());
#endif // debug
    }
  }

#if COIN_DEBUG && DEBUG_FILE_SEARCHING // debug
  SoDebugError::postInfo("SoInput::findFile", "done");
#endif // debug

  return fp;
}


// implementation of the SoInput_FileInfo class

#ifndef DOXYGEN_SKIP_THIS

SoInput_FileInfo::SoInput_FileInfo(const char * const filename, FILE * filepointer)
{
  this->commonConstructor();

  this->filename = filename;
  this->fp = filepointer;
  this->readbuf = new char[READBUFSIZE];
  this->readbuflen = 0;
  this->ismembuffer = FALSE;
}

SoInput_FileInfo::SoInput_FileInfo(void * bufPointer, size_t bufSize)
{
  this->commonConstructor();

  this->filename="<memory>";
  this->fp = NULL;
  this->readbuf = (char *)bufPointer;
  this->readbuflen = bufSize;
  this->ismembuffer=TRUE;
}

void
SoInput_FileInfo::commonConstructor(void)
{
  this->header = NULL;
  this->headerisread = FALSE;
  this->ivversion = 0.0f;
  this->linenr = 1;
  this->readbufidx = 0;
  this->totalread = 0;
  this->lastputback = -1;
  this->lastchar = -1;
  this->eof = FALSE;
  this->isbinary = FALSE;
  this->vrml1file = FALSE;
  this->vrml2file = FALSE;
  this->prefunc = NULL;
  this->postfunc = NULL;
}

SoInput_FileInfo::~SoInput_FileInfo()
{
  if (!this->ismembuffer) delete [] this->readbuf;

  // Close files which are not a memory buffer nor the stdin and
  // which we do have a filename for (if we don't have a filename,
  // the FILE ptr was just passed in through setFilePointer() and
  // is the library programmer's responsibility).
  if (this->fp &&
      (this->filename != "<stdin>") &&
      (this->filename.getLength()))
    fclose(this->fp);
}

SbBool
SoInput_FileInfo::doBufferRead(void)
{
  // Make sure that we really do need to read more bytes.
  assert(this->backbuffer.getLength() == 0);
  assert(this->readbufidx == this->readbuflen);

  if (this->ismembuffer) {
    // Input memory buffers are statically sized entities, so no
    // further reading can be done.
    //
    // (Note that it is still convenient to call doBufferRead()
    // for memory buffer streams, as it provides a "common lowest
    // denominator" for the character reading interface.)
    this->eof = TRUE;
    return FALSE;
  }

  int len = fread(this->readbuf, 1, READBUFSIZE, this->fp);
  if (len <= 0) {
    this->readbufidx = 0;
    this->readbuflen = 0;
    this->eof = TRUE;
#if 0 // debug
    SoDebugError::postInfo("doBufferRead", "met Mr End-of-file");
#endif // debug
    return FALSE;
  }

  this->totalread += this->readbufidx;
  this->readbufidx = 0;
  this->readbuflen = len;
  return TRUE;
}

size_t
SoInput_FileInfo::getNumBytesParsedSoFar(void) const
{
  return this->totalread + this->readbufidx - this->backbuffer.getLength();
}

SbBool
SoInput_FileInfo::getChunkOfBytes(unsigned char * ptr, size_t length)
{
  // Suck out any bytes from the backbuffer first.
  while ((this->backbuffer.getLength() > 0) && (length > 0)) {
    *ptr++ = this->backbuffer.pop();
    length--;
  }

  do {
    // Grab bytes from the buffer.
    while ((this->readbufidx < this->readbuflen) && (length > 0)) {
      *ptr++ = this->readbuf[this->readbufidx++];
      length--;
    }

    // Fetch more bytes if necessary.
    if ((length > 0) && !this->eof) this->doBufferRead();

  } while (length && !this->eof);

  return !this->eof;
}

SbBool
SoInput_FileInfo::get(char & c)
{
  if (this->backbuffer.getLength() > 0) {
    c = this->backbuffer.pop();
  }
  else if (this->readbufidx >= this->readbuflen) {
    // doBufferRead() also does the right thing (i.e. sets the EOF
    // flag for the stream) if we're reading from memory.
    if (!this->doBufferRead()) {
      c = (char) EOF;
      return FALSE;
    }

    c = this->readbuf[this->readbufidx++];
  }
  else {
    c = this->readbuf[this->readbufidx++];
  }

  // NB: the line counting is not working 100% if we start putting
  // back and re-reading '\r\n' sequences.
  if ((c == '\r') || ((c == '\n') && (this->lastchar != '\r')))
    this->linenr++;
  this->lastchar = c;
  this->lastputback = -1;

  return TRUE;
}

void
SoInput_FileInfo::putBack(const char c)
{
  // Decrease line count if we put back an end-of-line character.
  // This should take care of Unix-, MSDOS/MSWin- and MacOS-style
  // generated files. NB: the line counting is not working 100% if
  // we start putting back and re-reading multiple parts of '\r\n'
  // sequences.
  if (!this->isbinary && ((c == '\r') || (c == '\n'))) this->linenr--;

  this->lastputback = (int)c;
  this->lastchar = -1;

  if (this->readbufidx > 0 && this->backbuffer.getLength() == 0) {
    this->readbufidx--;
    // Make sure we write back the same character which was read..
    assert(c == this->readbuf[this->readbufidx]);
  }
  else {
    this->backbuffer.push(c);
  }

  this->eof = FALSE;
}

void
SoInput_FileInfo::putBack(const char * const str)
{
  assert(!this->isbinary);

  int n = strlen(str);
  if (!n) return;

  // Decrease line count if we put back any end-of-line
  // characters. This should take care of Unix-, MSDOS/MSWin- and
  // MacOS-style generated files. What a mess.
  for (int i=0; i < n; i++) {
    if ((str[i] == '\r') || ((str[i] == '\n') &&
                             (this->lastputback != (int)'\r')))
      this->linenr--;
    this->lastputback = (int)str[i];
  }

  this->lastchar = -1;

  if (n <= this->readbufidx && this->backbuffer.getLength() == 0) {
    this->readbufidx -= n;
#if COIN_DEBUG
    for (int i = 0; i < n; i++) {
      assert(this->readbuf[this->readbufidx+i] == str[i]);
    }
#endif // COIN_DEBUG

  }
  else
    for (int i = n - 1; i >= 0; i--) this->backbuffer.push(str[i]);

  this->eof = FALSE;
}

SbBool
SoInput_FileInfo::skipWhiteSpace(void)
{
  const char COMMENT_CHAR = '#';

  while (TRUE) {
    char c;
    SbBool gotchar;
    while ((gotchar = this->get(c)) && this->isSpace(c));

    if (!gotchar) return FALSE;

    if (c == COMMENT_CHAR) {
      while ((gotchar = this->get(c)) && (c != '\n') && (c != '\r'));
      if (!gotchar) return FALSE;
      if (c == '\r') {
        gotchar = this->get(c);
        if (!gotchar) return FALSE;
        if (c != '\n') this->putBack(c);
      }
    }
    else {
      this->putBack(c);
      break;
    }
  }
  return TRUE;
}

// Returns TRUE if an attempt at reading the file header went
// without hitting EOF. Check this->ivversion != 0.0f to see if the
// header parse actually succeeded.

// The SoInput parameter is used in the precallback
SbBool
SoInput_FileInfo::readHeader(SoInput * soinput)
{
  if (this->headerisread) return this->eof ? FALSE : TRUE;
  this->headerisread = TRUE;

  this->header = "";
  this->ivversion = 0.0f;
  this->vrml1file = FALSE;
  this->vrml2file = FALSE;

  char c;
  if (!this->get(c)) return FALSE;

  if (c != '#') {
    this->putBack(c);
    return TRUE;
  }

  this->header += c;

  while (this->get(c) && (c != '\n') && (c != '\r')) this->header += c;
  if (this->eof) return FALSE;

  if (!SoDB::getHeaderData(this->header, this->isbinary, this->ivversion,
                           this->prefunc, this->postfunc, this->userdata,
                           TRUE)) {
    this->ivversion = 0.0f;
  }
  else {
    if (this->header == SbString("#VRML V1.0 ascii")) {
      this->vrml1file = TRUE;
    }
    else if (this->header == SbString("#VRML V2.0 utf8")) {
      this->vrml2file = TRUE;
    }
    if (this->prefunc) this->prefunc(this->userdata, soinput);
  }
  return TRUE;
}

void
SoInput_FileInfo::connectRoutes(void)
{
  const SbName * routeptr = this->routelist.getArrayPtr();
  const int n = this->routelist.getLength();
  for (int i = 0; i < n; i += 4) {
    if (!SoBase::connectRoute(routeptr[i],
                              routeptr[i+1],
                              routeptr[i+2],
                              routeptr[i+3])) {
      SoDebugError::postWarning("SoInput_FileInfo::connectRoutes",
                                "Unable to create route from %s.%s to %s.%s",
                                routeptr[i].getString(),
                                routeptr[i+1].getString(),
                                routeptr[i+2].getString(),
                                routeptr[i+3].getString());

    }
  }
}

// Unrefernce all protos
void
SoInput_FileInfo::unrefProtos(void)
{
  const int n = this->protolist.getLength();
  for (int i = 0; i < n; i++) {
    this->protolist[i]->unref();
  }
  this->protolist.truncate(0);
}

#endif // DOXYGEN_SKIP_THIS
