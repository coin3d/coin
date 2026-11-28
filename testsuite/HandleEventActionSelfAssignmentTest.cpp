#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoEventManager.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/actions/SoHandleEventAction.h>

#include <cstdio>
#include <cstring>

namespace {

bool
check(bool condition, const char * message)
{
  if (!condition) std::fprintf(stderr, "%s\n", message);
  return condition;
}

class TrackingHandleEventAction : public SoHandleEventAction {
public:
  explicit TrackingHandleEventAction(int & destructions)
    : SoHandleEventAction(SbViewportRegion(73, 41)),
      destructions(destructions) { }

  ~TrackingHandleEventAction() override { ++this->destructions; }

private:
  int & destructions;
};

template <typename Manager>
int
exerciseHandleEventAction(Manager & manager, const char * owner)
{
  SoHandleEventAction * const defaultaction = manager.getHandleEventAction();
  if (!check(defaultaction != NULL,
             "manager has no default handle-event action")) return 1;

  const SbViewportRegion defaultviewport = defaultaction->getViewportRegion();
  manager.setHandleEventAction(defaultaction);
  if (!check(manager.getHandleEventAction() == defaultaction,
             "default handle-event action identity changed on self-assignment")) {
    return 1;
  }
  if (!check(defaultaction->getViewportRegion() == defaultviewport,
             "default handle-event viewport changed on self-assignment")) {
    return 1;
  }
  manager.setHandleEventAction(defaultaction);
  if (!check(manager.getHandleEventAction() == defaultaction,
             "repeated default handle-event self-assignment changed identity")) {
    return 1;
  }

  int destructions = 0;
  TrackingHandleEventAction * custom =
    new TrackingHandleEventAction(destructions);
  manager.setHandleEventAction(custom);
  if (!check(manager.getHandleEventAction() == custom,
             "custom handle-event action was not installed")) return 1;

  const SbViewportRegion customviewport = custom->getViewportRegion();
  manager.setHandleEventAction(custom);
  if (!check(manager.getHandleEventAction() == custom,
             "custom handle-event action identity changed on self-assignment")) {
    return 1;
  }
  if (!check(custom->getViewportRegion() == customviewport,
             "custom handle-event viewport changed on self-assignment")) {
    return 1;
  }
  if (!check(destructions == 0,
             "manager deleted an externally owned handle-event action")) {
    return 1;
  }

  delete custom;
  if (!check(destructions == 1,
             "external handle-event action destruction count is wrong")) {
    return 1;
  }

  std::printf("%s-handle-event default=self-preserved "
              "custom=external-preserved viewport=preserved\n", owner);
  return 0;
}

int
runMode(const char * mode)
{
  if (std::strcmp(mode, "event") == 0) {
    SoEventManager manager;
    return exerciseHandleEventAction(manager, "event-manager");
  }
  if (std::strcmp(mode, "scene") == 0) {
    SoSceneManager manager;
    return exerciseHandleEventAction(manager, "scene-manager");
  }
  std::fprintf(stderr, "unknown mode: %s\n", mode);
  return 2;
}

} // namespace

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s <event|scene>\n", argv[0]);
    return 2;
  }

  SoDB::init();
  const int result = runMode(argv[1]);
  SoDB::finish();
  return result;
}
