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

/*!
  \class SoMField SoMField.h Inventor/fields/SoMField.h
  \brief The SoMField class is the base class for fields which can contain multiple values.
  \ingroup fields

  All field types which may contain more than one member
  value inherits this class. SoMField is an abstract class.

  Use setValue(), setValues() or set1Value() to set the values of
  fields inheriting SoMField, and use getValues() or the index
  operator [] to read values. Example code:

  \code
    SoText2 * textnode = new SoText2;
    textnode->ref();

    // Setting multi-field values. /////////////////////////////

    // Set the first value of the SoMFString field of the SoText2 node.
    // The field array will be truncated to only contain this single value.
    textnode->string.setValue("Morten");
    // Full contents of the SoMFString is: [ "Morten" ]

    // The setValue() method and the = operator is interchangeable,
    // so this code line does the same as the previous line.
    textnode->string = "Peder";
    // Full contents of the SoMFString is now: [ "Peder" ]

    // Set the value at index 2. If the field value array contained
    // less than 3 elements before this call, first expand it to contain
    // 3 elements.
    textnode->string.set1Value(2, "Lars");
    // Full contents of the SoMFString is: [ "Peder", <undefined>, "Lars" ]

    // This sets 3 values of the array, starting at index 5. If the
    // array container had less than 8 elements before the setValues()
    // call, the array will first be expanded.
    SbString s[3] = { "Eriksen", "Blekken", "Aas" };
    textnode->string.setValues(5, sizeof(s), s);
    // Full contents of the SoMFString is now:
    //     [ "Peder", <undefined>, "Lars", <undefined>, <undefined>,
    //       "Eriksen", "Blekken", "Aas" ]

    // Note also that the setValues() call will *not* truncate a field
    // container if you use it to change a subset at the start:
    SbString n[4] = { "Dixon", "Adams", "Bould", "Winterburn" };
    textnode->string.setValues(0, sizeof(n), n);
    // Full contents of the SoMFString is now:
    //     [ "Dixon", "Adams", "Bould", "Winterburn", <undefined>,
    //       "Eriksen", "Blekken", "Aas" ]


    // Inspecting multi-field values. //////////////////////////

    // This will read the second element (counting from zero) if the
    // multivalue field and place it in "val".
    SbString val = textnode->string[2];

    // Gives us a pointer to the array which the multiple-value field
    // is using to store the values. Note that the return value is
    // "const", so you can only read from the array, not write to
    // it.
    const SbString * vals = textnode->string.getValues(0);


    // Modifying multi-field values. ///////////////////////////

    // You can of course modify multifield-values by using the set-
    // and get-methods shown above, but when you're working with
    // big sets of data, this will be ineffective. Then use this
    // technique instead:
    SbString * modvals = textnode->string.startEditing();

    // ... lots of modifications to the "modvals" array here ...

    // Calling the finishEditing() method is necessary for the
    // scene graph to be updated (and re-rendered).
    textnode->string.finishEditing();

  \endcode

  The reason it is more effective to wrap many modifications within
  startEditing() / finishEditing() is because we avoid the stream of
  notification messages which would otherwise be sent for each and
  every modification done. Instead there will just be a single
  notification about the changes, triggered by the finishEditing()
  call.

  The correct manner in which to pre-allocate a specific number of
  field values in one chunk is to use the SoMField::setNum() method,
  for instance in advance of using the startEditing() and
  finishEditing() methods. The field values will be uninitialized
  after expanding the field with the setNum() call.

  Be aware that your application code must be careful to not do silly
  things during a setValues()-triggered notification. If you have code
  that looks for instance like this:

  \code
    // update set of coordinate indices at the start of e.g.
    // an SoIndexedFaceSet's coordIndex field..
    ifs->coordIndex.setValues(0, runner->numIndices, runner->indices);
    // ..then truncate to make sure it's the correct size.
    ifs->coordIndex.setNum(runner->numIndices);
  \endcode

  As setValues() might leave some elements at the end of the array
  that typically can be invalid indices after the first statement is
  executed, something can go wrong during notification if you have
  application code monitoring changes, and the application code then
  for instance triggers an action or something that tries to use the
  coordIndex field before it's updated to it's correct size with the
  setNum() call.

  (Notification can in this case, as always, be temporarily disabled
  to be on the safe side:

  \code
  somefield.enableNotify(FALSE);
  somefield.setValues(...);
  somefield.setNum(...);
  somefield.enableNotify(TRUE);
  somefield.touch();
  \endcode

  This will guarantee that the setValues() and setNum() pair will be
  executed as an atomic operation.)


  When nodes, engines or other types of field containers are written
  to file, their multiple-value fields are written to file in this
  format:

  \code
    containerclass {
      fieldname [ value0, value1, value2, ...]
      ...
    }
  \endcode

  ..like this, for instance, a Coordinate3 node providing 6 vertex
  coordinates in the form of SbVec3f values in its "point" field for
  e.g. a faceset, lineset or pointset:

  \code
     Coordinate3 {
        point [
           -1 1 0, -1 -1 0, 1 -1 0,
           0 2 -1, -2 0 -1, 0 -2 -1,
        ]
     }
  \endcode

  Some fields support application data sharing through a
  setValuesPointer() method. setValuesPointer() makes it possible to
  set the data pointer directly in the field. Normally (when using
  setValues()), Coin makes a copy of your data, so this method can be
  very useful if your application needs the data internally and you're
  just using Coin for the visualization. Example code:

  \code

  myapp->calculateCoordinates(SOME_LARGE_VALUE);
  SbVec3f * mycoords = myapp->getCoordinates();

  SoCoordinate3 * mynode = myapp->getCoordinateNode();
  mynode->point.setValuesPointer(SOME_LARGE_VALUE, mycoords);

  \endcode

  Be aware that your field should be a read-only field when you set
  the data like this. If you write to the field, the values in your
  application array will be overwritten. If you append values to the
  field, a new array will be allocated, and the data will be copied
  into it before appending the new values. The array pointer will then
  be discarded.

  Also note that whenever you change some value(s) in the array, you
  must manually notify Coin about this by calling
  SoField::touch(). For our example:

  \code

  SbVec3f * mycoords = myapp->getCoordinate();
  myapp->updateCoordinate(mycoords);
  SoCoordinate3 * mynode = myapp->getCoordinateNode();
  mynode->point.touch(); // this will notify Coin that field has changed

  \endcode

  You can use SoMField::enableDeleteValues() to make Coin delete the
  array for you when the field is destructed or the array pointer is
  discarded because it isn't needed anymore (e.g. when the array size
  is changed). The array will be deleted using the C++ \e delete[]
  operator, so if you use it, your array must be allocated using the
  C++ \e new[] operator.

  SoMField::enableDeleteValues() is supported only to be compatible
  with later versions of TGS Inventor and we don't recommend using
  it. It can have undefined results on the Microsoft Windows
  platform. Allocating memory in the application and destructing it in
  a DLL can be a bad thing, causing mysterious crashes, if you're not
  very careful about how your application and DLLs are linked to the
  underlying C library.

  \sa SoSField
*/

/*!
  \fn int SoMField::getNum(void) const

  Returns number of values in this field.
*/

#include <Inventor/fields/SoMField.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoSubField.h>
#include <../tidbits.h> // coin_atexit()
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <Inventor/C/threads/threadsutilp.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var int SoMField::num
  Number of available values.
*/
/*!
  \var int SoMField::maxNum
  Number of array "slots" allocated for this field.
*/
/*!
  \var SbBool SoMField::userDataIsUsed
  Is \c TRUE if data has been set through a setValuesPointer() call
  and set to \c FALSE through a enableDeleteValues() call.
*/

// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoMField::classTypeId;

// need one static mutex for field_buffer in SoMField::get1(SbString &)
static void * somfield_mutex = NULL;

static void
somfield_mutex_cleanup(void)
{
  CC_MUTEX_DESTRUCT(somfield_mutex);
}

// Overridden from parent class.
SoType
SoMField::getClassTypeId(void)
{
  return SoMField::classTypeId;
}

// Overridden from parent class.
void
SoMField::initClass(void)
{
  PRIVATE_FIELD_INIT_CLASS(SoMField, "MField", inherited, NULL);

  CC_MUTEX_CONSTRUCT(somfield_mutex);
  coin_atexit((coin_atexit_f*) somfield_mutex_cleanup);
}

/*!
  Constructor. Initializes number of values in field to zero.
*/
SoMField::SoMField(void)
{
  this->maxNum = this->num = 0;
  this->userDataIsUsed = FALSE;
}

/*!
  Destructor in SoMField does nothing. Resource deallocation needs to
  be done from subclasses.
*/
SoMField::~SoMField()
{
}

/*!
  Make room in the field to store \a newnum values.
*/
void
SoMField::makeRoom(int newnum)
{
  assert(newnum >= 0);
  if (newnum != this->num) this->allocValues(newnum);
}

/*!
  Set the value at \a index to the value contained in \a valuestring.
  Returns \c TRUE if a valid value for this field can be extracted
  from \a valuestring, otherwise \c FALSE.
*/
SbBool
SoMField::set1(const int index, const char * const valuestring)
{
  SoInput in;
  in.setBuffer((void *)valuestring, strlen(valuestring));
  if (!this->read1Value(&in, index)) return FALSE;
  this->valueChanged();
  return TRUE;
}

static void * mfield_buffer = NULL;
static size_t mfield_buffer_size = 0;

static void
mfield_buffer_cleanup(void)
{
  if (mfield_buffer) {
    free(mfield_buffer);
    mfield_buffer = NULL;
    mfield_buffer_size = 0;
  }
}

static void *
mfield_buffer_realloc(void * bufptr, size_t size)
{
  void * newbuf = realloc(bufptr, size);
  mfield_buffer = newbuf;
  mfield_buffer_size = size;
  return newbuf;
}

/*!
  Return the value at \a index in the \a valuestring string.
*/
void
SoMField::get1(const int index, SbString & valuestring)
{
  CC_MUTEX_LOCK(somfield_mutex); // need to lock since a static array is used

  // Note: this code has an almost verbatim copy in SoField::get(), so
  // remember to update both places if any fixes are done.

  // Initial buffer setup.
  SoOutput out;
  const size_t STARTSIZE = 32;
  // if buffer grow bigger than 1024 bytes, free memory
  // at end of method. Otherwise, just keep using the allocated
  // memory the next time this method is called.
  const size_t MAXSIZE = 1024;

  if (mfield_buffer_size < STARTSIZE) {
    mfield_buffer = malloc(STARTSIZE);
    mfield_buffer_size = STARTSIZE;
    coin_atexit((coin_atexit_f *)mfield_buffer_cleanup);
  }

  out.setBuffer(mfield_buffer, mfield_buffer_size,
                mfield_buffer_realloc);

  // Record offset to skip header.
  out.write("");
  size_t offset;
  void * buffer;
  out.getBuffer(buffer, offset);

  // Write field..
  this->write1Value(&out, index);
  // ..then read it back into the SbString.
  size_t size;
  out.getBuffer(buffer, size);
  valuestring = ((char *)buffer) + offset;

  // check if buffer grew too big
  if (mfield_buffer_size >= MAXSIZE) {
    // go back to startsize
    (void) mfield_buffer_realloc(mfield_buffer, STARTSIZE);
  }
  CC_MUTEX_UNLOCK(somfield_mutex);
}

/*!
  Read and set all values for this field from input stream \a in.
  Returns \c TRUE if import went ok, otherwise \c FALSE.
*/
SbBool
SoMField::readValue(SoInput * in)
{
  // FIXME: temporary disable notification (if on) during reading the
  // field elements. 20000429 mortene.

  // This macro is convenient for reading with error detection.
#define READ_VAL(val) \
  if (!in->read(val)) { \
    SoReadError::post(in, "Premature end of file"); \
    return FALSE; \
  }


  if (in->isBinary()) {
    int numtoread;
    READ_VAL(numtoread);

    // Sanity checking on the value, to avoid barfing on corrupt
    // files.
    if (numtoread < 0) {
      SoReadError::post(in, "invalid number of values in field: %d",
                        numtoread);
      return FALSE;
    }
#if 0 // tmp disabled until we come up with something better
    // FIXME: this limit is way too low. Not sure if a limit is a good
    // thing at all. 20000405 mortene.
    else if (numtoread > 32768) {
      SoReadError::post(in, "%d values in field, file probably corrupt",
                        numtoread);
      return FALSE;
    }
#endif // disabled

    this->makeRoom(numtoread);
    return this->readBinaryValues(in, numtoread);
  }

  char c;
  READ_VAL(c);
  if (c == '[') {
    int currentidx = 0;

    READ_VAL(c);
    if (c == ']') {
      // Zero values -- done. :^)
    }
    else {
      in->putBack(c);

      while (TRUE) {
        // makeRoom() makes sure the allocation strategy is decent.
        if (currentidx >= this->num) this->makeRoom(currentidx + 1);

        if (!this->read1Value(in, currentidx++)) return FALSE;

        READ_VAL(c);
        if (c == ',') { READ_VAL(c); } // Treat trailing comma as whitespace.

        // That was the last array element, we're done.
        if (c == ']') { break; }

        if (c == '}') {
          SoReadError::post(in, "Premature end of array, got '%c'", c);
          return FALSE;
        }

        in->putBack(c);
      }
    }

    // Fit array to number of items.
    this->makeRoom(currentidx);
  }
  else {
    in->putBack(c);
    this->makeRoom(1);
    if (!this->read1Value(in, 0)) return FALSE;
  }

#undef READ_VAL

  return TRUE;
}

/*!
  Write all field values to \a out.
*/
void
SoMField::writeValue(SoOutput * out) const
{
  if (out->isBinary()) {
    this->writeBinaryValues(out);
    return;
  }

  const int num = this->getNum();
  if ((num > 1) || (num == 0)) out->write("[ ");

  out->incrementIndent();

  for (int i=0; i < num; i++) {
    this->write1Value(out, i);

    if (i != num-1) {
      if (((i+1) % this->getNumValuesPerLine()) == 0) {
        out->write(",\n");
        out->indent();
        // for alignment
        out->write("  ");
      }
      else {
        out->write(", ");
      }
    }
  }
  if ((num > 1) || (num == 0)) out->write(" ]");

  out->decrementIndent();
}

/*!
  Read \a num binary format values from \a in into this field.
*/
SbBool
SoMField::readBinaryValues(SoInput * in, int num)
{
  assert(in->isBinary());
  assert(num >= 0);

  for (int i=0; i < num; i++) if (!this->read1Value(in, i)) return FALSE;
  return TRUE;
}

/*!
  Write all values of field to \a out in binary format.
*/
void
SoMField::writeBinaryValues(SoOutput * out) const
{
  assert(out->isBinary());

  const int num = this->getNum();
  out->write(num);
  for (int i=0; i < num; i++) this->write1Value(out, i);
}

// Number of values written to each line during export to ASCII format
// files. Override this in subclasses for prettier formating.
int
SoMField::getNumValuesPerLine(void) const
{
  return 1;
}

/*!
  Set number of values to \a num.

  If the current number of values is larger than \a num, the array of
  values will be truncated from the end. But if \a num is larger, the
  array will automatically be expanded (and random values will be set
  for the new array items).
 */
void
SoMField::setNum(const int num)
{
  // Don't use getNum(), as that could trigger evaluate(), which is
  // _not_ supposed to be called recursively (which means setNum()
  // wouldn't have been available from within an evaluate() session).
  int oldnum = this->num;

  // Note: this method is implemented in terms of the virtual methods
  // deleteValues() and insertSpace() so the more "complex" fields
  // (like SoMFNode and SoMFEngine) can also handle setNum() in a
  // correct way.

  if (num < oldnum) {
    this->deleteValues(num, -1);
    // deleteValues() also handles notification.
  }
  else if (num > oldnum) {
    this->insertSpace(oldnum, num - oldnum);
    // insertSpace() also handles notification.
  }
  // else no change.
}

/*!
  Remove value elements from index \a start up to and including index
  \a start + \a num - 1.

  Elements with indices larger than the last deleted element will
  be moved downwards in the value array.

  If \a num equals -1, delete from index \a start and to the end of
  the array.
*/
void
SoMField::deleteValues(int start, int num)
{
  // Note: this function is overridden in SoMFNode, SoMFEngine and
  // SoMFPath, so if you do any changes here, take a look at those
  // methods aswell (they are collected in the common template
  // MFNodeEnginePath.tpl).

  // Don't use getNum(), so we avoid recursive evaluate() calls.
  int oldnum = this->num;

  if (num == -1) num = oldnum - start;
  if (num == 0) return;
  int end = start + num; // First element behind the delete block.

#if COIN_DEBUG
  if (start < 0 || start >= oldnum || end > oldnum || num < -1) {
    SoDebugError::post("SoMField::deleteValues",
                       "invalid indices [%d, %d] for array of size %d",
                       start, end - 1, oldnum);
    return;
  }
#endif // COIN_DEBUG

  // Move elements downward to fill the gap.
  for (int i = 0; i < oldnum-(start+num); i++)
    this->copyValue(start+i, start+num+i);

  // Truncate array.
  this->allocValues(oldnum - num);

  // Send notification.
  this->valueChanged();
}

/*!
  Can be used to make Coin delete the array pointer set through
  a setValuesPointer() call. See SoMField documentation for
  information about the setValuesPointer() function.

  This method is a TGS extension (introduced in TGS OIV v3.0) and is
  supported only for compatibility. We suggest that you don't use it
  since it can lead to hard-to-find bugs.

  \since 2002-02-08
*/
void
SoMField::enableDeleteValues(void)
{
  this->userDataIsUsed = FALSE;
}

/*!
  Returns whether SoMField::enableDeleteValues() has been
  called on a field. The result is only valid if setValuesPointer()
  has been called on the field first.

  This method is a TGS extension (introduced in TGS OIV v3.0) and is
  supported only for compatibility. We suggest that you don't use it
  since it can lead to hard-to-find bugs.

  \since 2002-02-08
*/
SbBool
SoMField::isDeleteValuesEnabled(void)
{
  return !this->userDataIsUsed;
}

/*!
  Insert \a num "slots" for new value elements from \a start.
  The elements already present from \a start will be moved
  "upward" in the extended array.
*/
void
SoMField::insertSpace(int start, int num)
{
  if (num == 0) return;

  // Don't use getNum(), so we avoid recursive evaluate() calls.
  int oldnum = this->num;
#if COIN_DEBUG
  if (start < 0 || start > oldnum || num < 0) {
    SoDebugError::post("SoMField::insertSpace",
                       "invalid indices [%d, %d] for array of size %d",
                       start, start + num, oldnum);
    return;
  }
#endif // COIN_DEBUG

  // Expand array.
  this->allocValues(oldnum + num);

  // Copy values upward.
  for (int i = oldnum - start - 1; i >= 0; i--) {
    this->copyValue(start+num+i, start+i);
  }

  // Send notification.
  this->valueChanged();
}

#ifndef DOXYGEN_SKIP_THIS // Internal method.
void
SoMField::allocValues(int newnum)
{
  // Important notice: the "non-realloc"-version of this method is
  // found in SoSubField.h. If you make modifications here, do check
  // whether or not they should be matched with modifications in that
  // method aswell.

  assert(newnum >= 0);

  if (newnum == 0) {
    if (!this->userDataIsUsed) {
      delete[] (unsigned char *) this->valuesPtr();
    }
    this->setValuesPtr(NULL);
    this->userDataIsUsed = FALSE;
    this->maxNum = 0;
  }
  else if (newnum > this->maxNum || newnum < this->num) {
    int fsize = this->fieldSizeof();
    if (this->valuesPtr()) {

      // Allocation strategy is to repeatedly double the size of the
      // allocated block until it will at least match the requested
      // size.  (Unless the requested size is less than what we've
      // got, then we'll repeatedly halve the allocation size.)
      //
      // I think this will handle both cases quite gracefully: 1)
      // newnum > this->maxNum, 2) newnum < num
      int oldmaxnum = this->maxNum;
      while (newnum > this->maxNum) this->maxNum *= 2;
      while ((this->maxNum / 2) >= newnum) this->maxNum /= 2;

#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoMField::allocValues",
                             "'%s' newnum==%d, old/new %p->maxNum==%d/%d",
                             this->getTypeId().getName().getString(),
                             newnum, this, oldmaxnum, this->maxNum);

#endif // debug

      if (oldmaxnum != this->maxNum) {
        // FIXME: Umm.. aren't we supposed to use realloc() here?
        // 20000915 mortene.
        unsigned char * newblock = new unsigned char[this->maxNum * fsize];
        int copysize = fsize * SbMin(this->num, newnum);
        (void) memcpy(newblock, this->valuesPtr(), copysize);
        // we have to dereference old values in SoMFNode, SoMFPath and
        // SoMFEngine, so we just initialize the part of the array
        // with no defined values to NULL.
        int rest = this->maxNum*fsize - copysize;
        if (rest > 0) {
          (void)memset(newblock + copysize, 0, rest);
        }
        if (!this->userDataIsUsed) {
          delete[] (unsigned char *) this->valuesPtr();
        }
        this->setValuesPtr(newblock);
        this->userDataIsUsed = FALSE;
      }
    }
    else {
      unsigned char * data = new unsigned char[newnum * fsize];
      // we have to dereference old values in SoMFNode, SoMFPath and
      // SoMFEngine, so we just initialize the array to NULL.
      (void)memset(data, 0, newnum * fsize);
      this->setValuesPtr(data);
      this->userDataIsUsed = FALSE;
      this->maxNum = newnum;
    }
  }

  this->num = newnum;
}
#endif // DOXYGEN_SKIP_THIS
