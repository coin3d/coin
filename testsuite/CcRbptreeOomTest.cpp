#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

static bool fail_node_allocation = false;

static void *
injected_malloc(size_t size)
{
  if (fail_node_allocation) return NULL;
  return std::malloc(size);
}

#define malloc(size) injected_malloc(size)
#include "../src/base/rbptree.cpp"
#undef malloc

int
main()
{
  struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);

  const pid_t child = fork();
  if (child == 0) {
    int values[3] = { 1, 2, 3 };
    cc_rbptree tree;
    cc_rbptree_init(&tree);
    cc_rbptree_insert(&tree, &values[0], NULL);
    cc_rbptree_insert(&tree, &values[1], NULL);
    fail_node_allocation = true;
    cc_rbptree_insert(&tree, &values[2], NULL);
    _exit(1);
  }
  if (child < 0) return 2;

  int status = 0;
  if (waitpid(child, &status, 0) != child) return 3;
  if (!WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {
    std::fprintf(stderr, "expected SIGABRT when the third node allocation fails\n");
    return 4;
  }
  return 0;
}
