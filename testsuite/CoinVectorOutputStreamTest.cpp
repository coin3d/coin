#include <Inventor/SoDB.h>
#include <Inventor/annex/HardCopy/SoVectorOutput.h>

#include <cstdlib>
#include <unistd.h>

int
main(void)
{
  char filename[] = "/tmp/coin-vector-output-XXXXXX";
  const int descriptor = mkstemp(filename);
  if (descriptor == -1) return 77;
  close(descriptor);

  SoDB::init();
  int result = 0;
  {
    SoVectorOutput output;
    FILE * defaultstream = output.getFilePointer();
    if (defaultstream == NULL || !output.openFile(filename)) {
      result = 1;
    }
    else {
      output.closeFile();
      if (output.getFilePointer() != defaultstream) result = 1;
    }
  }
  unlink(filename);
  SoDB::finish();
  return result;
}
