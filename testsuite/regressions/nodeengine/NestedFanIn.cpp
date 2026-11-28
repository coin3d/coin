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
static void exercise()
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
  changing->first.touchSlaves(NULL, FALSE);
  CHECK(shared.getValue() == 17);

  changing->shared = &shared;
  changing->change = true;
  changing->evaluateWrapper();
  CHECK(changing->observed == 2);
  CHECK(shared.getValue() == 2);

  shared.disconnect();
  changing->unref();
  other->unref();
}

int main(int argc, char ** argv)
{
  CHECK(argc == 3);
  initProbes();
  const bool node = !std::strcmp(argv[1], "node");
  const bool otherNode = !std::strcmp(argv[2], "node");
  if (node && otherNode) exercise<ProbeNode, ProbeNode>();
  else if (node) exercise<ProbeNode, ProbeEngine>();
  else if (otherNode) exercise<ProbeEngine, ProbeNode>();
  else exercise<ProbeEngine, ProbeEngine>();
}
