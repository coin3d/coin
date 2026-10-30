#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>

#include <cstdio>
#include <unistd.h>

int
main(void)
{
  SoDB::init();
  int result = 0;

  FILE * empty = tmpfile();
  if (!empty) return 77;
  {
    SoInput input;
    input.setFilePointer(empty);
    char c = 0;
    if (input.read(c) || input.hasReadError()) result = 1;
  }
  fclose(empty);

  int fds[2];
  if (pipe(fds) != 0) return 77;
  FILE * broken = fdopen(fds[0], "r");
  if (!broken) return 77;
  close(fds[1]);
  {
    SoInput input;
    input.setFilePointer(broken);
    close(fds[0]);
    char c = 0;
    if (input.read(c) || !input.hasReadError()) result = 2;
    input.closeFile();
    if (input.hasReadError()) result = 3;
  }
  fclose(broken);

  SoDB::finish();
  return result;
}
