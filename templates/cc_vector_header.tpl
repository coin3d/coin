//$ TEMPLATE cc_vector_header(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ DEFINE -cpp_wrapper- CC_VEC-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -cxx_class- SbVec-elements--letter-
//$ DEFINE -cc_class- cc_vec-elements--letter-
//$ DEFINE -exp- COIN_DLL_API
#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h>

#include <Inventor/C/basic.h>
#include <Inventor/C/types.h>
#include <Inventor/C/base/vec/*$ EVAL(-elements--1)*/-letter-.h> //$ IF -elements- > 2

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct -cc_class- {
  -type- vec[-elements-];
};


-exp- void -cc_class-_set(-cc_class- * vec, const -type- x
//$ APPEND , const -type- y //$ IF -elements- > 1
//$ APPEND , const -type- z //$ IF -elements- > 2
//$ APPEND , const -type- w //$ IF -elements- > 3
//$ APPEND );
-exp- void -cc_class-_copy(const -cc_class- * src, -cc_class- * dst);

-exp- void -cc_class-_cross(const -cc_class- * v1, const -cc_class- * v2, -cc_class- * dst);
-exp- -type- -cc_class-_dot(const -cc_class- * v1, const -cc_class- * v2);
-exp- void -cc_class-_find_closest_axis(const -cc_class- * v, -cc_class- * dst);
//$ IF ! -integertype-
-exp- void -cc_class-_find_real(const -cc_class- * v, cc_vec/*$ EVAL(-elements--1)*/-letter- * dst); //$ IF -elements- > 2
/*
-exp- void -cc_class-_find_from_planes(..., -cc_class- * dst); //$ IF ! -integertype-
*/
//$ ENDIF
-exp- -type- -cc_class-_length(const -cc_class- * vec); //$ IF ! -integertype-
-exp- -type- -cc_class-_sqr_length(const -cc_class- * vec);
-exp- void -cc_class-_negate(const -cc_class- * src, -cc_class- * dst);
-exp- -type- -cc_class-_normalize(const -cc_class- * src, -cc_class- * dst); //$ IF ! -integertype-

-exp- SbBool -cc_class-_equals(const -cc_class- * v1, const -cc_class- * v2, const -type- tolerance); //$ IF ! -integertype-
-exp- int -cc_class-_cmp(const -cc_class- * v1, const -cc_class- * v2);

//$ IF -integertype-
-exp- void -cc_class-_mult_vec_int(const -cc_class- * v, const int d, -cc_class- * dst);
-exp- void -cc_class-_mult_vec_dbl(const -cc_class- * v, const double d, -cc_class- * dst);
-exp- void -cc_class-_mult_int_vec(const int d, const -cc_class- * v, -cc_class- * dst);
-exp- void -cc_class-_mult_dbl_vec(const double d, const -cc_class- * v, -cc_class- * dst);
-exp- void -cc_class-_div_vec_int(const -cc_class- * v, const int d, -cc_class- * dst);
-exp- void -cc_class-_div_vec_dbl(const -cc_class- * v, const double d, -cc_class- * dst);
//$ ELSE
-exp- void -cc_class-_mult_vec_scalar(const -cc_class- * v, const -type- d, -cc_class- * dst);
-exp- void -cc_class-_mult_scalar_vec(const -type- d, const -cc_class- * v, -cc_class- * dst);
-exp- void -cc_class-_div_vec_scalar(const -cc_class- * v, const -type- d, -cc_class- * dst);
//$ ENDIF
-exp- void -cc_class-_add(const -cc_class- * v1, const -cc_class- * v2, -cc_class- * dst);
-exp- void -cc_class-_sub(const -cc_class- * v1, const -cc_class- * v2, -cc_class- * dst);

-exp- void -cc_class-_print(const -cc_class- * vec, FILE * fp);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !-cpp_wrapper- */
