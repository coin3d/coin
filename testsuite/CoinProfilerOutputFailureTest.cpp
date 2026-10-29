#include <Inventor/annex/Profiler/utils/SoProfilingReportGenerator.h>

#include <fcntl.h>
#include <sys/resource.h>

int
main(void)
{
  struct rlimit original;
  if (getrlimit(RLIMIT_NOFILE, &original) != 0 || original.rlim_max < 3) {
    return 77;
  }
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 77;
  }

  struct rlimit exhausted = original;
  exhausted.rlim_cur = 3;
  if (setrlimit(RLIMIT_NOFILE, &exhausted) != 0) return 77;

  if (SoProfilingReportGenerator::stdoutCB(NULL, 0, "test") !=
        SoProfilingReportGenerator::STOP ||
      SoProfilingReportGenerator::stderrCB(NULL, 0, "test") !=
        SoProfilingReportGenerator::STOP) {
    return 1;
  }
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 1;
  }
  return 0;
}
