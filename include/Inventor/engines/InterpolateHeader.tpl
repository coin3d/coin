//$ TEMPLATE InterpolateHeader(__classname__,__inputtype__)
#include <Inventor/engines/SoInterpolateAbs.h>
#include <Inventor/fields/__inputtype__.h>

class __classname__ : public SoInterpolate {
  SO_INTERPOLATE_HEADER(__classname__);

public:
  __inputtype__ input0;
  __inputtype__ input1;
};
