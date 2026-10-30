#include <Inventor/SoDB.h>
#include <Inventor/annex/HardCopy/SoVectorizePSAction.h>
#include <Inventor/nodes/SoCube.h>

#include <fcntl.h>
#include <sys/resource.h>

int
main(void)
{
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

  {
    SoVectorizePSAction action;
    action.beginPage(SbVec2f(0.0f, 0.0f), SbVec2f(100.0f, 100.0f));
    SoCube * cube = new SoCube;
    cube->ref();
    action.apply(cube);
    cube->unref();
    action.endPage();
  }

  if (setrlimit(RLIMIT_NOFILE, &original) != 0) return 1;
  SoDB::finish();
  return fcntl(1, F_GETFD) == -1 ? 1 : 0;
}
