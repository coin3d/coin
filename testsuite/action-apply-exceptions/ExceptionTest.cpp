#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <thread>

namespace {
bool shouldthrow = true;
int visits = 0;

void callback(void *, SoAction *)
{
  ++visits;
  if (shouldthrow) throw std::runtime_error("callback failure");
}

void check(bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "%s\n", message);
    std::abort();
  }
}

template <typename F>
void throws(F invoke)
{
  bool caught = false;
  try { invoke(); }
  catch (const std::runtime_error & error) {
    caught = std::string(error.what()) == "callback failure";
  }
  check(caught, "callback exception did not propagate");
}

void checkUnlocked()
{
  std::atomic<bool> done(false);
  std::thread writer([&done]() {
    SoDB::writelock();
    SoDB::writeunlock();
    done.store(true);
  });
  const auto deadline = std::chrono::steady_clock::now() +
                        std::chrono::seconds(3);
  while (!done.load() && std::chrono::steady_clock::now() < deadline) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  check(done.load(), "SoDB read lock remained held after exception");
  writer.join();
}
}

int main()
{
  SoDB::init();
  {
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoCallback * node = new SoCallback;
  node->setCallback(callback);
  root->addChild(node);
  SoSeparator * second = new SoSeparator;
  second->ref();
  SoCallback * secondnode = new SoCallback;
  secondnode->setCallback(callback);
  second->addChild(secondnode);

  SoPath * path = new SoPath(root);
  path->ref();
  path->append(0);
  SoPath * secondpath = new SoPath(second);
  secondpath->ref();
  secondpath->append(0);
  SoPathList one;
  one.append(path);
  SoPathList two;
  two.append(path);
  two.append(secondpath);

  SoCallbackAction action;
  const int statedepth = action.getState()->getDepth();
  const int rootrefs = root->getRefCount();
  throws([&]() { action.apply(root); });
  check(root->getRefCount() == rootrefs, "root reference leaked");
  check(action.getState()->getDepth() == statedepth, "node state depth changed");
  check(action.getNodeAppliedTo() == NULL, "node target not restored");
  checkUnlocked();

  const int pathrefs = path->getRefCount();
  throws([&]() { action.apply(path); });
  check(path->getRefCount() == pathrefs, "path reference leaked");
  check(action.getState()->getDepth() == statedepth, "path state depth changed");
  check(action.getPathAppliedTo() == NULL, "path target not restored");
  checkUnlocked();

  throws([&]() { action.apply(one, TRUE); });
  check(action.getState()->getDepth() == statedepth, "ordered list state depth changed");
  check(action.getPathListAppliedTo() == NULL, "ordered list target not restored");
  checkUnlocked();
  throws([&]() { action.apply(two, FALSE); });
  check(action.getState()->getDepth() == statedepth, "sorted list state depth changed");
  check(action.getPathListAppliedTo() == NULL, "sorted list target not restored");
  checkUnlocked();

  shouldthrow = false;
  const int before = visits;
  action.apply(root);
  action.apply(path);
  action.apply(one, TRUE);
  action.apply(two, FALSE);
  check(visits >= before + 4, "action was not reusable");

  secondpath->unref();
  path->unref();
  second->unref();
  root->unref();
  }
  SoDB::finish();
  return 0;
}
