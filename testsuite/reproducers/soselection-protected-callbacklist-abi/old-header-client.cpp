#include <Inventor/SoInteraction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/lists/SoCallbackList.h>
#include <cassert>
#include <cstdio>

static int calls;
static void callback(void *, SoSelection *) { ++calls; }
class ClientSelection : public SoSelection {
public:
  unsigned long marker;
  ClientSelection() : marker(0x12345678UL) {}
  const SoCallbackList & callbacks() const { return *changeCBList; }
};
int main()
{
  SoInteraction::init();
  ClientSelection * sel = new ClientSelection;
  sel->ref();
  assert(sel->marker == 0x12345678UL);
  sel->addChangeCallback(callback);
  SoCallbackList snapshot(sel->callbacks());
  sel->removeChangeCallback(callback);
  sel->unref();
  snapshot.invokeCallbacks(NULL);
  assert(calls == 1);
  puts("PASS: old-header subclass layout and callback-list copy");
}
