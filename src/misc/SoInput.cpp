/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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

  \sa SoOutput, SoDB
 */

// METADON doc:
/*¡
  A few minor things are missing in the SoInput class. Grep for
  "FIXME" in the sourcecode.
*/

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbStringList.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/SoDB.h>

#ifdef _WIN32
#include <windows.h>
#else // ! _WIN32
#include <netinet/in.h> // ntohl(), ntohs()
#include <ctype.h>
#endif // ! _WIN32

const unsigned int READBUFSIZE = 65536;

// *************************************************************************

class SoInput_FileInfo {
public:
  SoInput_FileInfo(const char * const filename, FILE * filepointer)
    {
      this->commonConstructor();

      this->filename = filename;
      this->fp = filepointer;
      this->readbuf = new char[READBUFSIZE];
      this->readbuflen = 0;
      this->ismembuffer = FALSE;
    }

  SoInput_FileInfo(void * bufPointer, size_t bufSize)
    {
      this->commonConstructor();

      this->filename="<memory>";
      this->fp = NULL;
      this->readbuf = (char *)bufPointer;
      this->readbuflen = bufSize;
      this->ismembuffer=TRUE;
    }

  void commonConstructor(void)
    {
      this->header = NULL;
      this->headerisread = FALSE;
      this->ivversion = 0.0f;
      this->linenr = 1;
      this->backbufidx = -1;
      this->readbufidx = 0;
      this->totalread = 0;
      this->lastputback = -1;
      this->lastchar = -1;
      this->eof = FALSE;
      this->isbinary = FALSE;
    }

  ~SoInput_FileInfo()
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

  SbBool doBufferRead(void)
    {
      // Make sure that we really do need to read more bytes.
      assert(this->backbufidx == -1);
      assert(this->readbufidx == this->readbuflen);

      if (this->ismembuffer) {
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

  size_t getNumBytesParsedSoFar(void) const
    {
      return this->totalread + this->readbufidx - (this->backbufidx + 1);
    }

  SbBool getChunkOfBytes(unsigned char * ptr, size_t length)
    {
      // Suck out any bytes from the backbuffer first.
      while ((this->backbufidx >= 0) && (length > 0)) {
        *ptr++ = this->backBuf[this->backbufidx--];
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

  SbBool get(char & c)
    {
      if (this->backbufidx >= 0) {
        c = this->backBuf[this->backbufidx--];
      }
      else if (this->readbufidx >= this->readbuflen) {
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

  void putBack(const char c)
    {
      // Decrease line count if we put back an end-of-line character.
      // This should take care of Unix-, MSDOS/MSWin- and MacOS-style
      // generated files. NB: the line counting is not working 100% if
      // we start putting back and re-reading multiple parts of '\r\n'
      // sequences.
      if (!this->isbinary && ((c == '\r') || (c == '\n'))) this->linenr--;

      this->lastputback = c;
      this->lastchar = -1;

      if (this->readbufidx > 0 && this->backbufidx < 0) {
        this->readbufidx--;
        // Make sure we write back the same character which was read..
        assert(c == this->readbuf[this->readbufidx]);
      }
      else {
        this->backBuf[++this->backbufidx] = c;
      }

      this->eof = FALSE;
    }

  void putBack(const char * const str)
    {
      assert(!this->isbinary);

      int n = strlen(str);
      if (!n) return;

      // Decrease line count if we put back any end-of-line
      // characters. This should take care of Unix-, MSDOS/MSWin- and
      // MacOS-style generated files. What a mess.
      for (int i=0; i < n; i++) {
        if ((str[i] == '\r') || ((str[i] == '\n') &&
                                 (this->lastputback != '\r')))
          this->linenr--;
        this->lastputback = str[i];
      }

      this->lastchar = -1;

      if (n <= this->readbufidx && this->backbufidx < 0)
        this->readbufidx -= n;
      else {
        for (int i = n - 1; i >= 0; i--)
          this->backBuf[++this->backbufidx] = str[i];
      }

      this->eof = FALSE;
    }

  SbBool skipWhiteSpace(void)
    {
      const char COMMENT_CHAR = '#';

      while (TRUE) {
        char c;
        SbBool gotchar;
        while ((gotchar = this->get(c)) && isspace(c));

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
  SbBool readHeader(void)
    {
      if (this->headerisread) return this->eof ? FALSE : TRUE;
      this->headerisread = TRUE;

      this->header = "";
      this->ivversion = 0.0f;

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
                               TRUE))
        this->ivversion = 0.0f;
      return TRUE;
    }

  SbBool isMemBuffer(void)
    {
      return this->ismembuffer;
    }

  SbBool isBinary(void)
    {
      return this->isbinary;
    }

  float ivVersion(void)
    {
      return this->ivversion;
    }

  void setIvVersion(const float v)
    {
      this->ivversion = v;
    }

  const SbString & ivHeader(void)
    {
      return this->header;
    }

  const SbString & ivFilename(void)
    {
      return this->filename;
    }

  FILE * ivFilePointer(void)
    {
      return this->fp;
    }

  unsigned int lineNr(void)
    {
      return this->linenr;
    }

  SbBool isEndOfFile(void)
    {
      return this->eof;
    }

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
  SbList<char> backBuf;
  int backbufidx;
  char lastputback; // The last character put back into the stream.
  char lastchar; // Last read character.
  SbBool ismembuffer;
  SbBool headerisread, eof;
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
  Uses the given file pointer as the input file. The input stack of files
  will be emptied first. Closing a file passed in by it's file pointer is
  the library user's responsibility.

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
  else {
    if (!okIfNotFound) {
      SoReadError::post(this, "Couldn't open file '%s' for reading.",
                        fileName);
    }
  }

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
  // This method should be obsoleted or something.. the point being
  // that SoInput is supposed to be an abstraction layer _above_ the
  // "are we using a file or a memory buffer?" inquiries.
  return this->isValidFile();
}

/*!
  Returns file pointer of the file on top of the input stack.

  \sa getCurFileName()
 */
FILE *
SoInput::getCurFile(void) const
{
  return this->getTopOfStack()->ivFilePointer();
}

/*!
  Returns the name of the file on top of the input stack.

  \sa getCurFile()
*/
const char *
SoInput::getCurFileName(void) const
{
  return this->getTopOfStack()->ivFilename().getString();
}

/*!
  Sets up the input stream for reading from a memory buffer. Closes all
  open files in the file stack first.
 */
void
SoInput::setBuffer(void * bufpointer, size_t bufsize)
{
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
  char buffer[32];
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
      else if (isspace(*buf)) {
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

  // FIXME: hack to cast away constness. Ugly. 19990713 mortene.
  if (((SoInput *)this)->isBinary()) {
    str.sprintf("\tOccurred at position %d in binary file %s",
                this->getTopOfStack()->getNumBytesParsedSoFar(),
                this->getCurFileName());
  }
  else {
    str.sprintf("\tOccurred at line %3d in %s",
                this->getTopOfStack()->lineNr(),
                this->getCurFileName());
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
                      SbBool /* addToGlobalDict */)
{
  this->refdict.enter((unsigned long)name.getString(), (void *) base);
}

/*!
  Removes a name<->SoBase mapping from our dictionary.

  \sa findReference(), addReference()
 */
void
SoInput::removeReference(const SbName & name)
{
  this->refdict.remove((unsigned long)name.getString());
}

/*!
  Finds an SoBase derived object which has been mapped to \a name earlier
  during the import process.

  \sa addReference(), removeReference()
 */
SoBase *
SoInput::findReference(const SbName & name) const
{
  void * base;

  if (this->refdict.find((unsigned long)name.getString(), base))
    return (SoBase *) base;

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
  const char * p = getenv(envVarName);
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
  (void)atexit(SoInput::clean);
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
#ifdef _WIN32

  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];

  _splitpath(filename, drive, dir, NULL, NULL);

  SbString s(drive);
  s += dir;
  return s;

#else // UNIX systems

  char * ptr = strrchr(filename, '/');
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
#ifdef _WIN32

  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];

  _splitpath(filename, NULL, NULL, fname, ext);

  SbString s(fname);
  s += ext;
  return s;

#else // UNIX systems

  char * ptr = strrchr(filename, '/');
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
  FIXME: write doc
*/
void
SoInput::initFile(FILE * /* newFP */, const char * /* fileName */,
                  SbString * /* fullName */,
                  SbBool /* openedHere */, SbDict * /* refDict */)
{
  COIN_STUB();
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
  return fi->readHeader() && (!bValidateBufferHeader || fi->ivVersion() != 0.0f);
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

  if (this->getTopOfStack()->ivFilePointer()) {
    const char * filename = this->getTopOfStack()->ivFilename().getString();
    SbString path = SoInput::getPathname(filename);
    if (path.getLength()) SoInput::removeDirectory(path.getString());
  }
  delete this->getTopOfStack();
  this->filestack.remove(0);
  return TRUE;
}

/*!
  FIXME: write doc
 */
size_t
SoInput::freeBytesInBuf(void) const
{
  COIN_STUB();
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
  FIXME: write doc
 */
SbBool
SoInput::makeRoomInBuf(size_t /* nBytes */)
{
  COIN_STUB();
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
  *s = (short)ntohs(*((unsigned short int *)from));
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
  assert(sizeof(int32_t) == sizeof(unsigned long int)); // Fails on 64-bit archs?
  *l = (int32_t)ntohl(*((unsigned long int *)from));
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
  assert(sizeof(unsigned long int) == sizeof(float));
  unsigned long int fbitval = ntohl(*((unsigned long int *)from));
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
  assert(sizeof(unsigned int) * 2 == sizeof(double));
  unsigned long int dbitvals[2] = {
    ntohl(*((unsigned long int *)from)),
    ntohl(*((unsigned long int *)(from + sizeof(double)/2))),
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
  FIXME: write function documentation
*/
SbBool
SoInput::isFileVRML2(void)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoInput::resetFilePointer(FILE * /* fptr */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SbBool
SoInput::isFileURL(const char * /* url */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
char *
SoInput::URLToFile(char * /* out_buf */, const char * /* in_buf */)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write doc
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
  Scan through the include directories in order and try to find a file with
  the given name in any one of them. If file is found, put the full name with
  complete path from current directory of process into \a fullname, open
  the file and return the file pointer.

  If the file could either not be found or not opened for reading, return
  \c NULL.
 */
FILE *
SoInput::findFile(const char * basename, SbString & fullname)
{
  // Try as absolute name first.
  FILE * fp = fopen(basename, "r");
  if (fp) fullname = basename;

  int diridx = 0;
  const SbStringList & sl = SoInput::getDirectories();

  while (!fp && (diridx < sl.getLength())) {
    fullname = * sl[diridx++];
    fullname += "/";
    fullname += basename;

    fp = fopen(fullname.getString(), "r");
  }

  return fp;
}
