#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/annex/HardCopy/SoVectorOutput.h>
#include <Inventor/nodes/SoSeparator.h>

#include <fcntl.h>
#include <cstring>
#include <sys/resource.h>

int
main(void)
{
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 77;
  }

  SoDB::init();

  struct rlimit original;
  if (getrlimit(RLIMIT_NOFILE, &original) != 0 || original.rlim_max < 3) {
    SoDB::finish();
    return 77;
  }
  struct rlimit exhausted = original;
  exhausted.rlim_cur = 3;
  if (setrlimit(RLIMIT_NOFILE, &exhausted) != 0) {
    SoDB::finish();
    return 77;
  }

  int result = 0;
  {
    SoInput input;
    char value = 0;
    if (input.read(value)) result = 1;

    // The buffer has no FILE, and must not be mistaken for the unavailable
    // stdin and discarded when pushing another file.
    static const char scene[] = "#Inventor V2.1 ascii\nCube {}\n";
    SoInput bufferinput;
    bufferinput.setBuffer(scene, strlen(scene));
    if (bufferinput.pushFile("nonexistent-coin-test-file.iv")) result = 1;
    if (bufferinput.getCurFileName() != NULL) {
      result = 1; // the buffer is gone
    }
    else {
      SoSeparator * root = SoDB::readAll(&bufferinput);
      if (root == NULL) {
        result = 1;
      }
      else {
        root->ref();
        if (root->getNumChildren() != 1) result = 1;
        root->unref();
      }
    }

    SoOutput output;
    output.write('x');

    SoOutput unwritten;
    SoOutput nullpointer;
    nullpointer.setFilePointer(NULL);
    SoVectorOutput vectoroutput;
    if (output.getFilePointer() != NULL || unwritten.getFilePointer() != NULL ||
        vectoroutput.getFilePointer() != NULL) {
      result = 1;
    }

    if (setrlimit(RLIMIT_NOFILE, &original) != 0) result = 1;

    // The default stream is picked up when it becomes available, except
    // in the middle of a stream where writing has already failed.
    if (unwritten.getFilePointer() == NULL ||
        vectoroutput.getFilePointer() != unwritten.getFilePointer()) {
      result = 1;
    }
    if (output.getFilePointer() != NULL) result = 1;
    output.reset();
    if (output.getFilePointer() != unwritten.getFilePointer()) result = 1;

    // An explicit NULL file pointer must not turn into stdout.
    if (nullpointer.getFilePointer() != NULL) result = 1;
  }
  SoDB::finish();
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) result = 1;
  }
  return result;
}
