//$ TEMPLATE cc_box_source(-type-, -elements-, -letter-)
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

#include <Inventor/C/base/box-elements--letter-.h>

/* ********************************************************************** */

