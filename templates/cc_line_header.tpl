//$ TEMPLATE cc_line_header(-type-, -elements-, -letter-)
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
//$ DEFINE -cpp_wrapper- CC_LINE-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -cxx_class- SbLine
//$ DEFINE -cc_class- cc_line-elements--letter-
//$ DEFINE -cc_element- cc_vec-elements--letter-
//$ DEFINE -exp- COIN_DLL_API
#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h> /* FILE */

#include <Inventor/C/basic.h>
#include <Inventor/C/types.h>
#include <Inventor/C/base/vec-elements--letter-.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct -cc_class- {
  cc_vec-elements--letter- pos;
  cc_vec-elements--letter- dir;
};

-exp- void -cc_class-_set(-cc_class- * line, const -cc_element- * pos, const -cc_element- * dir);
-exp- void -cc_class-_copy(const -cc_class- * src, -cc_class- * dst);

-exp- void -cc_class-_find_closest_point(const -cc_class- * line, const -cc_element- * point, -cc_element- * found);
-exp- SbBool -cc_class-_find_closest_points(const -cc_class- * line1, const -cc_class- * line2, -cc_element- * found1, -cc_element- * found2);

-exp- void -cc_class-_print(const -cc_class- * line, FILE * fp);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !-cpp_wrapper- */
