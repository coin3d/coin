//$ TEMPLATE SbBox-Header(-type-, -elements-, -letter-)
//$ DEFINE -cpp_wrapper- COIN_SBBOX-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -cxx_class- SbBox-elements--letter-
//$ DEFINE -cc_class- cc_box-elements--letter-
//$ DEFINE -cpp-element- SbVec-elements--letter-
//$ DEFINE -cc-element- cc_vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <Inventor/SbBasic.h>
#include <Inventor/C/base/box-elements--letter-.h>
#include <Inventor/SbVec-elements--letter-.h>

//$ IF "-elements--letter-" eq "3f"
//$ DEFINE -matrix-class- SbMatrix
//$ ELSE
//$ DEFINE -matrix-class- SbMatrix/*$ EVAL(-elements-+1)*/-letter-
//$ ENDIF
class -matrix-class-;

class COIN_DLL_API -cxx_class- : public -cc_class- {
public:
  -cxx_class-(void) { }
  -cxx_class-(const -type- minx, const -type- miny
//$ APPEND , const -type- minz //$ IF -elements- >= 3
//$ APPEND , const -type- minw //$ IF -elements- >= 4
//$ APPEND ,
          const -type- maxx, const -type- maxy
//$ APPEND , const -type- maxz //$ IF -elements- >= 3
//$ APPEND , const -type- maxw //$ IF -elements- >= 4
//$ APPEND )
    { -cc-element-_set(&this->min, minx, miny
//$ APPEND , minz //$ IF -elements- >= 3
//$ APPEND , minw //$ IF -elements- >= 4
//$ APPEND );
      -cc-element-_set(&this->max, maxx, maxy
//$ APPEND , maxz //$ IF -elements- >= 3
//$ APPEND , maxw //$ IF -elements- >= 4
//$ APPEND ); }
  -cxx_class-(const -cpp-element- & min, const -cpp-element- & max)
    { -cc-element-_copy(&min, &this->min);
      -cc-element-_copy(&max, &this->max); }
  ~-cxx_class-(void) { }

  const -cpp-element- & getMin(void) const
    { return *((const -cpp-element- *) &this->min); }
  const -cpp-element- & getMax(void) const
    { return *((const -cpp-element- *) &this->max); }
  -cpp-element- & getMin(void)
    { return *((-cpp-element- *) &this->min); }
  -cpp-element- & getMax(void)
    { return *((-cpp-element- *) &this->max); }

  -cpp-element- getCenter(void) const
    { -cpp-element- center;
      -cc-element-_add(&this->min, &this->max, &center);
      -cc-element-_div_vec_dbl(&center, 2.0, &center); //$ IF -integertype-
      -cc-element-_div_vec_scalar(&center, 2.0, &center); //$ IF !-integertype-
      return center; }

  void extendBy(const -cpp-element- & point);
  void extendBy(const -cxx_class- & box);

  SbBool intersect(const -cpp-element- & point) const;
  SbBool intersect(const -cxx_class- & box) const;

  void setBounds(const -type- minx, const -type- miny
//$ APPEND , const -type- minz //$ IF -elements- >= 3
//$ APPEND , const -type- minw //$ IF -elements- >= 4
//$ APPEND ,
                 const -type- maxx, const -type- maxy
//$ APPEND , const -type- maxz //$ IF -elements- >= 3
//$ APPEND , const -type- maxw //$ IF -elements- >= 4
//$ APPEND );
  void setBounds(const -cpp-element- & min, const -cpp-element- & max);
  void getBounds(-type- & minx, -type- & miny
//$ APPEND , -type- & minz //$ IF -elements- >= 3
//$ APPEND , -type- & minw //$ IF -elements- >= 4
//$ APPEND ,
                 -type- & maxx, -type- & maxy
//$ APPEND , -type- & maxz //$ IF -elements- >= 3
//$ APPEND , -type- & maxw //$ IF -elements- >= 4
//$ APPEND ) const;
  void getBounds(-cpp-element- & min, -cpp-element- & max) const;

  void getOrigin(-type- & x0, -type- & y0
//$ APPEND , -type- & z0 //$ IF -elements- >= 3
//$ APPEND , -type- & w0 //$ IF -elements- >= 4
//$ APPEND ) const;
  void getSize(-type- & dx, -type- & dy
//$ APPEND , -type- & dz //$ IF -elements- >= 3
//$ APPEND , -type- & dw //$ IF -elements- >= 4
//$ APPEND ) const;

  void makeEmpty(void);
  SbBool isEmpty(void) const;
  SbBool hasVolume(void) const;

  void getSpan(const -cpp-element- & dir, -type- & dmin, -type- & dmax) const;
  void transform(const -matrix-class- & matrix);
  -type- getVolume(void) const;

  SbBool outside(const -matrix-class- & mvp, int & cullbits) const;
  -cpp-element- getClosestPoint(const -cpp-element- & point) const;

  friend COIN_DLL_API int operator == (const -cxx_class- & b1, const -cxx_class- & b2);
  friend COIN_DLL_API int operator != (const -cxx_class- & b1, const -cxx_class- & b2);

  void print(FILE * fp) const
    { -cc_class-_print(this, fp); }

private:

};

COIN_DLL_API int operator == (const -cxx_class- & b1, const -cxx_class- & b2);
COIN_DLL_API int operator != (const -cxx_class- & b1, const -cxx_class- & b2);

#endif // !-cpp_wrapper-
