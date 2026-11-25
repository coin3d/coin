#include <Inventor/SoDB.h>
#include <Inventor/annex/HardCopy/SoVectorOutput.h>

#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

static int
countOpenDescriptors(void)
{
  int count = 0;
  for (int fd = 0; fd < 256; ++fd) {
    if (fcntl(fd, F_GETFD) != -1) ++count;
  }
  return count;
}

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

  // Destruction after SoDB::finish() must only close the opened file, and
  // not reopen the standard stream which was just released.
  SoVectorOutput * late = new SoVectorOutput;
  if (!late->openFile(filename)) result = 1;
  SoDB::finish();
  const int before = countOpenDescriptors();
  delete late;
  if (countOpenDescriptors() != before - 1) result = 1;

  unlink(filename);
  return result;
}
