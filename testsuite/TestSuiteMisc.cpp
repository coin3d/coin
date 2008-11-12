
#include <Inventor/SbVec4ub.h>
#include <Inventor/SbVec4us.h>
#include <Inventor/SbVec4ui32.h>
#include <Inventor/SbColor4f.h>

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

std::ostream &
operator << (std::ostream & os, const SbVec4us & vec)
{
  os << vec[0];
  os << " ";
  os << vec[1];
  os << " ";
  os << vec[2];
  os << " ";
  os << vec[3];
  return os;
}

std::ostream &
operator << (std::ostream & os, const SbVec4ui32 & vec)
{
  os << vec[0];
  os << " ";
  os << vec[1];
  os << " ";
  os << vec[2];
  os << " ";
  os << vec[3];
  return os;
}

std::ostream &
operator << (std::ostream & os, const SbColor4f & col4)
{
  os << col4[0];
  os << " ";
  os << col4[1];
  os << " ";
  os << col4[2];
  os << " ";
  os << col4[3];
  return os;
}
