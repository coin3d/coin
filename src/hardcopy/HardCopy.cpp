// Copyright header here.

#include <HardCopy/SoHardCopy.h>
#include <HardCopy/SoVectorizePSAction.h>

// *************************************************************************

void
SoHardCopy::init(void)
{
  SoVectorizeAction::initClass();
  SoVectorizePSAction::initClass();
}

const char *
SoHardCopy::getProductName(void)
{
  return "Systems in Motion hard-copy support for Coin.";
}

const char *
SoHardCopy::getVersion(void)
{
  return "1.0alpha";
}

// *************************************************************************
