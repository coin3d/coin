//$ TEMPLATE cc_vec_source(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbVec-elements--letter-
//$ DEFINE -cc-class- cc_vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ IF -elements- < 2 || -elements- > 4
//$ ERROR "unimplemented dimension"
//$ ENDIF
//$ IF -elements- == 2
//$ WARNING implemented dimension
//$ ENDIF

//$ INSERT TEMPLATE Copyright

#include <assert.h>

#include <Inventor/C/base/vec-elements--letter-.h>

/* ********************************************************************** */

void
-cc-class-_cross(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
//$ IF -elements- == 2
  assert(0 && "not implemented");
//$ ELSIF -elements- == 3
  (*dst)[0] = (*v1)[1]*(*v2)[2] - (*v1)[2]*(*v2)[1];
  (*dst)[1] = (*v1)[2]*(*v2)[0] - (*v1)[0]*(*v2)[2];
  (*dst)[2] = (*v1)[0]*(*v2)[1] - (*v1)[1]*(*v2)[0];
//$ ELSIF -elements- == 4
  assert(0 && "not implemented");
//$ ENDIF
}

-type-
-cc-class-_dot(const -cc-class- * v1, const -cc-class- * v2)
{
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1]; //$ IF -elements- == 2
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1] + (*v1)[2]*(*v2)[2]; //$ IF -elements- == 3
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1] + (*v1)[2]*(*v2)[2] + (*v1)[3]*(*v2)[3]; //$ IF -elements- == 4
}

SbBool
-cc-class-_equals(const -cc-class- * v1, const -cc-class- * v2, const -type- tolerance)
{
  -type- xdist, ydist;
  -type- zdist; //$ IF -elements- > 2
  -type- wdist; //$ IF -elements- > 3
#if COIN_DEBUG && 0
  if ( tolerance < (-type-) 0 )
    SoDebugError::postWarning("-cc-class-_equals", "tolerance should be >= 0");
#endif // COIN_DEBUG

  xdist = (*v1)[0] - (*v2)[0];
  ydist = (*v1)[1] - (*v2)[1];
  zdist = (*v1)[2] - (*v2)[2]; //$ IF -elements- > 2
  wdist = (*v1)[3] - (*v2)[2]; //$ IF -elements- > 3

  return ((xdist*xdist + ydist*ydist) <= tolerance); //$ IF -elements- == 2
  return ((xdist*xdist + ydist*ydist + zdist*zdist) <= tolerance); //$ IF -elements- == 3
  return ((xdist*xdist + ydist*ydist + zdist*zdist + wdist*wdist) <= tolerance); //$ IF -elements- == 4
}

-type-
-cc-class-_length(const -cc-class- * vec)
{
}

-type-
-cc-class-_sqr_length(const -cc-class- * vec)
{
}

void
-cc-class-_negate(const -cc-class- * src, -cc-class- * dst)
{
  (*dst)[0] = -(*src)[0];
  (*dst)[1] = -(*src)[1];
  (*dst)[2] = -(*src)[2]; //$ IF -elements- > 2
  (*dst)[3] = -(*src)[3]; //$ IF -elements- > 3
}

-type-
-cc-class-_normalize(const -cc-class- * src, -cc-class- * dst)
{
  -type- factor;
  factor = 1; /* FIXME */

  (*dst)[0] = (*src)[0] / factor;
  (*dst)[1] = (*src)[1] / factor;
  (*dst)[2] = (*src)[2] / factor; //$ IF -elements- > 2
  (*dst)[3] = (*src)[3] / factor; //$ IF -elements- > 3
  return factor;
}

int
-cc-class-_cmp(const -cc-class- * v1, const -cc-class- * v2)
{
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1]); //$ IF -elements- == 2
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1] && (*v1)[2] == (*v2)[2]); //$ IF -elements- == 3
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1] && (*v1)[2] == (*v2)[2] && (*v1)[3] == (*v2)[3]); //$ IF -elements- == 4
//$ ENDIF
}

void
-cc-class-_mult_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst)
{
}

void
-cc-class-_mult_scalar_vec(const -type- d, const -cc-class- * v, -cc-class- * dst)
{
}

void
-cc-class-_div_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst)
{
  (*dst)[0] = (*v)[0] / d;
  (*dst)[1] = (*v)[1] / d;
  (*dst)[2] = (*v)[2] / d; //$ IF -elements- > 2
  (*dst)[3] = (*v)[3] / d; //$ IF -elements- > 3
}

void
-cc-class-_add(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  (*dst)[0] = (*v1)[0] + (*v2)[0];
  (*dst)[1] = (*v1)[1] + (*v2)[1];
  (*dst)[2] = (*v1)[2] + (*v2)[2]; //$ IF -elements- > 2
  (*dst)[3] = (*v1)[3] + (*v2)[3]; //$ IF -elements- > 3
}

void
-cc-class-_sub(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  (*dst)[0] = (*v1)[0] - (*v2)[0];
  (*dst)[1] = (*v1)[1] - (*v2)[1];
  (*dst)[2] = (*v1)[2] - (*v2)[2]; //$ IF -elements- > 2
  (*dst)[3] = (*v1)[3] - (*v2)[3]; //$ IF -elements- > 3
}

/* ********************************************************************** */

/*!
  \class -class-name- SbLinear.h Inventor/SbLinear.h
  \brief The -class-name- class is a -elements- dimensional vector with
//$ IF -integertype-
  integer coordinates.
//$ ELSE
  floating point coordinates.
//$ ENDIF
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a -elements- dimensional vector
  aswell as simple arithmetic operations.

  \sa SbVec2s, SbVec2f, SbVec2d, SbVec3s, SbVec3f, SbVec3d, SbVec4s, SbVec4f, SbVec4d.
*/


/*!
  \fn -class-name-::-class-name-(void)

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  \fn -class-name-::-class-name-(const -type- v[-elements-])

  Constructs an -class-name- instance with initial values from \a v.
*/

/*!
  \fn -class-name-::-class-name-(const -type- x, const -type- y, const -type- z)

  Constructs an -class-name- instance with the initial vector endpoint set
  to \a <x,y,z>.
*/


/*!
  \fn -type- & -class-name-::operator[](const int i)

  Index operator. Returns modifiable x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn -type- -class-name-::operator[](const int i) const

  Index operator. Returns x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

