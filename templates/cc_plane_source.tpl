//$ TEMPLATE cc_plane_source(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ERROR "unimplemented data type"
//$ ENDIF
//$ DEFINE -cxx_class- SbPlane
//$ DEFINE -cc_class- cc_plane-elements--letter-
//$ DEFINE -cc_vector- cc_vec-elements--letter-
//$ DEFINE -cc_line- cc_line-elements--letter-
//$ DEFINE -cc_matrix- cc_mat/*$ EVAL(-elements-+1) */-letter-
//$ INSERT TEMPLATE Copyright

#include <assert.h>
#include <stdio.h>
#include <float.h>

#include <Inventor/C/base/plane-elements--letter-.h>
#include <Inventor/C/base/line-elements--letter-.h>
#include <Inventor/C/base/mat/*$ EVAL(-elements-+1) */-letter-.h>

#include <Inventor/C/errors/debugerror.h>

/* ********************************************************************** */

/*!
  \struct -cc_class- plane-elements--letter-.h Inventor/C/base/plane-elements--letter-.h
  \brief The -cc_class- type represents a plane in -elements-D space.
  \ingroup base

  This is a Coin extension.

  \sa -cxx_class-
*/

/*!
  \relates -cc_class-
*/

void
-cc_class-_set(-cc_class- * plane, const -cc_vector- * normal, const -type- distance)
{
  -cc_vector-_normalize(normal, &plane->normal);
  plane->distance = distance;
} /* -cc_class-_set() */

/*!
  \relates -cc_class-
*/

void
-cc_class-_set_from_points(-cc_class- * plane, const -cc_vector- * p0, const -cc_vector- * p1, const -cc_vector- * p2)
{
  -cc_vector- d10, d20;
#if COIN_DEBUG
  if( -cc_vector-_cmp(p0, p1) == 0 ||
      -cc_vector-_cmp(p1, p2) == 0 ||
      -cc_vector-_cmp(p0, p2) == 0 )
    cc_debugerror_postwarning("-cc_class-_set_from_points",
          "The three points defining the plane cannot be coincident.");
#endif /* COIN_DEBUG */

  /*
     this->normal = (p1 - p0).cross(p2 - p0);
     this->normal.normalize();
  */
  -cc_vector-_sub(p1, p0, &d10);
  -cc_vector-_sub(p2, p0, &d20);
  -cc_vector-_cross(&d10, &d20, &plane->normal);
  -cc_vector-_normalize(&plane->normal, &plane->normal);
  
  /*
  //     N·point
  // d = -------, |N| == 1
  //       |N|²
  */

  /* this->distance = this->normal.dot(p0); */
  plane->distance = -cc_vector-_dot(&plane->normal, p0);
} /* -cc_class-_set_from_points() */

/*!
  \relates -cc_class-
*/

void
-cc_class-_set_from_position(-cc_class- * plane, const -cc_vector- * point, const -cc_vector- * normal)
{
#if COIN_DEBUG
  if ( -cc_vector-_length(normal) == 0.0f )
    cc_debugerror_postwarning("-cc_class-_set_from_position",
                              "Plane normal vector is a null vector.");
#endif /* COIN_DEBUG */

  -cc_vector-_normalize(normal, &plane->normal);
 /*
  this->normal = normal;
  this->normal.normalize();
 */

  /*
  //     N·point
  // d = -------, |N| == 1
  //       |N|²
  */

  /* this->distance = this->normal.dot(point); */
  plane->distance = -cc_vector-_dot(&plane->normal, point);
} /* -cc_class-_set_from_position() */

/*!
  \relates -cc_class-
*/

void
-cc_class-_copy(const -cc_class- * src, -cc_class- * dst)
{
  -cc_vector-_copy(&src->normal, &dst->normal);
  dst->distance = src->distance;
} /* -cc_class-_copy() */

/* ********************************************************************** */

/*!
  \relates -cc_class-

  Add the given offset \a offset to the plane distance from the origin.
*/

void
-cc_class-_offset(-cc_class- * plane, const -type- offset)
{
  plane->distance += offset;
} /* -cc_class-_offset() */

/*!
  \relates -cc_class-

  Find the point on given line \a l intersecting the plane and return
  it in \a intersection. If the line is parallel to the plane,
  we return \c FALSE, otherwise \c TRUE.

  Do not pass an invalid line for the \a l parameter (i.e. with a
  null direction vector).
*/

SbBool
-cc_class-_intersect(const -cc_class- * plane, const -cc_line- * line, -cc_vector- * where)
{
  -cc_vector- offset;
  -type- t;
#if COIN_DEBUG
  if ( -cc_vector-_length(&plane->normal) == 0.0f) {
    cc_debugerror_postwarning("SbPlane::intersect",
                              "Normal vector for plane is null vector");

  }
  if ( -cc_vector-_length(&line->dir) == 0.0f ) {
    cc_debugerror_postwarning("SbPlane::intersect",
                              "Intersecting line doesn't have a direction.");
  }
#endif /* COIN_DEBUG */

  /* Check if the line is parallel to the plane. */
  if ( -cc_vector-_dot(&line->dir, &plane->normal) == 0.0f ) return FALSE;

  /*
  // From the discussion on SbLine::getClosestPoint() we know that
  // any point on the line can be expressed as:
  //                    Q = P + t*D    (1)
  //
  // We can also easily see that a point must satisfy this equation to lie
  // in the plane:
  //                    N·(Q - d*N) = 0, where N is the normal vector,
  //                                     Q is the point and d the offset
  //                                     from the origin.
  //
  // Combining these two equations and simplifying we get:
  //
  //                          d*|N|² - N·P
  //                    t = ----------------, |N| == 1
  //                               N·D
  //
  // Substituting t back in (1), we've solved the problem.
  //                                                         19980816 mortene.
  */


/*
  t = (this->distance - this->normal.dot(l.getPosition()))
    / this->normal.dot(l.getDirection());
*/
  t = (plane->distance - -cc_vector-_dot(&plane->normal, &line->pos))
    / -cc_vector-_dot(&plane->normal, &line->dir);

  /* where = l.getPosition() + t * l.getDirection(); */
  -cc_vector-_mult_vec_scalar(&line->dir, t, &offset);
  -cc_vector-_add(&line->pos, &offset, where);

  return TRUE;
} /* -cc_class-_intersect() */

/*!
  \relates -cc_class-

  Intersect this plane with \a pl, and return the resulting line in \a
  line. Returns \c TRUE if an intersection line can be found, and \c
  FALSE if the planes are parallel.
*/

SbBool
-cc_class-_get_intersection(const -cc_class- * plane1, const -cc_class- * plane2, -cc_line- * intersection)
{
  /*
  // Based on code from Graphics Gems III, Plane-to-Plane Intersection
  // by Priamos Georgiades
  */

  /*
  float invdet;  // inverse of 2x2 matrix determinant
  SbVec3f dir2;  // holds the squares of the coordinates of xdir

  SbVec3f xpt;
  SbVec3f xdir;
  */

  -type- invdet;     /* inverse of 2x2 matrix determinant */
  -cc_vector- xdir;
  -cc_vector- dir2;  /* holds the squares of the coordinates of xdir */
  -cc_vector- xpt, xptanddir;
  const -cc_vector- * pl1n, * pl2n;
  float pl1w, pl2w;

  -cc_vector-_cross(&plane1->normal, &plane2->normal, &xdir);

  dir2.vec[0] = xdir.vec[0] * xdir.vec[0];
  dir2.vec[1] = xdir.vec[1] * xdir.vec[1];
  dir2.vec[2] = xdir.vec[2] * xdir.vec[2];

  pl1n = &plane1->normal;
  pl2n = &plane2->normal;
  pl1w = - plane1->distance;
  pl2w = - plane2->distance;

  if ( dir2.vec[2] > dir2.vec[1] &&
       dir2.vec[2] > dir2.vec[0] &&
       dir2.vec[2] > FLT_EPSILON ) {
    /* then get a point on the XY plane */
    invdet = 1.0f / xdir.vec[2];
    -cc_vector-_set(&xpt, pl1n->vec[1] * pl2w - pl2n->vec[1] * pl1w,
                    pl2n->vec[0] * pl1w - pl1n->vec[0] * pl2w, 0.0f);
  } else if ( dir2.vec[1] > dir2.vec[0] &&
              dir2.vec[1] > FLT_EPSILON ) {
    /* then get a point on the XZ plane */
    invdet = 1.0f / xdir.vec[1];
    -cc_vector-_set(&xpt, pl1n->vec[2] * pl2w - pl2n->vec[2] * pl1w, 0.0f,
                    pl2n->vec[0] * pl1w - pl1n->vec[0] * pl2w);
  } else if ( dir2.vec[0] > FLT_EPSILON ) {
    /* then get a point on the YZ plane */
    invdet = 1.0f / xdir.vec[0];
    -cc_vector-_set(&xpt, 0.0f, pl1n->vec[2] * pl2w - pl2n->vec[2] * pl1w,
                    pl2n->vec[1] * pl1w - pl1n->vec[1] * pl2w);
  } else
    /* xdir is zero, then no point of intersection exists */
    return FALSE;

  -cc_vector-_mult_vec_scalar(&xpt, invdet, &xpt);
  invdet = 1.0f / (float) sqrt(dir2.vec[0] + dir2.vec[1] + dir2.vec[2]);

  -cc_vector-_mult_vec_scalar(&xdir, invdet, &xdir);
  -cc_vector-_add(&xpt, &xdir, &xptanddir);
  -cc_line-_set(intersection, &xpt, &xptanddir);
  return TRUE;
} /* -cc_class-_get_intersection() */

/*!
  \relates -cc_class-

  Transform the plane by \a matrix.

  \sa -cc_class-_offset().
*/

void
-cc_class-_transform(const -cc_class- * plane, const -cc_matrix- * matrix, -cc_class- * target)
{
  -cc_vector- pointinplane;
  -cc_matrix- invtransp;

  /* SbVec3f ptInPlane = this->normal * this->distance; */
  -cc_vector-_mult_vec_scalar(&plane->normal, plane->distance, &pointinplane);

  /*
  // according to discussions on comp.graphics.algorithms, the inverse
  // transpose matrix should be used to rotate the plane normal.
  */

  /*  SbMatrix invtransp = matrix.inverse().transpose(); */
  -cc_matrix-_inverse(matrix, &invtransp);
  -cc_matrix-_transpose(&invtransp, &invtransp);

  /* invtransp.multDirMatrix(this->normal, this->normal); */
  -cc_matrix-_mult_dir_matrix(&invtransp, &plane->normal, &plane->normal);

  /* the point should be transformed using the original matrix */
  /* matrix.multVecMatrix(ptInPlane, ptInPlane); */
  -cc_matrix-_mult_vec_matrix(matrix, &pointinplane, &pointinplane);

  /* this->normal.normalize(); */
  -cc_vector-_normalize(&plane->normal, &target->normal);

  /* this->distance = this->normal.dot(ptInPlane); */
  target->distance = -cc_vector-_dot(&target->normal, &pointinplane);
} /* -cc_class-_transform() */

/*!
  \relates -cc_class-

  Check if the given point lies in the halfspace of the plane which the
  plane normal vector is pointing.
*/

SbBool
-cc_class-_halfspace_upper(const -cc_class- * plane, const cc_vec3f * point)
{
  /*
  // This one is dead easy, we just take the dot product of the normal
  // vector and the vector going from the plane base point to the
  // point we're checking against, and see if the angle between the
  // vectors are within 90° (which is the same as checking the sign
  // of the dot product).
  //                                                    19980816 mortene.
  */
#if 0 // not very efficient code, disabled 19991012 pederb
  SbVec3f pointToPlaneBase = point - (this->normal * this->distance);
  float dotWithNormal = this->normal.dot(pointToPlaneBase);
  if(dotWithNormal >= 0.0f) return TRUE;
  return FALSE;
#else /* this code uses distance to plane instead */
  /* return this->getDistance(point) >= 0.0f; */
  return -cc_class-_get_distance(plane, point) >= 0.0f;
#endif /* new code */
} /* -cc_class-_halfspace_upper() */

/*!
  \relates -cc_class-

  Return the distance from \a point to plane. Positive distance means
  the point is in the plane's half space.

  This method is an extension specific to Coin versus the original SGI
  Inventor API.
*/

float
-cc_class-_get_distance(const -cc_class- * plane, const cc_vec3f * point)
{
  return -cc_vector-_dot(point, &plane->normal) - plane->distance;
} /* -cc_class-_get_distance() */

/*!
  \relates -cc_class-

  Return the plane's normal vector, which indicates which direction the plane
  is oriented.

  \sa -cc_class-_get_distance_from_origin().
*/

void
-cc_class-_get_normal(const -cc_class- * plane, cc_vec3f * normal)
{
  -cc_vector-_copy(&plane->normal, normal);
} /* -cc_class-_get_normal() */

/*!
  \relates -cc_class-

  Return distance from origin of coordinate system to the point in the plane
  which is closest to the origin.

  \sa -cc_class-_get_normal().
*/

float
-cc_class-_get_distance_from_origin(const -cc_class- * plane)
{
  return plane->distance;
} /* -cc_class-_get_distance_from_origin() */

/*!
  \relates -cc_class-

  Check the two given planes for equality.
*/

int
-cc_class-_equals(const -cc_class- * plane1, const -cc_class- * plane2)
{

  if ( plane1->distance == plane2->distance &&
       -cc_vector-_cmp(&plane1->normal, &plane2->normal) == 0 )
    return TRUE;
  return FALSE;
} /* -cc_class-_equals() */

/*!
  \relates -cc_class-

  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/

void
-cc_class-_print(const -cc_class- * plane, FILE * fp)
{
#if COIN_DEBUG
  -cc_vector-_print(&plane->normal, fp);
  /* fixme - %f for double too? */
  (void)fprintf(fp, "  %f", plane->distance);
#endif /* COIN_DEBUG */
} /* -cc_class-_print() */

