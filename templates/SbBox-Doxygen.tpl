//$ TEMPLATE SbBox-Doxygen(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ IF -elements- < 2 || -elements- > 3
//$ ERROR "unimplemented dimension"
//$ ENDIF
//$ DEFINE -cxx_class- SbBox-elements--letter-
//$ DEFINE -cc_class- cc_box-elements--letter-
//$ DEFINE -element- SbVec-elements--letter-
//$ INSERT TEMPLATE Copyright

#include <Inventor/-cxx_class-.h>

/* ********************************************************************** */
/* DOXYGEN DOC for C++ WRAPPER */

/*!
  \class -cxx_class- -cxx_class-.h Inventor/-cxx_class-.h
  \brief The -cxx_class- class is an abstraction for an axis aligned 3 dimensional box.
  \ingroup base

  This box abstraction class is used by other entities in the Coin
  library for data exchange and storage. It provides a representation
  of the defining corners of a box in 3D space, with the sides aligned
  with the 3 principal axes.
//$ IF "-cxx_class- !~ m/(SbBox2f|SbBox2s|SbBox3f)/

  This class is a Coin extension.
//$ ENDIF

  \sa SbBox2f, SbBox2s, SbXfBox3f, -cc_class-
*/


/*!
  \fn -cxx_class-::-cxx_class-(void)

  The default constructor makes an empty box.
*/

/*!
//$ CASE -elements-:
3:  \fn -cxx_class-::-cxx_class-(const -type- minx, const -type- miny, const -type- minz, const -type- maxx, const -type- maxy, const -type- maxz)
//$ ESAC

  Constructs a box with the given corners.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.
*/

/*!
  \fn -cxx_class-::-cxx_class-(const -element- & min, const -element- & max)

  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
*/

/*!
  \fn -cxx_class-::~-cxx_class-(void)

  Destructor does nothing.
*/

/*!
  \fn const -element- & -cxx_class-::getMin(void) const

  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/

/*!
  \fn const -element- & -cxx_class-::getMax(void) const

  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/

/*!
  \fn -element- & -cxx_class-::getMin(void)

  Returns a modifiable reference the minimum point.
*/


/*!
  \fn -element- & -cxx_class-::getMax(void)

  Returns a modifiable reference the maximum point.
*/

/*!
  \fn -element- -cxx_class-::getCenter(void) const

  Returns the center point of the box.
*/

/*!
  \fn void -cxx_class-::extendBy(const -element- & point)

  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.
*/

/*!
  \fn void -cxx_class-::extendBy(const -cxx_class- & box)

  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
*/


/*!
  \fn SbBool -cxx_class-::intersect(const -element- & point) const

  Check if the given point lies within the boundaries of this box.
*/

/*!
  \fn SbBool -cxx_class-::intersect(const -element- & box) const

  Check if the given \a box lies wholly or partly within the boundaries
  of this box.
*/

/*!
  \fn void -cxx_class-::setBounds(const -type- minx, const -type- miny, const -type- minz, const -type- maxx, const -type- maxy, const -type- maxz)

  Reset the boundaries of the box.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.

  \sa getBounds().
*/

/*!
  \fn void -cxx_class-::setBounds(const -element- & min, const -element- & max)

  Reset the boundaries of the box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.

  \sa getBounds().
 */

/*!
  \fn void -cxx_class-::getBounds(-type- & minx, -type- & miny, -type- & minz, -type- & maxx, -type- & maxy, -type- & maxz) const

  Returns the box boundaries.

  \sa setBounds().
*/


/*!
  \fn void -cxx_class-::getBounds(-element- & min, -element- & max) const

  Returns the box corner points.

  \sa setBounds().
*/

/*!
  void -cxx_class-::getOrigin(-type- & x0, -type- & y0, -type- & z0) const

  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/

/*!
  \fn void -cxx_class-::getSize(-type- & dx, -type- & dy, -type- & dz) const

  Returns width, height and depth of box.
*/

/*!
  \fn void -cxx_class-::makeEmpty(void)

  Marks this as an empty box.

  \sa isEmpty().
*/


/*!
  \fn SbBool -cxx_class-::isEmpty(void) const

  Check if this has been marked as an empty box.

  \sa makeEmpty().
*/

/*!
  \fn SbBool -cxx_class-::hasVolume(void) const

  Check if the box has been correctly specified and by that virtue
  has volume.
*/

/*!
  \fn -type- -cxx_class-::getVolume(void) const

  Check if the box has "positive" volume, i.e. the lower left corner is
  actually lower and more left than the maximum point.
*/

/*!
  \fn void -cxx_class-::getSpan(const -element- & dir, -type- & dmin, -type- & dmax) const

  Find the span of the box in the given direction (i.e. how much room in
  the given direction the box needs). The distance is returned as the minimum
  and maximum distance from origo to the closest and furthest plane defined
  by the direction vector and each of the box' corners. The difference
  between these values gives the span.
*/

/*!
  \fn void -cxx_class-::transform(const SbMatrix & matrix)

  Transform the box by the matrix, and change its boundaries to contain
  the transformed box.

  Doesn't touch illegal/empty boxes.
*/

/*!
  \fn void -cxx_class-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
*/


/*!
  \fn int operator == (const -cxx_class- & b1, const -cxx_class- & b2)

  \relates SbBox3f

  Check \a b1 and \a b2 for equality.
*/

/*!
  \fn int operator != (const -cxx_class- & b1, const -cxx_class- & b2)

  \relates SbBox3f

  Check \a b1 and \a b2 for inequality.
*/

/*!
  \fn SbBool -cxx_class-::outside(const SbMatrix & mvp, int & cullbits) const

  Check if the box is outside the view volume defined by the \a mvp
  matrix. Sets \a cullbits according to which planes we're inside or
  outside. Bit 0 (0x1) is cleared when box is completely inside
  left and right clipping planes. Bit 1 (0x2) is cleared when
  box is inside top and bottom clipping planes. Bit 2 (0x4) is
  cleared when box is inside near and far clipping planes.

  Returns TRUE if box is completely outside one of the clipping
  planes. FALSE otherwise.
*/

/*!
  \fn -element- -cxx_class-::getClosestPoint(const -element- & point) const

  Return the point on the box closest to the given \a point.
*/


