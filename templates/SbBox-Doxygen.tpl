//$ TEMPLATE SbBox-Doxygen(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbBox-elements--letter-
//$ DEFINE -cc-class- cc_box-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ IF -elements- < 2 || -elements- > 3
//$ ERROR "unimplemented dimension"
//$ ENDIF

//$ INSERT TEMPLATE Copyright

#include <Inventor/-class-name-.h>

/* ********************************************************************** */
/* DOXYGEN DOC for C++ WRAPPER */

/*!
  \class -class-name- -class-name-.h Inventor/-class-name-.h
  \brief The -class-name- class is an abstraction for an axis aligned 3 dimensional box.
  \ingroup base

  This box abstraction class is used by other entities in the Coin
  library for data exchange and storage. It provides a representation
  of the defining corners of a box in 3D space, with the sides aligned
  with the 3 principal axes.
//$ IF "-class-name- !~ m/(SbBox2f|SbBox2s|SbBox3f)/

  This class is a Coin extension.
//$ ENDIF

  \sa SbBox2f, SbBox2s, SbXfBox3f
*/


/*!
  \fn -class-name-::-class-name-(void)

  The default constructor makes an empty box.
*/

/*!
//$ CASE -elements-:
3:  \fn -class-name-::-class-name-(const -type- minx, const -type- miny, const -type- minz, const -type- maxx, const -type- maxy, const -type- maxz)
//$ ESAC

  Constructs a box with the given corners.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.
*/

/*!
  \fn -class-name-::-class-name-(const -element- & min, const -element- & max)

  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
*/

/*!
  \fn -class-name-::~-class-name-(void)

  Destructor does nothing.
*/

/*!
  \fn const -element- & -class-name-::getMin(void) const

  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/

/*!
  \fn const -element- & -class-name-::getMax(void) const

  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/

/*!
  \fn -element- & -class-name-::getMin(void)

  Returns a modifiable reference the minimum point.
*/


/*!
  \fn -element- & -class-name-::getMax(void)

  Returns a modifiable reference the maximum point.
*/

/*!
  \fn -element- -class-name-::getCenter(void) const

  Returns the center point of the box.
*/

/*!
  \fn void -class-name-::extendBy(const -element- & point)

  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.
*/

/*!
  \fn void -class-name-::extendBy(const -class-name- & box)

  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
*/


/*!
  \fn SbBool -class-name-::intersect(const -element- & point) const

  Check if the given point lies within the boundaries of this box.
*/

/*!
  \fn SbBool -class-name-::intersect(const -element- & box) const

  Check if the given \a box lies wholly or partly within the boundaries
  of this box.
*/

/*!
  \fn void -class-name-::setBounds(const -type- minx, const -type- miny, const -type- minz, const -type- maxx, const -type- maxy, const -type- maxz)

  Reset the boundaries of the box.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.

  \sa getBounds().
*/

/*!
  \fn void -class-name-::setBounds(const -element- & min, const -element- & max)

  Reset the boundaries of the box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.

  \sa getBounds().
 */

/*!
  \fn void -class-name-::getBounds(-type- & minx, -type- & miny, -type- & minz, -type- & maxx, -type- & maxy, -type- & maxz) const

  Returns the box boundaries.

  \sa setBounds().
*/


/*!
  \fn void -class-name-::getBounds(-element- & min, -element- & max) const

  Returns the box corner points.

  \sa setBounds().
*/

/*!
  void -class-name-::getOrigin(-type- & x0, -type- & y0, -type- & z0) const

  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/

/*!
  \fn void -class-name-::getSize(-type- & dx, -type- & dy, -type- & dz) const

  Returns width, height and depth of box.
*/

/*!
  \fn void -class-name-::makeEmpty(void)

  Marks this as an empty box.

  \sa isEmpty().
*/


/*!
  \fn SbBool -class-name-::isEmpty(void) const

  Check if this has been marked as an empty box.

  \sa makeEmpty().
*/

/*!
  \fn SbBool -class-name-::hasVolume(void) const

  Check if the box has been correctly specified and by that virtue
  has volume.
*/

/*!
  \fn -type- -class-name-::getVolume(void) const

  Check if the box has "positive" volume, i.e. the lower left corner is
  actually lower and more left than the maximum point.
*/

/*!
  \fn void -class-name-::getSpan(const -element- & dir, -type- & dmin, -type- & dmax) const

  Find the span of the box in the given direction (i.e. how much room in
  the given direction the box needs). The distance is returned as the minimum
  and maximum distance from origo to the closest and furthest plane defined
  by the direction vector and each of the box' corners. The difference
  between these values gives the span.
*/

/*!
  \fn void -class-name-::transform(const SbMatrix & matrix)

  Transform the box by the matrix, and change its boundaries to contain
  the transformed box.

  Doesn't touch illegal/empty boxes.
*/

/*!
  \fn void -class-name-::print(FILE * fp) const

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
*/


/*!
  \fn int operator == (const -class-name- & b1, const -class-name- & b2)

  \relates SbBox3f

  Check \a b1 and \a b2 for equality.
*/

/*!
  \fn int operator != (const -class-name- & b1, const -class-name- & b2)

  \relates SbBox3f

  Check \a b1 and \a b2 for inequality.
*/

/*!
  \fn SbBool -class-name-::outside(const SbMatrix & mvp, int & cullbits) const

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
  \fn -element- -class-name-::getClosestPoint(const -element- & point) const

  Return the point on the box closest to the given \a point.
*/


