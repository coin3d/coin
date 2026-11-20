#ifndef COIN_FIELDDB_REGRESSION_SUPPORT_H
#define COIN_FIELDDB_REGRESSION_SUPPORT_H
#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <utility>

#define REQUIRE(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); \
  std::_Exit(1); } } while (0)
namespace fielddb {
struct Failure {};
static Failure failure;
template <class Operation> void expectFailure(Operation operation)
{
  bool caught = false;
  try { operation(); }
  catch (Failure * error) { REQUIRE(error == &failure); caught = true; }
  catch (...) { REQUIRE(false && "exception identity/type changed"); }
  REQUIRE(caught);
}
// A stuck worker must never reach std::thread's destructor or a blocking join.
// This also leaves failed Coin global state untouched during process teardown.
template <class Operation> void onOtherThread(Operation operation)
{
  std::atomic<bool> done(false);
  std::thread worker([&]() {
    operation();
    done.store(true, std::memory_order_release);
  });
  const std::chrono::steady_clock::time_point deadline =
    std::chrono::steady_clock::now() + std::chrono::seconds(3);
  while (!done.load(std::memory_order_acquire)) {
    REQUIRE(std::chrono::steady_clock::now() < deadline);
    std::this_thread::yield();
  }
  worker.join();
}
inline void init()
{
  SoDB::init();
  SoDB::enableRealTimeSensor(FALSE);
  SoDB::setDelaySensorTimeout(SbTime::zero());
}
}
#endif
