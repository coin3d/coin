//$ TEMPLATE SbVec-Header(-type-, -elements-, -letter-)
//$ DEFINE -class-name- SbVec-elements--letter-
//$ DEFINE -cc-class- cc_vec-elements--letter-
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
#include <Inventor/C/base/vec-elements--letter-.h>
//$ IF ! -integertype-
//$ IF -elements- == 3
class SbPlane;
//$ ELSIF -elements- == 4
class SbVec3-letter-;
//$ ENDIF
//$ ENDIF

class COIN_DLL_API -class-name- {
public:
  -class-name-(void) {} // no initialization
  -class-name-(const -type- v[-elements-])
//$ IF -elements- == 2
    { this->vec[0] = v[0]; this->vec[1] = v[1]; }
//$ ELSIF -elements- == 3
    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; }
//$ ELSIF -elements- == 4
    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; this->vec[3] = v[3]; }
//$ ENDIF
//$ IF -elements- == 2
  -class-name-(const -type- x, const -type- y)
    { this->vec[0] = x; this->vec[1] = y; }
//$ ELSIF -elements- == 3
  -class-name-(const -type- x, const -type- y, const -type- z)
    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; }
//$ ELSIF -elements- == 4
  -class-name-(const -type- x, const -type- y, const -type- z, const -type- w)
    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; this->vec[3] = w; }
//$ ENDIF
//$ IF ! -integertype-
//$ IF -elements- == 3
  -class-name-(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);
  -class-name- cross(const -class-name- & v) const
    { -class-name- tmp;
      -cc-class-_cross(&this->vec, &v.vec, &tmp.vec);
      return tmp; }
//$ ENDIF
  -type- dot(const -class-name- & v) const
    { return -cc-class-_dot(&this->vec, &v.vec); }
  SbBool equals(const -class-name- & v, const -type- tolerance) const
    { return -cc-class-_equals(&this->vec, &v.vec, tolerance); }
  -class-name- getClosestAxis(void) const
    { -class-name- tmp;
      -cc-class-_get_closest_axis(&this->vec, &tmp.vec);
      return tmp; }
//$ IF -elements- == 4
  void getReal(SbVec3-letter- & v) const
    { -cc-class-_get_real(&this->vec, &v.vec); }
//$ ENDIF
//$ ELSE
  int32_t dot(const -class-name- & v) const
    { return -cc-class-_dot(&this->vec, &v.vec); }
//$ ENDIF
  const -type- * getValue(void) const
    { return (const -type- *) &this->vec; }
//$ IF -elements- == 2
  void getValue(-type- & x, -type- & y) const
    { x = this->vec[0]; y = this->vec[1]; }
//$ ELSIF -elements- == 3
  void getValue(-type- & x, -type- & y, -type- & z) const
    { x = this->vec[0]; y = this->vec[1]; z = this->vec[2]; }
//$ ELSIF -elements- == 4
  void getValue(-type- & x, -type- & y, -type- & z, -type- & w) const
    { x = this->vec[0]; y = this->vec[1]; z = this->vec[2]; w = this->vec[3]; }
//$ ENDIF
//$ IF ! -integertype-
  -type- length(void) const
    { return -cc-class-_length(&this->vec); }
  -type- sqrLength(void) const
    { return -cc-class-_sqr_length(&this->vec); }
//$ ENDIF
  void negate(void)
    { -cc-class-_negate(&this->vec, &this->vec); }
//$ IF ! -integertype-
  -type- normalize(void)
    { return -cc-class-_normalize(&this->vec, &this->vec); }
//$ ENDIF
  -class-name- & setValue(const -type- v[-elements-])
//$ IF -elements- == 2
    { this->vec[0] = v[0]; this->vec[1] = v[1]; return *this; }
//$ ELSIF -elements- == 3
    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; return *this; }
//$ ELSIF -elements- == 4
    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; this->vec[3] = v[3]; return *this; }
//$ ENDIF
//$ IF -elements- == 2
  -class-name- & setValue(const -type- x, const -type- y)
    { this->vec[0] = x; this->vec[1] = y; return *this; }
//$ ELSIF -elements- == 3
  -class-name- & setValue(const -type- x, const -type- y, const -type- z)
    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; return *this; }
//$ ELSIF -elements- == 4
  -class-name- & setValue(const -type- x, const -type- y, const -type- z, const -type- w)
    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; this->vec[3] = z; return *this; }
//$ ENDIF
//$ IF ! -integertype- && -elements- == 3
  -class-name- & setValue(const -class-name- & barycentric,
                     const -class-name- & v0,
                     const -class-name- & v1,
                     const -class-name- & v2);
//$ ENDIF
  -type- & operator [] (const int i)
    { return this->vec[i]; }
  const -type- & operator [] (const int i) const
    { return this->vec[i]; }
//$ IF -integertype-
  -class-name- & operator *= (int d)
    { -cc-class-_mult_vec_int(&this->vec, d, &this->vec); return *this; }
  -class-name- & operator *= (double d)
    { -cc-class-_mult_vec_dbl(&this->vec, d, &this->vec); return *this; }
  -class-name- & operator /= (int d)
    { -cc-class-_div_vec_int(&this->vec, d, &this->vec); return *this; }
  -class-name- & operator /= (double d)
    { -cc-class-_div_vec_dbl(&this->vec, d, &this->vec); return *this; }
//$ ELSE
  -class-name- & operator *= (const -type- d)
    { -cc-class-_mult_vec_scalar(&this->vec, d, &this->vec); return *this; }
  -class-name- & operator /= (const -type- d)
    { -cc-class-_div_vec_scalar(&this->vec, d, &this->vec); return *this; }
//$ ENDIF
  -class-name- & operator += (const -class-name- & u)
    { -cc-class-_add(&this->vec, &u.vec, &this->vec); return *this; }
  -class-name- & operator -= (const -class-name- & u)
    { -cc-class-_sub(&this->vec, &u.vec, &this->vec); return *this; }
  -class-name- operator - (void) const
    { -class-name- tmp; -cc-class-_negate(&this->vec, &tmp.vec); return tmp; }

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
  -cc-class- vec;
};

//$ IF -integertype-
COIN_DLL_API -class-name- operator * (const -class-name- & v, int d)
  { -class-name- tmp; -cc-class-_mult_vec_int(&v.vec, d, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator * (const -class-name- & v, double d)
  { -class-name- tmp; -cc-class-_mult_vec_dbl(&v.vec, d, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator * (int d, const -class-name- & v)
  { -class-name- tmp; -cc-class-_mult_int_vec(d, &v.vec, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator * (double d, const -class-name- & v)
  { -class-name- tmp; -cc-class-_mult_dbl_vec(d, &v.vec, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator / (const -class-name- & v, int d)
  { -class-name- tmp; -cc-class-_div_vec_int(d, &v.vec, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator / (const -class-name- & v, double d)
  { -class-name- tmp; -cc-class-_div_vec_dbl(d, &v.vec, &tmp.vec); return tmp; }
//$ ELSE
COIN_DLL_API -class-name- operator * (const -class-name- & v, const -type- d)
  { -class-name- tmp; -cc-class-_mult_vec_scalar(&v.vec, d, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator * (const -type- d, const -class-name- & v)
  { -class-name- tmp; -cc-class-_mult_scalar_vec(d, &v.vec, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator / (const -class-name- & v, const -type- d)
  { -class-name- tmp; -cc-class-_div_vec_scalar(&v.vec, d, &tmp.vec); return tmp; }
//$ ENDIF
COIN_DLL_API -class-name- operator + (const -class-name- & v1, const -class-name- & v2)
  { -class-name- tmp; -cc-class-_add(&v1.vec, &v2.vec, &tmp.vec); return tmp; }
COIN_DLL_API -class-name- operator - (const -class-name- & v1, const -class-name- & v2)
  { -class-name- tmp; -cc-class-_sub(&v1.vec, &v2.vec, &tmp.vec); return tmp; }
COIN_DLL_API int operator == (const -class-name- & v1, const -class-name- & v2)
  { return -cc-class-_cmp(&v1.vec, &v2.vec) == 0; }
COIN_DLL_API int operator != (const -class-name- & v1, const -class-name- & v2)
  { return -cc-class-_cmp(&v1.vec, &v2.vec); }

#endif // !-cpp-wrapper-
