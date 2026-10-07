/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <Inventor/lists/SbList.h>

/*!
  \class SbList SbList.h Inventor/lists/SbList.h
  \brief The SbList class is a template container class for lists.

  \ingroup coin_base

  SbList is an extension of the Coin library versus the original Open
  Inventor API. Open Inventor handles most list classes by inheriting
  the SbPList class, which contains an array of generic \c void*
  pointers. By using this template-based class instead, we can share
  more code and make the list handling code more typesafe.

  Care has been taken to make sure the list classes which are part of
  the Open Inventor API to still be compatible with their original
  interfaces, as derived from the SbPList base class. But if you still
  bump into any problems when porting your Open Inventor applications,
  let us know and we'll do our best to sort them out.

  A feature with this class is that the list object arrays grow
  dynamically as you append() more items to the list.  The actual
  growing technique used is to double the list size when it becomes
  too small.

  There are also other array-related convenience methods; e.g. finding
  item indices, inserting items at any position, removing items (and
  shrink the array), copying of arrays, etc.

  \sa SbPList
*/


// FIXME: all methods on this class is now inlined. This probably adds
// quite a few (hundred) kBytes to the total size of the
// library. Several methods on this class should therefore be
// "de-inlined". The problem with this is that compilers seems to
// differ on whether or not subclasses or template instances then need
// to explicitly "declare themselves".  This is not too hard to fix,
// but it involves _some_ pain as it needs some nifty configure
// checking. 20000227 mortene.


/*!
  \fn SbList<Type>::SbList(const int sizehint)

  Default constructor.

  The \a sizehint argument hints about how many elements the list will
  contain, so memory allocation can be done efficiently.

  Important note: explicitly specifying an \a sizehint value does \e
  not mean that the list will initially contain this number of values.
  After construction, the list will contain zero items, just as for
  the default constructor. Here's a good example on how to give
  yourself hard to find bugs:

  \code
  SbList<SbBool> flags(2); // Assume we need only 2 elements. Note
                           // that the list is still 0 elements long.
  flags[0] = TRUE;         // Throws: list is still 0 elements long.
  \endcode

  Since this conceptual misunderstanding is so easy to make, you're
  probably better (or at least safer) off leaving the \a sizehint
  argument to its default value by not explicitly specifying it.

  It improves performance if you know the approximate total size of
  the list in advance before adding list elements, as the number of
  reallocations will be minimized.
 */

/*!
  \fn SbList<Type>::SbList(const SbList<Type> & l)

  Copy constructor. Creates a complete copy of the given list.
 */

/*!
\fn SbList<Type>::~SbList()

  Destructor, frees all internal resources used by the list container.
*/

/*!
  \fn void SbList<Type>::copy(const SbList<Type> & l)

  Make this list a copy of \a l.

  If allocating or copying the new storage throws, newly allocated storage is
  released. When reallocation is required, this list is left unchanged.
 */

/*!
  \fn SbList<Type> & SbList<Type>::operator=(const SbList<Type> & l)

  Make this list a copy of \a l.
 */

/*!
  \fn void SbList<Type>::fit(void)

  Fit the allocated array exactly around the length of the list,
  discarding memory spent on unused pre-allocated array cells.

  You should normally not need or want to call this method, and it is
  only available for the sake of having the option to optimize memory
  usage for the unlikely event that you should throw around huge
  SbList objects within your application.

  If allocating or copying the compact storage throws, the original storage
  remains active.
 */

/*!
  \fn void SbList<Type>::append(const Type item)

  Append the \a item at the end of list, expanding the list array by
  one.
 */

/*!
  \fn int SbList<Type>::find(const Type item) const

  Return index of first occurrence of \a item in the list, or -1 if \a
  item is not present.
*/

/*!
  \fn void SbList<Type>::insert(const Type item, const int insertbefore)

  Insert \a item at index \a insertbefore.

  \a insertbefore may equal, but must not be larger than, the current number
  of items in the list. Throws \c std::out_of_range for an invalid index.
 */


/*!
  \fn void SbList<Type>::removeItem(const Type item)

  Removes an \a item from the list. If there are several items with
  the same value, removes the \a item with the lowest index.

  If \a item is not present, the list is left unchanged. Builds with
  COIN_EXTRA_DEBUG enabled also report the invalid removal with an assertion.
*/

/*!
  \fn void SbList<Type>::remove(const int index)

  Remove the item at \a index, moving all subsequent items downwards
  one place in the list. Throws \c std::out_of_range for an invalid index.
*/

/*!
  \fn void SbList<Type>::removeFast(const int index)

  Remove the item at \a index, moving the last item into its place and
  truncating the list. Throws \c std::out_of_range for an invalid index.
*/

/*!
  \fn int SbList<Type>::getLength(void) const

  Returns number of items in the list.
*/

/*!
 \fn void SbList<Type>::truncate(const int length, const int fit)

 Shorten the list to contain \a length elements, removing items from
 \e index \a length and onwards.

 If \a fit is non-zero, will also shrink the internal size of the
 allocated array. Note that this is much less efficient than not
 re-fitting the array size.

 Throws \c std::out_of_range if \a length is negative or larger than the
 current list length.
*/

/*!
  \fn void SbList<Type>::push(const Type item)

  This appends \a item at the end of the list in the same fashion as
  append() does. Provided as an abstraction for using the list class
  as a stack.
*/

/*!
  \fn Type SbList<Type>::pop(void)

  Pops off the last element of the list and returns it.

  Throws \c std::out_of_range if the list is empty.
*/

/*!
  \fn const Type * SbList<Type>::getArrayPtr(const int start = 0) const

  Returns pointer to a non-modifiable array of the lists elements.
  \a start specifies an index into the array.

  The caller is \e not responsible for freeing up the array, as it is
  just a pointer into the internal array used by the list.

  For an empty list, the default \a start of zero returns the internal empty
  array view. Other invalid start indices throw \c std::out_of_range.
*/

/*!
  \fn Type SbList<Type>::operator[](const int index) const

  Returns a copy of item at \a index.

  Throws \c std::out_of_range for an invalid index.
*/

/*!
  \fn Type & SbList<Type>::operator[](const int index)

  Returns a reference to item at \a index.

  Throws \c std::out_of_range for an invalid index.
*/

/*!
  \fn SbBool SbList<Type>::operator==(const SbList<Type> & l) const

  Equality operator. Returns \c TRUE if this list and \a l are
  identical, containing the exact same set of elements.
*/

/*!
  \fn SbBool SbList<Type>::operator!=(const SbList<Type> & l) const

  Inequality operator. Returns \c TRUE if this list and \a l are not
  equal.
*/

/*!
  \fn void SbList<Type>::expand(const int size)

  Expand the list to contain \a size items. The new items added at the
  end have undefined value.

  Throws \c std::out_of_range if \a size is negative.
*/

/*!
  \fn int SbList<Type>::getArraySize(void) const

  Return number of items there's allocated space for in the array.

  \sa getLength()
*/

/*!
  \fn void SbList<Type>::ensureCapacity(const int size)

  Ensure that the internal buffer can hold at least \a size
  elements. SbList will automatically resize itself to make room for
  new elements, but this method can be used to improve performance
  (and avoid memory fragmentation) if you know approximately the
  number of elements that is going to be added to the list.
  
  \since Coin 2.5
*/

#ifdef COIN_TEST_SUITE

#include <climits>
#include <stdexcept>
#include <vector>

// Directed characterization coverage for SbList's typed-value contract.
// These tests intentionally precede the implementation changes they specify.

class SbListTestAccess : public SbList<int> {
public:
  SbListTestAccess(const int sizehint = 4) : SbList<int>(sizehint) { }

  int capacity(void) const { return this->getArraySize(); }
  void resize(const int size) { this->expand(size); }
};

static void
sblist_check_model(const SbListTestAccess & list,
                   const std::vector<int> & model)
{
  BOOST_REQUIRE_EQUAL(list.getLength(), static_cast<int>(model.size()));
  BOOST_CHECK(list.capacity() >= list.getLength());
  for (size_t i = 0; i < model.size(); i++) {
    BOOST_CHECK_EQUAL(list[static_cast<int>(i)], model[i]);
  }
}

class SbListThrowingValue {
public:
  SbListThrowingValue(const int v = 0) : value(v) { ++livecount; }
  SbListThrowingValue(const SbListThrowingValue & other)
    : value(other.value) { ++livecount; }
  ~SbListThrowingValue() { --livecount; }

  SbListThrowingValue & operator=(const SbListThrowingValue & other) {
    if (assignmentsbeforethrow == 0) {
      throw std::runtime_error("injected SbList value assignment failure");
    }
    if (assignmentsbeforethrow > 0) --assignmentsbeforethrow;
    this->value = other.value;
    return *this;
  }

  bool operator==(const SbListThrowingValue & other) const {
    return this->value == other.value;
  }
  bool operator!=(const SbListThrowingValue & other) const {
    return !(*this == other);
  }

  static void throwAfter(const int assignments) {
    assignmentsbeforethrow = assignments;
  }
  static void disableFailure(void) { assignmentsbeforethrow = -1; }
  static int live(void) { return livecount; }

  int value;

private:
  static int assignmentsbeforethrow;
  static int livecount;
};

int SbListThrowingValue::assignmentsbeforethrow = -1;
int SbListThrowingValue::livecount = 0;

class SbListThrowingTestAccess : public SbList<SbListThrowingValue> {
public:
  using SbList<SbListThrowingValue>::operator=;
  int capacity(void) const { return this->getArraySize(); }
};

BOOST_AUTO_TEST_CASE(default_storage_append_and_array_contract)
{
  SbListTestAccess list;
  BOOST_CHECK_EQUAL(list.getLength(), 0);
  BOOST_CHECK_EQUAL(list.capacity(), 4);
  BOOST_CHECK(list.getArrayPtr(0) != NULL);

  const int * initial = list.getArrayPtr();
  for (int i = 0; i < 4; i++) {
    list.append(i * 3);
    BOOST_CHECK(list.getArrayPtr() == initial);
  }
  list.append(12);

  BOOST_CHECK_EQUAL(list.getLength(), 5);
  BOOST_CHECK_EQUAL(list.capacity(), 8);
  BOOST_CHECK(list.getArrayPtr() != initial);
  for (int i = 0; i < 5; i++) BOOST_CHECK_EQUAL(list[i], i * 3);
}

BOOST_AUTO_TEST_CASE(insert_find_and_removal_contracts)
{
  SbList<int> list;
  list.append(10);
  list.append(20);
  list.append(20);
  list.insert(15, 1);
  list.insert(30, list.getLength());

  BOOST_REQUIRE_EQUAL(list.getLength(), 5);
  BOOST_CHECK_EQUAL(list.find(20), 2);
  BOOST_CHECK_EQUAL(list.find(99), -1);

  list.removeItem(20);
  BOOST_CHECK_EQUAL(list[2], 20);
  list.remove(1);
  BOOST_CHECK_EQUAL(list[1], 20);
  list.removeFast(0);

  BOOST_REQUIRE_EQUAL(list.getLength(), 2);
  BOOST_CHECK_EQUAL(list[0], 30);
  BOOST_CHECK_EQUAL(list[1], 20);
}

BOOST_AUTO_TEST_CASE(remove_missing_item_is_a_noop)
{
  SbList<int> list;
  list.append(10);
  list.append(20);
  list.removeItem(99);

  BOOST_REQUIRE_EQUAL(list.getLength(), 2);
  BOOST_CHECK_EQUAL(list[0], 10);
  BOOST_CHECK_EQUAL(list[1], 20);

  SbList<int> empty;
  empty.removeItem(99);
  BOOST_CHECK_EQUAL(empty.getLength(), 0);
}

BOOST_AUTO_TEST_CASE(copy_assignment_and_self_assignment_are_independent)
{
  SbList<int> original;
  original.append(1);
  original.append(2);

  SbList<int> copy(original);
  SbList<int> assigned;
  assigned = original;
  assigned = assigned;

  original[0] = 9;
  BOOST_CHECK_EQUAL(copy[0], 1);
  BOOST_CHECK_EQUAL(assigned[0], 1);
  BOOST_CHECK(copy == assigned);
  BOOST_CHECK(copy != original);
}

BOOST_AUTO_TEST_CASE(size_hints_capacity_and_fit_contract)
{
  const int hints[] = { INT_MIN, -1, 0, 1, 4, 5, 9 };
  const int capacities[] = { 4, 4, 4, 4, 4, 5, 9 };
  for (size_t i = 0; i < sizeof(hints) / sizeof(hints[0]); i++) {
    SbListTestAccess list(hints[i]);
    BOOST_CHECK_EQUAL(list.getLength(), 0);
    BOOST_CHECK_EQUAL(list.capacity(), capacities[i]);
  }

  SbListTestAccess list;
  for (int i = 0; i < 9; i++) list.append(i);
  BOOST_CHECK_EQUAL(list.capacity(), 16);
  list.truncate(6, TRUE);
  BOOST_CHECK_EQUAL(list.capacity(), 6);
  list.truncate(3, TRUE);
  BOOST_CHECK_EQUAL(list.capacity(), 4);
  for (int i = 0; i < 3; i++) BOOST_CHECK_EQUAL(list[i], i);
}

BOOST_AUTO_TEST_CASE(ensure_capacity_reserves_without_changing_length)
{
  SbListTestAccess list;
  list.append(7);
  const int * initial = list.getArrayPtr();

  list.ensureCapacity(3);
  BOOST_CHECK(list.getArrayPtr() == initial);
  list.ensureCapacity(17);

  BOOST_CHECK_EQUAL(list.getLength(), 1);
  BOOST_CHECK_EQUAL(list.capacity(), 17);
  BOOST_CHECK_EQUAL(list[0], 7);
}

BOOST_AUTO_TEST_CASE(repeated_fit_and_regrow_cycles_preserve_values)
{
  SbListTestAccess list;
  for (int round = 0; round < 8; round++) {
    list.truncate(0, TRUE);
    BOOST_CHECK_EQUAL(list.capacity(), 4);
    for (int i = 0; i < 9; i++) list.append(i);
    BOOST_CHECK_EQUAL(list.capacity(), 16);
    list.truncate(5, TRUE);
    BOOST_CHECK_EQUAL(list.capacity(), 5);
    list.append(9);
    BOOST_CHECK_EQUAL(list.capacity(), 10);
    list.truncate(3, TRUE);
    for (int i = 0; i < 3; i++) BOOST_CHECK_EQUAL(list[i], i);
  }
}

BOOST_AUTO_TEST_CASE(valid_operations_match_vector_model)
{
  SbListTestAccess list;
  std::vector<int> model;
  unsigned int randomstate = 0x51b1157U;

  for (int step = 0; step < 4000; step++) {
    randomstate = randomstate * 1664525U + 1013904223U;
    unsigned int operation = (randomstate >> 16) % 9U;
    const int value = static_cast<int>((randomstate >> 8) % 1000U);

    if (model.empty()) operation = 0;
    if (model.size() > 192 && (operation == 0 || operation == 1)) operation = 4;

    switch (operation) {
    case 0:
      list.append(value);
      model.push_back(value);
      break;
    case 1: {
      const int index = static_cast<int>(randomstate % (model.size() + 1));
      list.insert(value, index);
      model.insert(model.begin() + index, value);
      break;
    }
    case 2: {
      const int index = static_cast<int>(randomstate % model.size());
      list.remove(index);
      model.erase(model.begin() + index);
      break;
    }
    case 3: {
      const int index = static_cast<int>(randomstate % model.size());
      list.removeFast(index);
      model[index] = model.back();
      model.pop_back();
      break;
    }
    case 4: {
      const int length = static_cast<int>(randomstate % (model.size() + 1));
      list.truncate(length, (randomstate >> 31) != 0);
      model.resize(length);
      break;
    }
    case 5: {
      const int index = static_cast<int>(randomstate % model.size());
      list[index] = value;
      model[index] = value;
      break;
    }
    case 6:
      list.fit();
      break;
    case 7:
      list.ensureCapacity(static_cast<int>(model.size()) + 13);
      break;
    case 8: {
      SbList<int> copy(list);
      SbList<int> assigned;
      assigned = list;
      BOOST_CHECK(copy == assigned);
      BOOST_CHECK(copy == list);
      break;
    }
    }

    sblist_check_model(list, model);
  }
}

BOOST_AUTO_TEST_CASE(invalid_index_operations_throw_and_preserve_state)
{
  SbList<int> list;
  list.append(10);
  list.append(20);
  const SbList<int> & constlist = list;

  BOOST_REQUIRE_THROW(list[-1], std::out_of_range);
  BOOST_REQUIRE_THROW(list[2], std::out_of_range);
  BOOST_REQUIRE_THROW(constlist[-1], std::out_of_range);
  BOOST_REQUIRE_THROW(constlist[2], std::out_of_range);
  BOOST_REQUIRE_THROW(list.getArrayPtr(-1), std::out_of_range);
  BOOST_REQUIRE_THROW(list.getArrayPtr(2), std::out_of_range);
  BOOST_REQUIRE_THROW(list.insert(30, -1), std::out_of_range);
  BOOST_REQUIRE_THROW(list.insert(30, 3), std::out_of_range);
  BOOST_REQUIRE_THROW(list.remove(-1), std::out_of_range);
  BOOST_REQUIRE_THROW(list.remove(2), std::out_of_range);
  BOOST_REQUIRE_THROW(list.removeFast(-1), std::out_of_range);
  BOOST_REQUIRE_THROW(list.removeFast(2), std::out_of_range);
  BOOST_REQUIRE_THROW(list.truncate(-1), std::out_of_range);
  BOOST_REQUIRE_THROW(list.truncate(3), std::out_of_range);

  BOOST_REQUIRE_EQUAL(list.getLength(), 2);
  BOOST_CHECK_EQUAL(list[0], 10);
  BOOST_CHECK_EQUAL(list[1], 20);

  SbList<int> empty;
  BOOST_REQUIRE_THROW(empty.pop(), std::out_of_range);
  BOOST_CHECK(empty.getArrayPtr(0) != NULL);

  SbListTestAccess access;
  BOOST_REQUIRE_THROW(access.resize(-1), std::out_of_range);
  BOOST_CHECK_EQUAL(access.getLength(), 0);
}

BOOST_AUTO_TEST_CASE(growth_assignment_failure_preserves_storage_and_values)
{
  SbListThrowingTestAccess list;
  for (int i = 0; i < 4; i++) list.append(SbListThrowingValue(i + 1));
  const SbListThrowingValue * oldarray = list.getArrayPtr();
  const int oldlive = SbListThrowingValue::live();

  bool threw = false;
  SbListThrowingValue::throwAfter(0);
  try { list.append(SbListThrowingValue(5)); }
  catch (const std::runtime_error &) { threw = true; }
  SbListThrowingValue::disableFailure();

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(SbListThrowingValue::live(), oldlive);
  BOOST_CHECK_EQUAL(list.getLength(), 4);
  BOOST_CHECK_EQUAL(list.capacity(), 4);
  BOOST_CHECK(list.getArrayPtr() == oldarray);
  for (int i = 0; i < 4; i++) BOOST_CHECK_EQUAL(list[i].value, i + 1);
}

BOOST_AUTO_TEST_CASE(fit_assignment_failure_does_not_leak_or_publish_buffer)
{
  SbListThrowingTestAccess list;
  for (int i = 0; i < 9; i++) list.append(SbListThrowingValue(i + 1));
  list.truncate(6);
  const SbListThrowingValue * oldarray = list.getArrayPtr();
  const int oldlive = SbListThrowingValue::live();

  bool threw = false;
  SbListThrowingValue::throwAfter(0);
  try { list.fit(); }
  catch (const std::runtime_error &) { threw = true; }
  SbListThrowingValue::disableFailure();

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(SbListThrowingValue::live(), oldlive);
  BOOST_CHECK_EQUAL(list.getLength(), 6);
  BOOST_CHECK_EQUAL(list.capacity(), 16);
  BOOST_CHECK(list.getArrayPtr() == oldarray);
  for (int i = 0; i < 6; i++) BOOST_CHECK_EQUAL(list[i].value, i + 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor_assignment_failure_does_not_leak)
{
  SbList<SbListThrowingValue> source;
  for (int i = 0; i < 5; i++) source.append(SbListThrowingValue(i + 1));
  const int oldlive = SbListThrowingValue::live();

  bool threw = false;
  SbListThrowingValue::throwAfter(0);
  try { SbList<SbListThrowingValue> copy(source); }
  catch (const std::runtime_error &) { threw = true; }
  SbListThrowingValue::disableFailure();

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(SbListThrowingValue::live(), oldlive);
  BOOST_REQUIRE_EQUAL(source.getLength(), 5);
  for (int i = 0; i < 5; i++) BOOST_CHECK_EQUAL(source[i].value, i + 1);
}

BOOST_AUTO_TEST_CASE(copy_reallocation_failure_preserves_destination)
{
  SbList<SbListThrowingValue> source;
  for (int i = 0; i < 5; i++) source.append(SbListThrowingValue(i + 10));
  SbListThrowingTestAccess destination;
  destination.append(SbListThrowingValue(1));
  destination.append(SbListThrowingValue(2));
  const SbListThrowingValue * oldarray = destination.getArrayPtr();
  const int oldlive = SbListThrowingValue::live();

  bool threw = false;
  SbListThrowingValue::throwAfter(2);
  try { destination = source; }
  catch (const std::runtime_error &) { threw = true; }
  SbListThrowingValue::disableFailure();

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(SbListThrowingValue::live(), oldlive);
  BOOST_CHECK_EQUAL(destination.getLength(), 2);
  BOOST_CHECK_EQUAL(destination.capacity(), 4);
  BOOST_CHECK(destination.getArrayPtr() == oldarray);
  BOOST_CHECK_EQUAL(destination[0].value, 1);
  BOOST_CHECK_EQUAL(destination[1].value, 2);
}

BOOST_AUTO_TEST_CASE(in_place_assignment_failures_keep_logical_length)
{
  {
    SbList<SbListThrowingValue> list;
    list.append(SbListThrowingValue(1));
    list.append(SbListThrowingValue(2));

    bool threw = false;
    SbListThrowingValue::throwAfter(0);
    try { list.append(SbListThrowingValue(3)); }
    catch (const std::runtime_error &) { threw = true; }
    SbListThrowingValue::disableFailure();

    BOOST_REQUIRE(threw);
    BOOST_REQUIRE_EQUAL(list.getLength(), 2);
    BOOST_CHECK_EQUAL(list[0].value, 1);
    BOOST_CHECK_EQUAL(list[1].value, 2);
  }

  {
    SbList<SbListThrowingValue> list;
    for (int i = 0; i < 3; i++) list.append(SbListThrowingValue(i + 1));

    bool threw = false;
    SbListThrowingValue::throwAfter(0);
    try { list.remove(0); }
    catch (const std::runtime_error &) { threw = true; }
    SbListThrowingValue::disableFailure();

    BOOST_REQUIRE(threw);
    BOOST_REQUIRE_EQUAL(list.getLength(), 3);
    for (int i = 0; i < 3; i++) BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }

  {
    SbList<SbListThrowingValue> list;
    for (int i = 0; i < 3; i++) list.append(SbListThrowingValue(i + 1));

    bool threw = false;
    SbListThrowingValue::throwAfter(0);
    try { list.removeFast(0); }
    catch (const std::runtime_error &) { threw = true; }
    SbListThrowingValue::disableFailure();

    BOOST_REQUIRE(threw);
    BOOST_REQUIRE_EQUAL(list.getLength(), 3);
    for (int i = 0; i < 3; i++) BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }
}



#include <new>

class SbListGrowthFailureValue {
public:
  SbListGrowthFailureValue(const int valuearg = 0)
    : value(valuearg)
  {
    if (SbListGrowthFailureValue::faildefault && valuearg == 0) {
      throw std::bad_alloc();
    }
    ++SbListGrowthFailureValue::livecount;
  }

  SbListGrowthFailureValue(const SbListGrowthFailureValue & other)
    : value(other.value)
  {
    ++SbListGrowthFailureValue::livecount;
  }

  ~SbListGrowthFailureValue()
  {
    --SbListGrowthFailureValue::livecount;
  }

  SbListGrowthFailureValue & operator=(const SbListGrowthFailureValue & other)
  {
    if (SbListGrowthFailureValue::failassignment) {
      throw std::bad_alloc();
    }
    this->value = other.value;
    return *this;
  }

  static void failDefaultConstruction(const bool enable)
  {
    SbListGrowthFailureValue::faildefault = enable;
  }

  static void failAssignment(const bool enable)
  {
    SbListGrowthFailureValue::failassignment = enable;
  }

  static int live(void)
  {
    return SbListGrowthFailureValue::livecount;
  }

  int value;

private:
  static bool faildefault;
  static bool failassignment;
  static int livecount;
};

bool SbListGrowthFailureValue::faildefault = false;
bool SbListGrowthFailureValue::failassignment = false;
int SbListGrowthFailureValue::livecount = 0;

class SbListGrowthFailureAccess : public SbList<SbListGrowthFailureValue> {
public:
  int capacity(void) const { return this->getArraySize(); }
};

BOOST_AUTO_TEST_CASE(growth_construction_failure_preserves_capacity_and_recovery)
{
  SbListGrowthFailureAccess list;
  for (int i = 0; i < 4; ++i) {
    list.append(SbListGrowthFailureValue(i + 1));
  }

  SbListGrowthFailureValue fifth(5);
  const SbListGrowthFailureValue * const oldarray = list.getArrayPtr();
  const int oldlive = SbListGrowthFailureValue::live();

  bool threw = false;
  SbListGrowthFailureValue::failDefaultConstruction(true);
  try {
    list.append(fifth);
  }
  catch (const std::bad_alloc &) {
    threw = true;
  }
  SbListGrowthFailureValue::failDefaultConstruction(false);

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(list.getLength(), 4);
  BOOST_REQUIRE_EQUAL(list.capacity(), 4);
  BOOST_CHECK(list.getArrayPtr() == oldarray);
  BOOST_CHECK_EQUAL(SbListGrowthFailureValue::live(), oldlive);
  for (int i = 0; i < 4; ++i) {
    BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }

  list.append(fifth);
  BOOST_CHECK_EQUAL(list.getLength(), 5);
  BOOST_CHECK_EQUAL(list.capacity(), 8);
  BOOST_CHECK_EQUAL(list[4].value, 5);
}

BOOST_AUTO_TEST_CASE(growth_assignment_failure_releases_candidate_buffer)
{
  SbListGrowthFailureAccess list;
  for (int i = 0; i < 4; ++i) {
    list.append(SbListGrowthFailureValue(i + 1));
  }

  SbListGrowthFailureValue fifth(5);
  const SbListGrowthFailureValue * const oldarray = list.getArrayPtr();
  const int oldlive = SbListGrowthFailureValue::live();

  bool threw = false;
  SbListGrowthFailureValue::failAssignment(true);
  try {
    list.append(fifth);
  }
  catch (const std::bad_alloc &) {
    threw = true;
  }
  SbListGrowthFailureValue::failAssignment(false);

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(list.getLength(), 4);
  BOOST_REQUIRE_EQUAL(list.capacity(), 4);
  BOOST_CHECK(list.getArrayPtr() == oldarray);
  BOOST_CHECK_EQUAL(SbListGrowthFailureValue::live(), oldlive);
  for (int i = 0; i < 4; ++i) {
    BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }

  list.append(fifth);
  BOOST_CHECK_EQUAL(list.getLength(), 5);
  BOOST_CHECK_EQUAL(list.capacity(), 8);
  BOOST_CHECK_EQUAL(list[4].value, 5);
}

BOOST_AUTO_TEST_CASE(fit_assignment_failure_releases_candidate_buffer)
{
  SbListGrowthFailureAccess list;
  for (int i = 0; i < 9; ++i) {
    list.append(SbListGrowthFailureValue(i + 1));
  }
  list.truncate(6);

  const SbListGrowthFailureValue * const oldarray = list.getArrayPtr();
  const int oldlive = SbListGrowthFailureValue::live();

  bool threw = false;
  SbListGrowthFailureValue::failAssignment(true);
  try {
    list.fit();
  }
  catch (const std::bad_alloc &) {
    threw = true;
  }
  SbListGrowthFailureValue::failAssignment(false);

  BOOST_REQUIRE(threw);
  BOOST_CHECK_EQUAL(list.getLength(), 6);
  BOOST_REQUIRE_EQUAL(list.capacity(), 16);
  BOOST_CHECK(list.getArrayPtr() == oldarray);
  BOOST_CHECK_EQUAL(SbListGrowthFailureValue::live(), oldlive);
  for (int i = 0; i < 6; ++i) {
    BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }

  list.fit();
  BOOST_CHECK_EQUAL(list.getLength(), 6);
  BOOST_CHECK_EQUAL(list.capacity(), 6);
  for (int i = 0; i < 6; ++i) {
    BOOST_CHECK_EQUAL(list[i].value, i + 1);
  }
}

// Regression test for the real-world usage pattern found in e.g.
// SoLightPath::setHead(), SoBaseKit::createFieldList() and
// SbHeap::emptyHeap(): truncate(0) immediately followed by append(),
// then growing well past the point where a re-grow of the internal
// buffer is required. This is also the exact pattern that used to
// trigger a GCC -Warray-bounds false positive in grow() (GCC's
// constant propagation of numitems==0 from truncate(0) into the
// inlined append()/grow() met its inability to prove itembuffersize
// can never be 0, and it flagged a "new Type[0]" allocation that is
// not actually reachable) -- this test exercises the real memory
// behavior so any future change to grow()'s bookkeeping that broke
// the underlying invariant (itembuffersize is always >= DEFAULTSIZE)
// would show up here, not just as a compiler warning.
BOOST_AUTO_TEST_CASE(truncate_zero_then_grow_past_default_size)
{
  SbList<int> list;
  for (int i = 0; i < 4; i++) { list.append(i); }
  BOOST_CHECK_EQUAL(list.getLength(), 4);

  list.truncate(0);
  BOOST_CHECK_EQUAL(list.getLength(), 0);

  // Append well past the built-in inline buffer size (4), forcing
  // grow() to run its "double the buffer" path multiple times right
  // after numitems was reset to 0 by truncate(0).
  const int n = 100;
  for (int i = 0; i < n; i++) { list.append(i * 3); }

  BOOST_REQUIRE_EQUAL(list.getLength(), n);
  for (int i = 0; i < n; i++) {
    BOOST_CHECK_MESSAGE(list[i] == i * 3,
                        "SbList value corrupted after truncate(0) + growth");
  }
}

#if !defined(COIN_EXTRA_DEBUG)
BOOST_AUTO_TEST_CASE(remove_missing_item_preserves_list)
{
  SbList<int> list;
  list.append(10);
  list.append(20);
  list.append(30);

  list.removeItem(99);

  BOOST_REQUIRE_EQUAL(list.getLength(), 3);
  BOOST_CHECK_EQUAL(list[0], 10);
  BOOST_CHECK_EQUAL(list[1], 20);
  BOOST_CHECK_EQUAL(list[2], 30);

  SbList<int> empty;
  empty.removeItem(99);
  BOOST_CHECK_EQUAL(empty.getLength(), 0);
}
#endif // !COIN_EXTRA_DEBUG

#endif // COIN_TEST_SUITE
