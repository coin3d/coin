//$ TEMPLATE SbVec-Header(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbVec-elements--letter-
//$ DEFINE -cpp-wrapper- COIN_SBVEC-elements-/*$ EVAL(uc "-letter-") */_H
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
#ifndef -cpp-wrapper-
#define -cpp-wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h>

#include <Inventor/SbBasic.h>
//$ IF -integertype- && 0
#include <Inventor/system/inttypes.h>
//$ ENDIF
//$ IF ! -integertype-
//$ IF -elements- == 3
class SbPlane;
//$ ELSIF -elements- == 4
class SbVec3-letter-;
//$ ENDIF
//$ ENDIF

class COIN_DLL_API -class-name- {
public:
  -class-name-(void);
  -class-name-(const -type- v[-elements-]);
//$ IF -elements- == 2
  -class-name-(const -type- x, const -type- y);
//$ ELSIF -elements- == 3
  -class-name-(const -type- x, const -type- y, const -type- z);
//$ ELSIF -elements- == 4
  -class-name-(const -type- x, const -type- y, const -type- z, const -type- w);
//$ ENDIF
//$ IF ! -integertype-
//$ IF -elements- == 3
  -class-name-(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);
  -class-name- cross(const -class-name- & v) const;
//$ ENDIF
  -type- dot(const -class-name- & v) const;
  SbBool equals(const -class-name- & v, const -type- tolerance) const;
  -class-name- getClosestAxis(void) const;
//$ IF -elements- == 4
  void getReal(SbVec3-letter- & v) const;
//$ ENDIF
//$ ELSE
  int32_t dot(const -class-name- & v) const;
//$ ENDIF
  const -type- * getValue(void) const;
//$ IF -elements- == 2
  void getValue(-type- & x, -type- & y) const;
//$ ELSIF -elements- == 3
  void getValue(-type- & x, -type- & y, -type- & z) const;
//$ ELSIF -elements- == 4
  void getValue(-type- & x, -type- & y, -type- & z, -type- & w) const;
//$ ENDIF
//$ IF ! -integertype-
  -type- length(void) const;
  -type- sqrLength(void) const;
//$ ENDIF
  void negate(void);
//$ IF ! -integertype-
  -type- normalize(void);
//$ ENDIF
  -class-name- & setValue(const -type- v[-elements-]);
//$ IF -elements- == 2
  -class-name- & setValue(const -type- x, const -type- y);
//$ ELSIF -elements- == 3
  -class-name- & setValue(const -type- x, const -type- y, const -type- z);
//$ ELSIF -elements- == 4
  -class-name- & setValue(const -type- x, const -type- y, const -type- z, const -type- w);
//$ ENDIF
//$ IF ! -integertype- && -elements- == 3
  -class-name- & setValue(const -class-name- & barycentric,
                     const -class-name- & v0,
                     const -class-name- & v1,
                     const -class-name- & v2);
//$ ENDIF
  -type- & operator [] (const int i);
  const -type- & operator [] (const int i) const;
//$ IF -integertype-
  -class-name- & operator *= (int d);
  -class-name- & operator *= (double d);
  -class-name- & operator /= (int d);
  -class-name- & operator /= (double d);
//$ ELSE
  -class-name- & operator *= (const -type- d);
  -class-name- & operator /= (const -type- d);
//$ ENDIF
  -class-name- & operator += (const -class-name- & u);
  -class-name- & operator -= (const -class-name- & u);
  -class-name- operator - (void) const;

//$ IF -integertype-
  friend COIN_DLL_API -class-name- operator * (const -class-name- & v, int d);
  friend COIN_DLL_API -class-name- operator * (const -class-name- & v, double d);
  friend COIN_DLL_API -class-name- operator * (int d, const -class-name- & v);
  friend COIN_DLL_API -class-name- operator * (double d, const -class-name- & v);
  friend COIN_DLL_API -class-name- operator / (const -class-name- & v, int d);
  friend COIN_DLL_API -class-name- operator / (const -class-name- & v, double d);
//$ ELSE
  friend COIN_DLL_API -class-name- operator * (const -class-name- & v, const -type- d);
  friend COIN_DLL_API -class-name- operator * (const -type- d, const -class-name- & v);
  friend COIN_DLL_API -class-name- operator / (const -class-name- & v, const -type- d);
//$ ENDIF
  friend COIN_DLL_API -class-name- operator + (const -class-name- & v1, const -class-name- & v2);
  friend COIN_DLL_API -class-name- operator - (const -class-name- & v1, const -class-name- & v2);
  friend COIN_DLL_API int operator == (const -class-name- & v1, const -class-name- & v2);
  friend COIN_DLL_API int operator != (const -class-name- & v1, const -class-name- & v2);

  void print(FILE * fp) const;

private:
  -type- vec[-elements-];
};

//$ IF -integertype-
COIN_DLL_API -class-name- operator * (const -class-name- & v, int d);
COIN_DLL_API -class-name- operator * (const -class-name- & v, double d);
COIN_DLL_API -class-name- operator * (int d, const -class-name- & v);
COIN_DLL_API -class-name- operator * (double d, const -class-name- & v);
COIN_DLL_API -class-name- operator / (const -class-name- & v, int d);
COIN_DLL_API -class-name- operator / (const -class-name- & v, double d);
//$ ELSE
COIN_DLL_API -class-name- operator * (const -class-name- & v, const -type- d);
COIN_DLL_API -class-name- operator * (const -type- d, const -class-name- & v);
COIN_DLL_API -class-name- operator / (const -class-name- & v, const -type- d);
//$ ENDIF
COIN_DLL_API -class-name- operator + (const -class-name- & v1, const -class-name- & v2);
COIN_DLL_API -class-name- operator - (const -class-name- & v1, const -class-name- & v2);
COIN_DLL_API int operator == (const -class-name- & v1, const -class-name- & v2);
COIN_DLL_API int operator != (const -class-name- & v1, const -class-name- & v2);

// *************************************************************************

//$ IMPORT INLINE ../../src/base/-class-name-.cpp

// *************************************************************************

#endif // !-cpp-wrapper-
