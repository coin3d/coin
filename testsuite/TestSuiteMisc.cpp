
#include <Inventor/SbVec4ub.h>

#include <TestSuiteMisc.h>

std::ostream &
operator << (std::ostream & os, const SbVec4ub & vec)
{
  os << (int) vec[0];
  os << " ";
  os << (int) vec[1];
  os << " ";
  os << (int) vec[2];
  os << " ";
  os << (int) vec[3];
  return os;
}

