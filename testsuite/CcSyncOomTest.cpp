#include <cstdio>
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/sync.h>
#include "base/dict.h"

static int fail_stage = -1;

static cc_dict * injected_dict_construct(unsigned int size, float factor)
{
  return fail_stage == 0 ? NULL : cc_dict_construct(size, factor);
}

static cc_mutex * injected_mutex_construct()
{
  return fail_stage == 1 ? NULL : cc_mutex_construct();
}

static cc_dict_put_result injected_dict_put(cc_dict * dict, uintptr_t key,
                                            void * value)
{
  return fail_stage == 2 ? CC_DICT_PUT_FAILED :
    cc_dict_try_put(dict, key, value);
}

#define cc_dict_construct injected_dict_construct
#define cc_mutex_construct injected_mutex_construct
#define cc_dict_try_put injected_dict_put
#include "../src/threads/sync.cpp"
#undef cc_dict_construct
#undef cc_mutex_construct
#undef cc_dict_try_put

int main()
{
  struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);

  for (int stage = 0; stage < 3; ++stage) {
    const pid_t child = fork();
    if (child == 0) {
      fail_stage = stage;
      (void) cc_sync_begin(reinterpret_cast<void *>(1));
      _exit(1);
    }
    if (child < 0) return 2;
    int status = 0;
    if (waitpid(child, &status, 0) != child ||
        !WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {
      std::fprintf(stderr, "OOM stage %d did not abort safely\n", stage);
      return 1;
    }
  }

  fail_stage = -1;
  void * key = cc_sync_begin(reinterpret_cast<void *>(1));
  if (key == NULL) return 1;
  cc_sync_end(key);
  cc_sync_free(reinterpret_cast<void *>(1));
  return 0;
}
