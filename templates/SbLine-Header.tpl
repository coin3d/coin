//$ TEMPLATE SbLine-Header(-type-, -elements-, -letter-)
//$ IF "-elements--letter-" eq "3f"
//$ DEFINE -class-name- SbLine
//$ ELSE
//$ DEFINE -class-name- SbLine-elements--letter-
//$ ENDIF
//$ DEFINE -cc-class- cc_line-elements--letter-
//$ DEFINE -cpp-wrapper- COIN_SBLINE-elements-/*$ EVAL(uc "-letter-") */_H
//$ DEFINE -element- SbVec-elements--letter-
//$ DEFINE -cc-element- cc_vec-elements--letter-
//$ IF "-type-" =~ m/^(float|double|long double)$/
//$ DEFINE -integertype- 0
//$ ELSE
//$ DEFINE -integertype- 1
//$ ENDIF
#ifndef -cpp-wrapper-
#define -cpp-wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h>

#include <Inventor/C/base/line-elements--letter-.h>

#include <Inventor/SbVec-elements--letter-.h>

typedef -class-name- SbLine3f; //$ IF "-class-name-" eq "SbLine"

class COIN_DLL_API -class-name- : public -cc-class- {
public:
  -class-name-(void) { }
  -class-name-(const -class-name- & orig)
    { -cc-class-_copy(&orig, this); }
  -class-name-(const -element- & origin, const -element- & point)
    { this->setValue(origin, point); }
  ~-class-name-(void) { }

  void setValue(const -element- & origin, const -element- & point)
    { -cc-element-_copy(&origin, &this->pos);
      -cc-element-_sub(&point, &origin, &this->dir);
      -cc-element-_normalize(&this->dir, &this->dir); }
  -element- getClosestPoint(const -element- & point) const
    { -element- found;
      -cc-class-_find_closest_point(this, &point, &found);
      return found; }
  SbBool getClosestPoints(const -class-name- & line2, -element- & ptOnThis, -element- & ptOnLine2) const
    { return -cc-class-_find_closest_points(this, &line2, &ptOnThis, &ptOnLine2); }

  const -element- & getPosition(void) const
    { return *((const -element- *) &this->pos); }
  const -element- & getDirection(void) const
    { return *((-element- *) &this->dir); }

  void print(FILE * fp) const
    { -cc-class-_print(this, fp); }

}; // class -class-name-

#endif // !-cpp-wrapper-
