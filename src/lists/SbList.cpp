/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SbList Inventor/lists/SbList.h
  \brief The SbList class is a template container class for lists.
*/

#if defined(__GNUC__) || defined( __BEOS__ ) || defined( WIN32 )
// IRIX/CC doesn't instanciate templates like this.

// #define NO_SB_LIST_INLINE_KEYWORDS

#include <Inventor/lists/SbList.h>

#endif // defined(__GNUC__) || defined( __BEOS__ ) || defined( WIN32 )

/*!
  \var SbList::itemBuffer

  FIXME: write doc.
*/

/*!
  \var SbList::itemBufferSize

  FIXME: write doc.

*/

/*!
  \var SbList::numItems

  FIXME: write doc.
*/

/*!
  \fn SbList::SbList(const int initSize = 0)

  A constructor (default).
*/

/*!
  \fn SbList::SbList(const SbList<Type> & list)

  A copy constructor.
*/

/*!
  \fn SbList::~SbList(void)

  The destructor.
*/

/*!
  \fn void SbList::copy(const SbList<Type> & list)

  FIXME: write doc.
*/

/*!
  \fn void SbList::append(const Type item)

  FIXME: write doc.
*/

/*!
  \fn void SbList::append(const SbList<Type> & list)

  FIXME: write doc.
*/

/*!
  \fn int SbList::find(const Type item) const

  Return index of \a item in the list, or -1 if \item is
  not present.
*/

/*!
  \fn void SbList::insert(const Type item, const int addBefore)

  FIXME: write doc.
*/

/*!
  \fn void SbList::remove(const int index)

  FIXME: write doc.
*/

/*!
  \fn void SbList::removeFast(const int index)

  FIXME: write doc.
*/

/*!
  \fn int SbList::getLength(void) const

  FIXME: write doc.
*/

/*!
  \fn void SbList::truncate(const int length)

  FIXME: write doc.
*/

/*!
  \fn void SbList::clear(const int newSize = MIN_LIST_SIZE)

  FIXME: write doc.
*/

/*!
  \fn void SbList::push(const Type item)

  FIXME: write doc.
*/

/*!
  \fn Type SbList::pop(void)

  FIXME: write doc.
*/

/*!
  \fn SbList<Type> & SbList::operator = (const SbList<Type> & list)

  FIXME: write doc.
*/

/*!
  \fn SbList::operator Type * (void)

  FIXME: write doc.
*/

/*!
  \fn SbList::operator const Type * (void) const

  FIXME: write doc.
*/

/*!
  \fn const Type * SbList::constArrayPointer(void) const

  FIXME: write doc.
*/

/*!
  \fn Type * SbList::arrayPointer(void)

  FIXME: write doc.
*/

/*!
  \fn Type SbList::operator [] (const int index) const

  FIXME: write doc.
*/

/*!
  \fn Type & SbList::operator [] (const int index)

  FIXME: write doc.
*/

/*!
  \fn int SbList::operator == (const SbList<Type> & list) const

  FIXME: write doc.
*/

/*!
  \fn int SbList::operator != (const SbList<Type> & list) const

  FIXME: write doc.
*/

/*!
  \fn Type SbList::get(const int index) const

  FIXME: write doc.
*/

/*!
  \fn void SbList::set(const int index, const Type item)

  FIXME: write doc.
*/

/*!
  \fn void SbList::fit(void)

  FIXME: write doc.
*/

/*!
  \fn Type * SbList::stealPointer(void)

  FIXME: write doc.

  Do not use the array after calling this method.
*/

/*!
  \fn void SbList::setBufferSize(const int newsize)

  FIXME: write doc.
*/

/*!
  \fn void SbList::growList(void);

  FIXME: write doc.
*/

#if 0
void main(void) {
    SbIntList list1;
    list1.append(0);
    list1.append(4);
    list1.append(2);
    list1.append(8);
    list1.append(0);
    SbIntList list2(list1);
    list1.print();
    printf("list1: 0: %d, 1: %d, 2: %d, 3: %d, 4: %d\n",
        list1[0], list1[1], list1[2], list1[3], list1[4]);
    list2.print();
    printf("list1: 0: %d, 1: %d, 2: %d, 3: %d, 4: %d\n",
        list2[0], list2[1], list2[2], list2[3], list2[4]);
    if (list1 == list2)  printf( "  list1 == list2\n" );
    else                   printf("! list1 == list2\n");
    if (list1 != list2)  printf( "  list1 != list2\n" );
    else                   printf("! list1 != list2\n");
    list2.append(list1);
    list2.print();
    printf("list2: 0: %d, 1: %d, 2: %d, 3: %d, 4: %d",
        list2[0], list2[1], list2[2], list2[3], list2[4]);
    printf(" 5: %d, 6: %d, 7: %d, 8: %d, 9: %d\n",
        list2[5], list2[6], list2[7], list2[8], list2[9]);
    if (list1 == list2)  printf( "  list1 == list2\n" );
    else                   printf("! list1 == list2\n");
    if (list1 != list2)  printf( "  list1 != list2\n" );
    else                   printf("! list1 != list2\n");
}
#endif
