//$ TEMPLATE cc_vector_source(-type-, -elements-, -letter-)
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
  dst->vec[0] = v1->vec[1]*v2->vec[2] - v1->vec[2]*v2->vec[1];
  dst->vec[1] = v1->vec[2]*v2->vec[0] - v1->vec[0]*v2->vec[2];
  dst->vec[2] = v1->vec[0]*v2->vec[1] - v1->vec[1]*v2->vec[0];
//$ ELSIF -elements- == 4
  assert(0 && "not implemented");
//$ ENDIF
}

-type-
-cc-class-_dot(const -cc-class- * v1, const -cc-class- * v2)
{
  return v1->vec[0]*v2->vec[0] + v1->vec[1]*v2->vec[1]; //$ IF -elements- == 2
  return v1->vec[0]*v2->vec[0] + v1->vec[1]*v2->vec[1] + v1->vec[2]*v2->vec[2]; //$ IF -elements- == 3
  return v1->vec[0]*v2->vec[0] + v1->vec[1]*v2->vec[1] + v1->vec[2]*v2->vec[2] + v1->vec[3]*v2->vec[3]; //$ IF -elements- == 4
}

void
-cc-class-_find_closest_axis(const -cc-class- * vec, -cc-class- * dst)
{
  -type- xabs, yabs
//$ APPEND , zabs //$ IF -elements- > 2
//$ APPEND , wabs //$ IF -elements- > 3
//$ APPEND ;
  dst->vec[0] = (-type-) 0;
  dst->vec[1] = (-type-) 0;
  dst->vec[2] = (-type-) 0; //$ IF -elements- > 2
  dst->vec[3] = (-type-) 0; //$ IF -elements- > 3

  xabs = (vec->vec[0] >= (-type-) 0) ? vec->vec[0] : - vec->vec[0];
  yabs = (vec->vec[1] >= (-type-) 0) ? vec->vec[1] : - vec->vec[1];
  zabs = (vec->vec[2] >= (-type-) 0) ? vec->vec[2] : - vec->vec[2]; //$ IF -elements- > 2
  wabs = (vec->vec[3] >= (-type-) 0) ? vec->vec[3] : - vec->vec[3]; //$ IF -elements- > 3

  if ( (xabs >= yabs)
//$ APPEND  && (xabs >= zabs) //$ IF -elements- > 2
//$ APPEND  && (xabs >= wabs) //$ IF -elements- > 3
//$ APPEND  )
    dst->vec[0] = (vec->vec[0] > 0.0f) ? 1.0f : -1.0f;
  else
//$ IF -elements- > 2
//$ APPEND: if ( (yabs >= xabs) && (yabs >= zabs)
//$ APPEND: && (yabs >= wabs) //$ IF -elements- > 3
//$ APPEND: )
//$ ENDIF
    dst->vec[1] = (vec->vec[1] > 0.0f) ? 1.0f : -1.0f;
  else //$ IF -elements- > 2
//$ APPEND: if ( (zabs >= xabs) && (zabs >= yabs) && (zabs >= wabs) ) //$ IF -elements- > 3
    dst->vec[2] = (vec->vec[2] > 0.0f) ? 1.0f : -1.0f;
//$ IF -elements- > 3
  else
    dst->vec[3] = (vec->vec[3] > 0.0f) ? 1.0f : -1.0f;
//$ ENDIF
}

//$ IF ! -integertype- && -elements- == 4
void
-cc-class-_find_real(const -cc-class- * vec, cc_vec3-letter- * dst)
{
#if COIN_DEBUG && 0
  if(!(this->vec[3] != 0.0f))
    SoDebugError::postWarning("SbVec4f::getReal",
                              "The 4th vector component is zero => "
                              "division by zero");
#endif // COIN_DEBUG
  dst->vec[0] = vec->vec[0]/vec->vec[3];
  dst->vec[1] = vec->vec[1]/vec->vec[3];
  dst->vec[2] = vec->vec[2]/vec->vec[3];
}
//$ ENDIF

SbBool
-cc-class-_equals(const -cc-class- * v1, const -cc-class- * v2, const -type- tolerance)
{
  -type- xdist, ydist
//$ APPEND , zdist //$ IF -elements- > 2
//$ APPEND , wdist //$ IF -elements- > 3
//$ APPEND ;
#if COIN_DEBUG && 0
  if ( tolerance < (-type-) 0 )
    SoDebugError::postWarning("-cc-class-_equals", "tolerance should be >= 0");
#endif // COIN_DEBUG

  xdist = v1->vec[0] - v2->vec[0];
  ydist = v1->vec[1] - v2->vec[1];
  zdist = v1->vec[2] - v2->vec[2]; //$ IF -elements- > 2
  wdist = v1->vec[3] - v2->vec[2]; //$ IF -elements- > 3

  return ((xdist*xdist + ydist*ydist) <= tolerance); //$ IF -elements- == 2
  return ((xdist*xdist + ydist*ydist + zdist*zdist) <= tolerance); //$ IF -elements- == 3
  return ((xdist*xdist + ydist*ydist + zdist*zdist + wdist*wdist) <= tolerance); //$ IF -elements- == 4
}

//$ IF ! -integertype-
-type-
-cc-class-_length(const -cc-class- * vec)
{
  
  return (-type-) sqrt(vec->vec[0]*vec->vec[0]
                     + vec->vec[1]*vec->vec[1]
                     + vec->vec[2]*vec->vec[2] //$ IF -elements- > 2
                     + vec->vec[3]*vec->vec[3] //$ IF -elements- > 3
//$ APPEND );
}
//$ ENDIF

-type-
-cc-class-_sqr_length(const -cc-class- * vec)
{
  return vec->vec[0]*vec->vec[0]
       + vec->vec[1]*vec->vec[1]
       + vec->vec[2]*vec->vec[2] //$ IF -elements- > 2
       + vec->vec[3]*vec->vec[3] //$ IF -elements- > 3
//$ APPEND ;
}

void
-cc-class-_negate(const -cc-class- * src, -cc-class- * dst)
{
  dst->vec[0] = - src->vec[0];
  dst->vec[1] = - src->vec[1];
  dst->vec[2] = - src->vec[2]; //$ IF -elements- > 2
  dst->vec[3] = - src->vec[3]; //$ IF -elements- > 3
}

//$ IF ! -integertype-
-type-
-cc-class-_normalize(const -cc-class- * src, -cc-class- * dst)
{
  -type- len;
  len = -cc-class-_length(src);
  if (len > 0.0f) {
    -cc-class-_div_vec_scalar(src, len, dst);
  }
#if COIN_DEBUG && 0
  else {
    SoDebugError::postWarning("SbVec3f::normalize",
                              "The length of the vector should be > 0.0f "
                              "to be able to normalize.");
  }
#endif // COIN_DEBUG
  return len;
}
//$ ENDIF

int
-cc-class-_cmp(const -cc-class- * v1, const -cc-class- * v2)
{
  return (v1->vec[0] == v2->vec[0] && v1->vec[1] == v2->vec[1]); //$ IF -elements- == 2
  return (v1->vec[0] == v2->vec[0] && v1->vec[1] == v2->vec[1] && v1->vec[2] == v2->vec[2]); //$ IF -elements- == 3
  return (v1->vec[0] == v2->vec[0] && v1->vec[1] == v2->vec[1] && v1->vec[2] == v2->vec[2] && v1->vec[3] == v2->vec[3]); //$ IF -elements- == 4
//$ ENDIF
}

void
-cc-class-_mult_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst)
{
  dst->vec[0] = v->vec[0] * d;
  dst->vec[1] = v->vec[1] * d;
  dst->vec[2] = v->vec[2] * d; //$ IF -elements- > 2
  dst->vec[3] = v->vec[3] * d; //$ IF -elements- > 3
}

void
-cc-class-_mult_scalar_vec(const -type- d, const -cc-class- * v, -cc-class- * dst)
{
  dst->vec[0] = v->vec[0] * d;
  dst->vec[1] = v->vec[1] * d;
  dst->vec[2] = v->vec[2] * d; //$ IF -elements- > 2
  dst->vec[3] = v->vec[3] * d; //$ IF -elements- > 3
}

void
-cc-class-_div_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst)
{
  dst->vec[0] = v->vec[0] / d;
  dst->vec[1] = v->vec[1] / d;
  dst->vec[2] = v->vec[2] / d; //$ IF -elements- > 2
  dst->vec[3] = v->vec[3] / d; //$ IF -elements- > 3
}

void
-cc-class-_add(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  dst->vec[0] = v1->vec[0] + v2->vec[0];
  dst->vec[1] = v1->vec[1] + v2->vec[1];
  dst->vec[2] = v1->vec[2] + v2->vec[2]; //$ IF -elements- > 2
  dst->vec[3] = v1->vec[3] + v2->vec[3]; //$ IF -elements- > 3
} /* -cc-class-_add() */

void
-cc-class-_sub(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst)
{
  dst->vec[0] = v1->vec[0] - v2->vec[0];
  dst->vec[1] = v1->vec[1] - v2->vec[1];
  dst->vec[2] = v1->vec[2] - v2->vec[2]; //$ IF -elements- > 2
  dst->vec[3] = v1->vec[3] - v2->vec[3]; //$ IF -elements- > 3
} /* -cc-class-_sub() */

void
-cc-class-_print(const -cc-class- * vec, FILE * fp)
{
#if COIN_DEBUG
//$ IF -integertype-
  (void) fprintf(fp, "<%d, %d>", //$ IF -elements- == 2
  (void) fprintf(fp, "<%d, %d, %d>", //$ IF -elements- == 3
  (void) fprintf(fp, "<%d, %d, %d, %d>", //$ IF -elements- == 4
//$ ELSE
  (void) fprintf(fp, "<%f, %f>", //$ IF -elements- == 2
  (void) fprintf(fp, "<%f, %f, %f>", //$ IF -elements- == 3
  (void) fprintf(fp, "<%f, %f, %f, %f>", //$ IF -elements- == 4
//$ ENDIF
    vec->vec[0], vec->vec[1]
//$ APPEND , vec->vec[2] //$ IF -elements- > 2
//$ APPEND , vec->vec[3] //$ IF -elements- > 3
//$ APPEND );
#endif // COIN_DEBUG
} /* -cc-class-_print() */

