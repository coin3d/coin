#include "Fixture.h"
#include <cstring>

template <typename Engine>
class ChangingEngine : public Engine {
public:
  SoSFInt32 * shared = NULL;
  bool change = false;
  int observed = -1;
protected:
  void evaluate() override {
    if (change) {
      change = false;
      this->input.setValue(2);
      // Read before the outer evaluation writes. This exposes which
      // engine the field selected when its input changed.
      observed = shared->getValue();
    }
    SO_ENGINE_OUTPUT(this->first, SoSFInt32, setValue(this->input.getValue()));
  }
};

template <typename Engine, typename Other>
static void exercise(const char * mode)
{
  auto * changing = new ChangingEngine<Engine>;
  changing->ref();
  auto * other = new Other;
  other->ref();

  SoSFInt32 shared;
  shared.setValue(-1);
  CHECK(shared.connectFrom(&changing->first));
  CHECK(shared.appendConnection(&other->first));
  changing->input.setValue(1);
  CHECK(shared.getValue() == 1);

  // Leave the changing engine dirty while selecting the other engine.
  changing->input.touch();
  other->input.touch();
  CHECK(shared.getValue() == 17);

  CHECK(changing->enableNotify(FALSE));
  if (!std::strcmp(mode, "nested")) {
    changing->shared = &shared;
    changing->change = true;
    changing->evaluateWrapper();
    CHECK(changing->observed == 17);
  }
  else {
    const bool disableConnection = !std::strcmp(mode, "disabled");
    if (disableConnection) shared.enableConnection(FALSE);

    changing->input.setValue(2);
    if (disableConnection) {
      CHECK(shared.getValue() == 17);
      shared.enableConnection(TRUE);
    }
    CHECK(shared.getValue() == 17);
  }

  changing->enableNotify(TRUE);
  changing->input.setValue(3);
  CHECK(shared.getValue() == 3);

  shared.disconnect();
  changing->unref();
  other->unref();
}

int main(int argc, char ** argv)
{
  CHECK(argc == 4);
  initProbes();
  const bool node = !std::strcmp(argv[1], "node");
  const bool otherNode = !std::strcmp(argv[2], "node");
  if (node && otherNode) exercise<ProbeNode, ProbeNode>(argv[3]);
  else if (node) exercise<ProbeNode, ProbeEngine>(argv[3]);
  else if (otherNode) exercise<ProbeEngine, ProbeNode>(argv[3]);
  else exercise<ProbeEngine, ProbeEngine>(argv[3]);
}
