//$ TEMPLATE cc_plane_header(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ DEFINE -cpp_wrapper- CC_PLANE-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -cxx_class- SbPlane
//$ DEFINE -cc_class- cc_plane-elements--letter-
//$ DEFINE -cc_vector- cc_vec-elements--letter-
//$ DEFINE -exp- COIN_DLL_API
#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <Inventor/C/basic.h>
#include <Inventor/C/types.h>
#include <Inventor/C/base/vec-elements--letter-.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct -cc_class- {
  -cc_vector- normal;
  -type- distance;
};

-exp- void -cc_class-_set(-cc_class- * plane, const -cc_vector- * normal, const -type- distance);
-exp- void -cc_class-_set_from_points(-cc_class- * plane, const -cc_vector- * p0, const -cc_vector- * p1, const -cc_vector- * p2);
-exp- void -cc_class-_set_from_position(-cc_class- * plane, const -cc_vector- * point, const -cc_vector- * normal);
-exp- void -cc_class-_copy(const -cc_class- * src, -cc_class- * dst);

-exp- void -cc_class-_offset(-cc_class- * plane, const -type- offset);

-exp- SbBool -cc_class-_intersect(const -cc_class- * plane, const cc_line3f * line, -cc_vector- * where);
-exp- void -cc_class-_transform(const -cc_class- * plane, const cc_mat4f * matrix, -cc_class- * target);
-exp- SbBool -cc_class-_halfspace_upper(const -cc_class- * plane, const -cc_vector- * point);
-exp- float -cc_class-_get_distance(const -cc_class- * plane, const -cc_vector- * point);
-exp- void -cc_class-_get_normal(const -cc_class- * plane, -cc_vector- * normal);
-exp- float -cc_class-_get_distance_from_origo(const -cc_class- * plane);

-exp- int -cc_class-_equals(const -cc_class- * plane1, const -cc_class- * plane2);

-exp- void -cc_class-_print(const -cc_class- * plane, FILE * fp);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !-cpp_wrapper- */
