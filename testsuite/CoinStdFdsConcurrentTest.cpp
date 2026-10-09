#include "tidbitsp.h"

#include <atomic>
#include <cstdio>
#include <fcntl.h>
#include <thread>
#include <vector>

extern "C" void free_std_fds(void);

int
main(void)
{
  if (fcntl(2, F_GETFD) == -1) return 77;

  for (int round = 0; round < 8; ++round) {
    const int threadcount = 8;
    std::atomic<int> ready(0);
    std::atomic<bool> start(false);
    std::vector<FILE *> streams(threadcount, NULL);
    std::vector<std::thread> threads;

    for (int i = 0; i < threadcount; ++i) {
      threads.push_back(std::thread([&ready, &start, &streams, i]() {
        ++ready;
        while (!start.load()) std::this_thread::yield();
        streams[i] = coin_get_stderr();
      }));
    }
    while (ready.load() != threadcount) std::this_thread::yield();
    start = true;
    for (std::size_t i = 0; i < threads.size(); ++i) threads[i].join();

    if (streams[0] == NULL) return 1;
    for (int i = 1; i < threadcount; ++i) {
      if (streams[i] != streams[0]) return 1;
    }
    free_std_fds();
    if (fcntl(2, F_GETFD) == -1) return 1;
  }
  return 0;
}
