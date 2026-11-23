#include <Inventor/SoDB.h>

#include <cstdio>

int
main()
{
  SoDB::init();
  std::puts(SoDB::getVersion());
  SoDB::finish();
  return 0;
}
