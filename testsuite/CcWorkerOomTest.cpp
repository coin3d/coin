#include <cstdio>
#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/worker.h>

static cc_thread * injected_thread_construct(cc_thread_f *, void *)
{
  return NULL;
}

#define cc_thread_construct injected_thread_construct
#include "../src/threads/worker.cpp"
#undef cc_thread_construct

static void idle_job(void *) {}

int main()
{
  cc_worker * worker = cc_worker_construct();
  if (worker == NULL) return 2;
  const SbBool started = cc_worker_start(worker, idle_job, NULL);
  cc_worker_destruct(worker);
  if (started) {
    std::fputs("worker accepted a job without a thread\n", stderr);
    return 1;
  }
  return 0;
}
