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
  \class SbList SbList.h Inventor/lists/SbList.h
  \brief The SbList class is a template container class for lists.
  \ingroup base

  SbList is an extension of the Coin library versus the original Open
  Inventor API. Open Inventor handles most list classes by inheriting
  the SbPList class, which contains an array of generic \c void*
  pointers. By using this template-based class instead, we can share
  more code and make the list handling code more typesafe.

  Care has been taken to make sure the list classes which are part of
  the Open Inventor API to still be compatible with their original
  interfaces derived from the SbPList base class. If you still bump
  into any problems when porting your Open Inventor applications, let
  us know and we'll do our best to sort them out.


  \sa SbPList
*/

#include <Inventor/lists/SbList.h>

/*!
  \fn SbList<Type>::SbList(const int initsize)

  Default constructor.

  The \a initsize argument hints about how many elements the list will
  contain, so memory allocation can be done efficiently.

  Important note: explicitly specifying an \a initsize value does \c
  not mean that the list will initially contain this number of
  values. Here's a good example on how to give yourself hard to find
  bugs:

  \code
  SbList<SbBool> flags(2); // Assume we need only 2 elements. Note
                           // that the list is still 0 elements long.
  flags[0] = TRUE;         // Ouch. List is still 0 elements long.
  \endcode

  Since this conceptual misunderstanding is so easy to make, you're
  probably better (or at least safer) off leaving the \a initsize
  argument to its default value by not explicitly specifying it.

 */

/*!
  \fn SbList<Type>::SbList(const SbList<Type> & l)

  Copy constructor.
 */

/*!
\fn SbList<Type>::~SbList()

  Destructor, frees all internal resources used by the list container.
*/

/*!
  \fn void SbList<Type>::copy(const SbList<Type> & l)

  Make this list a copy of \a l.
 */

/*!
  \fn SbList<Type> & SbList<Type>::operator=(const SbList<Type> & l)

  Make this list a copy of \a l.
 */

/*!
  \fn void SbList<Type>::fit(void)

  Fit the allocated array exactly around the length of the list.

  You should normally not need to call this method, and it is only
  available for the sake of having the option to optimize memory usage
  for the unlikely event that you should use a huge SbList list.
 */

/*!
  \fn void SbList<Type>::append(const Type item)

  Append the \a item at the end of list.
 */

/*!
  \fn int SbList<Type>::find(const Type item) const

  Return index of \a item in the list, or -1 if \a item is not
  present.
*/

/*!
  \fn void SbList<Type>::insert(const Type item, const int insertbefore)

  Insert \a item at index \a insertbefore.

  \a insertbefore should not be larger than the current number of
  items in the list.
 */

/*!
  \fn void SbList<Type>::remove(const int index)

  Remove the item at \a index, moving all subsequent items downwards
  on place in the list.
*/

/*!
  \fn void SbList<Type>::removeFast(const int index)

  Remove the item at \a index, moving the last item into its place and
  truncating the list.
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
*/

/*!
  \fn SbList<Type>::operator Type *(void)

  Returns pointer to a modifiable array of the lists elements.
*/

/*!
  \fn SbList<Type>::operator const Type *(void) const

  Returns pointer to a non-modifiable array of the lists elements.
*/

/*!
  \fn Type SbList<Type>::operator[](const int index) const

  Returns a copy of item at \a index.
*/

/*!
  \fn Type & SbList<Type>::operator[](const int index)

  Returns a reference to item at \a index.
*/

/*!
  \fn SbBool SbList<Type>::operator==(const SbList<Type> & l) const

  Returns \c TRUE if this list and \a l are identical, containing
  the exact same set of elements.
*/

/*!
  \fn SbBool SbList<Type>::operator!=(const SbList<Type> & l) const

  Returns \c TRUE if this list and \a l are not equal.
*/
