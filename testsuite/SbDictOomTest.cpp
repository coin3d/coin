#define COIN_ALLOW_SBDICT
#include <Inventor/SbDict.h>
#include "base/dict.h"
#include <cstdio>
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
static bool failconstruct, failput;
static cc_dict * injected_construct(unsigned int size, float factor) {
  if (failconstruct) { failconstruct = false; return NULL; }
  return cc_dict_construct(size, factor);
}
static cc_dict_put_result injected_put(cc_dict * table, uintptr_t key, void * value) {
  if (failput) { failput = false; return CC_DICT_PUT_FAILED; }
  return cc_dict_try_put(table, key, value);
}
#define cc_dict_construct(size, factor) injected_construct(size, factor)
#define cc_dict_try_put(table, key, value) injected_put(table, key, value)
#include "../src/base/SbDict.cpp"
#undef cc_dict_construct
#undef cc_dict_try_put
int main() {
  const rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);
  for (int stage = 0; stage < 4; ++stage) {
    const pid_t child = fork();
    if (child == 0) {
      if (stage == 0) failconstruct = true;
      SbDict source(17), target(17);
      if (stage == 1) { failput = true; source.enter(1, NULL); }
      source.enter(1, NULL);
      if (stage == 2) failconstruct = true;
      if (stage == 3) failput = true;
      target = source;
      _exit(1);
    }
    if (child < 0) return 2;
    int status;
    if (waitpid(child, &status, 0) != child || !WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {
      std::fprintf(stderr, "expected mandatory SbDict failure at stage %d\n", stage);
      return 1;
    }
  }
  return 0;
}
