#include <cstdio>
#include <cstdlib>
#include <Inventor/C/threads/wpool.h>
#include <Inventor/C/threads/worker.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/base/list.h>
#include "threads/wpoolp.h"
#include "base/listp.h"

static int failed_stage = -1;
static int current_stage = 0;

static bool should_fail()
{
  return current_stage++ == failed_stage;
}

static void * injected_malloc(size_t size)
{
  return should_fail() ? NULL : std::malloc(size);
}

static cc_mutex * injected_mutex_construct()
{
  return should_fail() ? NULL : cc_mutex_construct();
}

static cc_condvar * injected_condvar_construct()
{
  return should_fail() ? NULL : cc_condvar_construct();
}

static cc_list * injected_list_construct()
{
  return should_fail() ? NULL : cc_list_construct();
}

static cc_worker * injected_worker_construct()
{
  return should_fail() ? NULL : cc_worker_construct();
}

#define malloc(size) injected_malloc(size)
#define cc_mutex_construct injected_mutex_construct
#define cc_condvar_construct injected_condvar_construct
#define cc_list_construct injected_list_construct
#define cc_worker_construct injected_worker_construct
#include "../src/threads/wpool.cpp"
#undef malloc
#undef cc_mutex_construct
#undef cc_condvar_construct
#undef cc_list_construct
#undef cc_worker_construct

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return 1; \
} } while (0)

int main()
{
  for (int stage = 0; stage < 8; ++stage) {
    failed_stage = stage;
    current_stage = 0;
    CHECK(cc_wpool_construct(3) == NULL);
  }

  failed_stage = -1;
  current_stage = 0;
  cc_wpool * pool = cc_wpool_construct(1);
  CHECK(pool != NULL);
  CHECK(cc_wpool_get_num_workers(pool) == 1);
  cc_wpool_set_num_workers(pool, 3);
  CHECK(cc_wpool_get_num_workers(pool) == 3);
  cc_wpool_set_num_workers(pool, 1);
  CHECK(cc_wpool_get_num_workers(pool) == 1);
  cc_wpool_destruct(pool);
  return 0;
}
