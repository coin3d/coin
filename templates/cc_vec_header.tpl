//$ TEMPLATE cc_vec_header(-type-, -elements-, -letter-)
//$ DEFINE -cpp-wrapper- CC_VEC-elements-/*$ EVAL(uc "-letter-") */_H
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ DEFINE -cc-class- cc_vec-elements--letter-
#ifndef -cpp-wrapper-
#define -cpp-wrapper-

//$ INSERT TEMPLATE Copyright

#include <Inventor/SbBasic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef -type- -cc-class-[-elements-];

//$ IF ! -integertype-
COIN_DLL_API void -cc-class-_cross(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst);
COIN_DLL_API -type- -cc-class-_dot(const -cc-class- * v1, const -cc-class- * v2);
COIN_DLL_API SbBool -cc-class-_equals(const -cc-class- * v1, const -cc-class- * v2, const -type- tolerance);
COIN_DLL_API void -cc-class-_get_closest_axis(const -cc-class- * v, -cc-class- * dst);
//$ IF -elements- == 4
COIN_DLL_API void -cc-class-_get_real(const -cc-class- * v, -cc-class- * dst);
//$ ENDIF
COIN_DLL_API -type- -cc-class-_length(const -cc-class- * vec);
COIN_DLL_API -type- -cc-class-_sqr_length(const -cc-class- * vec);
COIN_DLL_API void -cc-class-_negate(const -cc-class- * src, -cc-class- * dst);
COIN_DLL_API -type- -cc-class-_normalize(const -cc-class- * src, -cc-class- * dst);
//$ ENDIF
COIN_DLL_API int -cc-class-_cmp(const -cc-class- * v1, const -cc-class- * v2);

//$ IF -integertype-
COIN_DLL_API void -cc-class-_mult_vec_int(const -cc-class- * v, const int d, -cc-class- * dst);
COIN_DLL_API void -cc-class-_mult_vec_dbl(const -cc-class- * v, const double d, -cc-class- * dst);
COIN_DLL_API void -cc-class-_mult_int_vec(const int d, const -cc-class- * v, -cc-class- * dst);
COIN_DLL_API void -cc-class-_mult_dbl_vec(const double d, const -cc-class- * v, -cc-class- * dst);
COIN_DLL_API void -cc-class-_div_vec_int(const -cc-class- * v, const int d, -cc-class- * dst);
COIN_DLL_API void -cc-class-_div_vec_dbl(const -cc-class- * v, const double d, -cc-class- * dst);
//$ ELSE
COIN_DLL_API void -cc-class-_mult_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst);
COIN_DLL_API void -cc-class-_mult_scalar_vec(const -type- d, const -cc-class- * v, -cc-class- * dst);
COIN_DLL_API void -cc-class-_div_vec_scalar(const -cc-class- * v, const -type- d, -cc-class- * dst);
//$ ENDIF
COIN_DLL_API void -cc-class-_add(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst);
COIN_DLL_API void -cc-class-_sub(const -cc-class- * v1, const -cc-class- * v2, -cc-class- * dst);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !-cpp-wrapper- */
