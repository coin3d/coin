//$ TEMPLATE InterpolateSource(className,outType,baseType,default0,default1,evaluation)

#include <Inventor/engines/className.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>

SO_ENGINE_SOURCE(className);

className::className()
{
  SO_ENGINE_CONSTRUCTOR(className);

  SO_ENGINE_ADD_INPUT(input0,default0);
  SO_ENGINE_ADD_INPUT(input1,default1);

  SO_ENGINE_ADD_OUTPUT(output,outType);
}

void
className::initClass()
{
  SO_ENGINE_INIT_CLASS(className,SoInterpolate,"Interpolate");
}

//
// private members
//
className::~className()
{
}

void
className::evaluate()
{
  int num0=input0.getNum();
  int num1=input1.getNum();

  int numOut=num0>num1?num0:num1;

  SO_ENGINE_OUTPUT(output,outType,setNum(numOut));

  baseType val0,val1,outVal;
  for (int i=0;i<numOut;i++) {
    val0=i<num0?input0[i]:input0[num0-1];
    val1=i<num1?input1[i]:input1[num1-1];

    outVal=evaluation;
    SO_ENGINE_OUTPUT(output,outType,set1Value(i,outVal));
  }
}
