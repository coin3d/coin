//$ TEMPLATE cc_vec_source(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbVec-elements--letter-
//$ DEFINE -cc-class- cc_vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
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
//$ IF -elements- == 2
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1];
//$ ELSIF -elements- == 3
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1] + (*v1)[2]*(*v2)[2];
//$ ELSIF -elements- == 4
  return (*v1)[0]*(*v2)[0] + (*v1)[1]*(*v2)[1]
       + (*v1)[2]*(*v2)[2] + (*v1)[3]*(*v2)[3];
//$ ENDIF
}

SbBool
-cc-class-_equals(const -cc-class- * v1, const -cc-class- * v2, const -type- tolerance)
{
  -type- xdist, ydist, zdist, wdist;
#if COIN_DEBUG && 0
  if ( tolerance < (-type-) 0 )
    SoDebugError::postWarning("-cc-class-_equals", "tolerance should be >= 0");
#endif // COIN_DEBUG

  xdist = (*v1)[0] - (*v2)[0];
  ydist = (*v1)[1] - (*v2)[1];
//$ IF -elements- > 2
  zdist = (*v1)[2] - (*v2)[2];
//$ ENDIF
//$ IF -elements- > 3
  wdist = (*v1)[3] - (*v2)[2];
//$ ENDIF

//$ IF -elements- == 2
  return ((xdist*xdist + ydist*ydist) <= tolerance);
//$ ELSIF -elements- == 3
  return ((xdist*xdist + ydist*ydist + zdist*zdist) <= tolerance);
//$ ELSIF -elements- == 4
  return ((xdist*xdist + ydist*ydist + zdist*zdist + wdist*wdist) <= tolerance);
//$ ENDIF
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
//$ IF -elements- > 2
  (*dst)[2] = -(*src)[2];
//$ ENDIF
//$ IF -elements- > 3
  (*dst)[3] = -(*src)[3];
//$ ENDIF
}

-type-
-cc-class-_normalize(const -cc-class- * src, -cc-class- * dst)
{
  -type- factor;
  factor = 1; /* FIXME */
  (*dst)[0] = (*src)[0] / factor;
  (*dst)[1] = (*src)[1] / factor;
//$ IF -elements- > 2
  (*dst)[2] = (*src)[2] / factor;
//$ ENDIF
//$ IF -elements- > 3
  (*dst)[3] = (*src)[3] / factor;
//$ ENDIF
  return factor;
}

int
-cc-class-_cmp(const -cc-class- * v1, const -cc-class- * v2)
{
//$ ELSIF -elements- == 2
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1]);
//$ ELSIF -elements- == 3
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1] && (*v1)[2] == (*v2)[2]);
//$ ELSIF -elements- == 4
  return ((*v1)[0] == (*v2)[0] && (*v1)[1] == (*v2)[1]
       && (*v1)[2] == (*v2)[2] && (*v1)[3] == (*v2)[3]);
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
//$ IF -elements- > 2
  (*dst)[2] = (*v)[2] / d;
//$ ENDIF
//$ IF -elements- > 3
  (*dst)[3] = (*v)[3] / d;
//$ ENDIF
}

void
-cc-class-_add(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  (*dst)[0] = (*v1)[0] + (*v2)[0];
  (*dst)[1] = (*v1)[1] + (*v2)[1];
//$ IF -elements- > 2
  (*dst)[2] = (*v1)[2] + (*v2)[2];
//$ ENDIF
//$ IF -elements- > 3
  (*dst)[3] = (*v1)[3] + (*v2)[3];
//$ ENDIF
}

void
-cc-class-_sub(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  (*dst)[0] = (*v1)[0] - (*v2)[0];
  (*dst)[1] = (*v1)[1] - (*v2)[1];
//$ IF -elements- > 2
  (*dst)[2] = (*v1)[2] - (*v2)[2];
//$ ENDIF
//$ IF -elements- > 3
  (*dst)[3] = (*v1)[3] - (*v2)[3];
//$ ENDIF
}

/* ********************************************************************** */

/*!
  \class -class-name- Inventor/-class-name-.h
  \brief The -class-name- class is yet to be documented.

  This class acts mainly as a wrapper around the -cc-class- C type.
*/

