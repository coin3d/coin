//$ TEMPLATE SbPlane-Header(-type-, -elements-, -letter-)
//$ DEFINE -cpp_wrapper- COIN_SBPLANE-elements-/*$ EVAL(uc "-letter-")*/_H
//$ DEFINE -cxx_class- SbPlane
//$ DEFINE -cxx_element- SbVec3f
//$ DEFINE -cc_class- cc_plane3f

#ifndef -cpp_wrapper-
#define -cpp_wrapper-

//$ INSERT TEMPLATE Copyright

#include <stdio.h>

#include <Inventor/-cxx_element-.h>
#include <Inventor/C/base/plane-elements--letter-.h>

class SbLine;
class SbMatrix;

class COIN_DLL_API -cxx_class- : public -cc_class- {
public:
  -cxx_class-(void) { }
  -cxx_class-(const -cxx_element- & normal, const float D)
    { this->setValue(normal, D); }
  -cxx_class-(const -cxx_element- & p0, const -cxx_element- & p1, const -cxx_element- & p2)
    { this->setValue(p0, p1, p2); }
  -cxx_class-(const -cxx_element- & normal, const -cxx_element- & point)
    { this->setValue(normal, point); }

  -cxx_class- & setValue(const -cxx_element- & normal, const float D)
    { -cc_class-_set(this, &normal, D); return *this; }
  -cxx_class- & setValue(const -cxx_element- & p0, const -cxx_element- & p1, const -cxx_element- & p2)
    { -cc_class-_set_from_points(this, &p0, &p1, &p2); return *this; }
  -cxx_class- & setValue(const -cxx_element- & normal, const -cxx_element- & point)
    { -cc_class-_set_from_position(this, &normal, &point); return *this; }

  void offset(const float d)
    { -cc_class-_offset(this, d); }
  void transform(const SbMatrix & matrix)
    { -cc_class-_transform(this, &matrix); }
  SbBool isInHalfSpace(const -cxx_element- & point) const
    { return -cc_class-_upper_halfspace(this, &point); }
  const -cxx_element- & getNormal(void) const
    { return (*(-cxx_element- *) &this->normal); }
  float getDistance(const -cxx_element- & point) const
    { return -cc_class-_get_distance(this, &point); }
  float getDistanceFromOrigin(void) const
    { return -cc_class-_get_distance_from_origin(this); }
  SbBool intersect(const SbLine & l, -cxx_element- & intersection) const
    { return -cc_class-_intersect(this, &l, &intersection); }
  SbBool intersect(const -cxx_class- & pl, SbLine & line)
    { return -cc_class-_get_intersection(this, &pl, &line); }

  void print(FILE * file) const
    { -cc_class-_print(this, file); }

  friend int operator == (const -cxx_class- & p1, const -cxx_class- & p2);
  friend int operator != (const -cxx_class- & p1, const -cxx_class- & p2);
};

inline int operator == (const -cxx_class- & p1, const -cxx_class- & p2)
  { return -cc_class-_equals(&p1, &p2) != 0; }
inline int operator != (const -cxx_class- & p1, const -cxx_class- & p2)
  { return -cc_class-_equals(&p1, &p2) == 0; }

#endif // !-cpp_wrapper-
