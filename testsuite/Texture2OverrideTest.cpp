#include "CoinTest.h"
#include "Texture2OverrideTestSupport.h"

BOOST_AUTO_TEST_SUITE(SoTexture2Override_TestSuite)
BOOST_AUTO_TEST_CASE(callback_image_state_and_protection)
{
  SoCallbackAction action;
  Texture2OverrideTest::run(action, true, true);
}
BOOST_AUTO_TEST_CASE(raypick_image_state_and_protection)
{
  SoRayPickAction action(SbViewportRegion(16,16));
  action.setRay(SbVec3f(0,0,5), SbVec3f(0,0,-1));
  Texture2OverrideTest::run(action, true, true);
}
BOOST_AUTO_TEST_SUITE_END()
