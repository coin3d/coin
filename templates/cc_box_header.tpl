//$ TEMPLATE cc_box_header(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ DEFINE -cpp_wrapper- CC_BOX-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -cxx_class- SbBox-elements--letter-
//$ DEFINE -cc_class- cc_box-elements--letter-
//$ DEFINE -cc_element- cc_vec-elements--letter-
//$ DEFINE -exp- COIN_DLL_API
#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h> /* FILE* */

#include <Inventor/C/basic.h>
#include <Inventor/C/types.h>
#include <Inventor/C/base/vec-elements--letter-.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct -cc_class- {
  -cc_element- min;
  -cc_element- max;
};

COIN_DLL_API SbBool -cc_class-_set(-cc_class- * box, const -cc_element- * min, const -cc_element- * max);
COIN_DLL_API SbBool -cc_class-_copy(const -cc_class- * src, -cc_class- * dst);

COIN_DLL_API SbBool -cc_class-_intersect_point(const -cc_class- * box, const -cc_element- * point);
COIN_DLL_API SbBool -cc_class-_intersect_box(const -cc_class- * box, const -cc_class- * box2);

COIN_DLL_API void -cc_class-_get_span(const -cc_class- * box, const -cc_element- * dir, -type- * dmin, -type- * dmax);
COIN_DLL_API void -cc_class-_transform(const -cc_class- * src, const cc_matrix4f * matrix, -cc_class- * dst);
COIN_DLL_API -type- -cc_class-_volume(const -cc_class- * box);

COIN_DLL_API SbBool -cc_class-_outside(const -cc_class- * box, const cc_matrix4f * mvp, int * cullbits);
COIN_DLL_API void -cc_class-_get_closest_point(const -cc_class- * box, const -cc_element- * point, -cc_element- * dst);

COIN_DLL_API void -cc_class-_print(const -cc_class- * box, FILE * fp);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !-cpp_wrapper- */
