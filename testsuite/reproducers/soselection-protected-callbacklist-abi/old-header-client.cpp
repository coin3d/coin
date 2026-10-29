#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/lists/SoCallbackList.h>
#include <cstdio>

static int typedcalls, genericcalls;
static void typed(void *, SoSelection *) { ++typedcalls; }
static void generic(void *, void *) { ++genericcalls; }
class ClientSelection : public SoSelection {
public:
  unsigned long marker;
  ClientSelection() : marker(0x12345678UL) {}
  SoCallbackList & callbacks() { return *changeCBList; }
};
int main()
{
  SoInteraction::init();
  bool valid;
  {
    ClientSelection * selection = new ClientSelection;
    selection->ref();
    valid = selection->marker == 0x12345678UL;
    selection->addChangeCallback(typed);
    selection->callbacks().addCallback(generic);
    SoCallbackList snapshot(selection->callbacks());
    SoCallbackList assigned;
    assigned = snapshot;
    selection->callbacks().clearCallbacks();
    selection->unref();
    snapshot.invokeCallbacks(NULL);
    snapshot.clearCallbacks();
    assigned.invokeCallbacks(NULL);
    valid = valid && typedcalls == 2 && genericcalls == 2;
  }
  SoDB::finish();
  std::printf("old-header layout, mixed dispatch, copy and assignment: %s\n",
              valid ? "PASS" : "FAIL");
  return valid ? 0 : 1; // Assertions disabled by NDEBUG must not erase checks.
}
