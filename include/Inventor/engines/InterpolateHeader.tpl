//$ TEMPLATE InterpolateHeader(className,inputType)
#include <Inventor/engines/SoInterpolateAbs.h>
#include <Inventor/fields/inputType.h>

class className : public SoInterpolate {
  typedef SoInterpolate inherited;
  SO_ENGINE_HEADER(className);

public:
  inputType input0;
  inputType input1;

  className();

  static void initClass();

private:
  virtual ~className();
  virtual void evaluate();

};
