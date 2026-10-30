#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#include <fcntl.h>
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
    if (input.read(value) || !input.hasReadError()) result = 1;

    SoOutput output;
    output.write('x');
  }

  if (setrlimit(RLIMIT_NOFILE, &original) != 0) result = 1;
  SoDB::finish();
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) result = 1;
  }
  return result;
}
