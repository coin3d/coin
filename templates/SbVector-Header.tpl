//$ TEMPLATE SbVector-Header(-type-, -elements-, -letter-)
//$ DEFINE -cxx_class- SbVec-elements--letter-
//$ DEFINE -cc-class- cc_vec-elements--letter-
//$ DEFINE -cpp-wrapper- COIN_SBVEC-elements-/*$ EVAL(uc "-letter-") */_H
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ IF "-letter-" eq "f"
//$ DEFINE -comp_letter- d
//$ ELSE
//$ DEFINE -comp_letter- f
//$ ENDIF
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
#ifndef -cpp-wrapper-
#define -cpp-wrapper-

//$ INSERT TEMPLATE Copyright

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h> //$ IF -integertype- && 0
#include <Inventor/C/types.h>
#include <Inventor/C/base/vec-elements--letter-.h>
//$ IF ! -integertype-
class SbPlane; //$ IF -elements- == 3
#include <Inventor/SbVec/*$ EVAL(-elements--1)*/-letter-.h> //$ IF -elements- > 2
class SbVec-elements--comp_letter-; //$ IF ! -integertype-
//$ ENDIF

class COIN_DLL_API -cxx_class- : public -cc-class- {
public:
  -cxx_class-(void) {} // no initialization
//$ CASE -elements-:
2:  -cxx_class-(const -type- v[-elements-])
2:    { this->vec[0] = v[0]; this->vec[1] = v[1]; }
3:  -cxx_class-(const -type- v[-elements-])
3:    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; }
4:  -cxx_class-(const -type- v[-elements-])
4:    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; this->vec[3] = v[3]; } //$ IF -elements- == 4
2:  -cxx_class-(const -type- x, const -type- y)
2:    { this->vec[0] = x; this->vec[1] = y; }
3:  -cxx_class-(const -type- x, const -type- y, const -type- z)
3:    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; }
4:  -cxx_class-(const -type- x, const -type- y, const -type- z, const -type- w)
4:    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; this->vec[3] = w; }
//$ ESAC
//$ IF ! -integertype-
//$ IF -elements- == 3
//  -cxx_class-(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2)
//    { -cc-class-_find_from_planes(p0, p1, p2, this); }
//  -cxx_class-(const SbVec#o & vec) { this->setValue(vec); }
  -cxx_class- cross(const -cxx_class- & v) const
    { -cxx_class- tmp;
      -cc-class-_cross(this, &v, &tmp);
      return tmp; }
//$ ENDIF
  -type- dot(const -cxx_class- & v) const
    { return -cc-class-_dot(this, &v); }
  SbBool equals(const -cxx_class- & v, const -type- tolerance) const
    { return -cc-class-_equals(this, &v, tolerance); }
  -cxx_class- getClosestAxis(void) const
    { -cxx_class- tmp;
      -cc-class-_find_closest_axis(this, &tmp);
      return tmp; }
//$ IF -elements- > 2
  void getReal(SbVec/*$ EVAL(-elements--1)*/-letter- & v) const
    { -cc-class-_find_real(this, &v); }
//$ ENDIF
//$ ELSE
  int32_t dot(const -cxx_class- & v) const
    { return -cc-class-_dot(this, &v); }
//$ ENDIF
  const -type- * getValue(void) const
    { return (const -type- *) &this->vec; }
//$ CASE -elements-:
2:  void getValue(-type- & x, -type- & y) const
2:    { x = this->vec[0]; y = this->vec[1]; }
3:  void getValue(-type- & x, -type- & y, -type- & z) const
3:    { x = this->vec[0]; y = this->vec[1]; z = this->vec[2]; }
4:  void getValue(-type- & x, -type- & y, -type- & z, -type- & w) const
4:    { x = this->vec[0]; y = this->vec[1]; z = this->vec[2]; w = this->vec[3]; }
//$ ESAC
//$ IF ! -integertype-
  -type- length(void) const
    { return -cc-class-_length(this); }
  -type- sqrLength(void) const
    { return -cc-class-_sqr_length(this); }
//$ ENDIF
  void negate(void)
    { -cc-class-_negate(this, this); }
  -type- normalize(void) //$ IF ! -integertype-
    { return -cc-class-_normalize(this, this); } //$ IF ! -integertype-
//$ CASE -elements-:
2:  -cxx_class- & setValue(const -type- v[-elements-])
2:    { this->vec[0] = v[0]; this->vec[1] = v[1]; return *this; }
2:  -cxx_class- & setValue(const -type- x, const -type- y)
2:    { this->vec[0] = x; this->vec[1] = y; return *this; }
3:  -cxx_class- & setValue(const -type- v[-elements-])
3:    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; return *this; }
3:  -cxx_class- & setValue(const -type- x, const -type- y, const -type- z)
3:    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; return *this; }
4:  -cxx_class- & setValue(const -type- v[-elements-])
4:    { this->vec[0] = v[0]; this->vec[1] = v[1]; this->vec[2] = v[2]; this->vec[3] = v[3]; return *this; }
4:  -cxx_class- & setValue(const -type- x, const -type- y, const -type- z, const -type- w)
4:    { this->vec[0] = x; this->vec[1] = y; this->vec[2] = z; this->vec[3] = z; return *this; }
//$ ESAC
//$ IF ! -integertype-
//$ IF -elements- == 3
  -cxx_class- & setValue(const -cxx_class- & barycentric,
                     const -cxx_class- & v0,
                     const -cxx_class- & v1,
                     const -cxx_class- & v2);
//$ ENDIF
//$ IF ! -integertype-
  -cxx_class- & setValue(const SbVec-elements--comp_letter- & vec);
//$ ENDIF
//$ ENDIF
  -type- & operator [] (const int i)
    { return this->vec[i]; }
  const -type- & operator [] (const int i) const
    { return this->vec[i]; }
//$ IF -integertype-
  -cxx_class- & operator *= (int d)
    { -cc-class-_mult_vec_int(this, d, this); return *this; }
  -cxx_class- & operator *= (double d)
    { -cc-class-_mult_vec_dbl(this, d, this); return *this; }
  -cxx_class- & operator /= (int d)
    { -cc-class-_div_vec_int(this, d, this); return *this; }
  -cxx_class- & operator /= (double d)
    { -cc-class-_div_vec_dbl(this, d, this); return *this; }
//$ ELSE
  -cxx_class- & operator *= (const -type- d)
    { -cc-class-_mult_vec_scalar(this, d, this); return *this; }
  -cxx_class- & operator /= (const -type- d)
    { -cc-class-_div_vec_scalar(this, d, this); return *this; }
//$ ENDIF
  -cxx_class- & operator += (const -cxx_class- & u)
    { -cc-class-_add(this, &u, this); return *this; }
  -cxx_class- & operator -= (const -cxx_class- & u)
    { -cc-class-_sub(this, &u, this); return *this; }
  -cxx_class- operator - (void) const
    { -cxx_class- tmp; -cc-class-_negate(this, &tmp); return tmp; }

//$ IF -integertype-
  friend COIN_DLL_API -cxx_class- operator * (const -cxx_class- & v, int d);
  friend COIN_DLL_API -cxx_class- operator * (const -cxx_class- & v, double d);
  friend COIN_DLL_API -cxx_class- operator * (int d, const -cxx_class- & v);
  friend COIN_DLL_API -cxx_class- operator * (double d, const -cxx_class- & v);
  friend COIN_DLL_API -cxx_class- operator / (const -cxx_class- & v, int d);
  friend COIN_DLL_API -cxx_class- operator / (const -cxx_class- & v, double d);
//$ ELSE
  friend COIN_DLL_API -cxx_class- operator * (const -cxx_class- & v, const -type- d);
  friend COIN_DLL_API -cxx_class- operator * (const -type- d, const -cxx_class- & v);
  friend COIN_DLL_API -cxx_class- operator / (const -cxx_class- & v, const -type- d);
//$ ENDIF
  friend COIN_DLL_API -cxx_class- operator + (const -cxx_class- & v1, const -cxx_class- & v2);
  friend COIN_DLL_API -cxx_class- operator - (const -cxx_class- & v1, const -cxx_class- & v2);
  friend COIN_DLL_API int operator == (const -cxx_class- & v1, const -cxx_class- & v2);
  friend COIN_DLL_API int operator != (const -cxx_class- & v1, const -cxx_class- & v2);

  void print(FILE * fp) const
    { -cc-class-_print(this, fp); }

};

//$ IF -integertype-
inline -cxx_class- operator * (const -cxx_class- & v, int d)
  { -cxx_class- tmp; -cc-class-_mult_vec_int(&v, d, &tmp); return tmp; }
inline -cxx_class- operator * (const -cxx_class- & v, double d)
  { -cxx_class- tmp; -cc-class-_mult_vec_dbl(&v, d, &tmp); return tmp; }
inline -cxx_class- operator * (int d, const -cxx_class- & v)
  { -cxx_class- tmp; -cc-class-_mult_int_vec(d, &v, &tmp); return tmp; }
inline -cxx_class- operator * (double d, const -cxx_class- & v)
  { -cxx_class- tmp; -cc-class-_mult_dbl_vec(d, &v, &tmp); return tmp; }
inline -cxx_class- operator / (const -cxx_class- & v, int d)
  { -cxx_class- tmp; -cc-class-_div_vec_int(&v, d, &tmp); return tmp; }
inline -cxx_class- operator / (const -cxx_class- & v, double d)
  { -cxx_class- tmp; -cc-class-_div_vec_dbl(&v, d, &tmp); return tmp; }
//$ ELSE
inline -cxx_class- operator * (const -cxx_class- & v, const -type- d)
  { -cxx_class- tmp; -cc-class-_mult_vec_scalar(&v, d, &tmp); return tmp; }
inline -cxx_class- operator * (const -type- d, const -cxx_class- & v)
  { -cxx_class- tmp; -cc-class-_mult_scalar_vec(d, &v, &tmp); return tmp; }
inline -cxx_class- operator / (const -cxx_class- & v, const -type- d)
  { -cxx_class- tmp; -cc-class-_div_vec_scalar(&v, d, &tmp); return tmp; }
//$ ENDIF
inline -cxx_class- operator + (const -cxx_class- & v1, const -cxx_class- & v2)
  { -cxx_class- tmp; -cc-class-_add(&v1, &v2, &tmp); return tmp; }
inline -cxx_class- operator - (const -cxx_class- & v1, const -cxx_class- & v2)
  { -cxx_class- tmp; -cc-class-_sub(&v1, &v2, &tmp); return tmp; }
inline int operator == (const -cxx_class- & v1, const -cxx_class- & v2)
  { return -cc-class-_cmp(&v1, &v2) == 0; }
inline int operator != (const -cxx_class- & v1, const -cxx_class- & v2)
  { return -cc-class-_cmp(&v1, &v2); }

#endif // !-cpp-wrapper-
