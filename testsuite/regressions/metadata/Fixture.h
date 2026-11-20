#ifndef COIN_METADATA_REGRESSION_FIXTURE_H
#define COIN_METADATA_REGRESSION_FIXTURE_H

#include <Inventor/SoDB.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSubNode.h>

#include <cstdio>
#include <cstdlib>

#define CHECK(condition) do { \
  if (!(condition)) { \
    std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); \
    std::abort(); \
  } \
} while (0)

class ProbeNode : public SoNodeEngine {
  SO_NODE_HEADER(ProbeNode);

public:
  SoEngineOutput first;
  SoEngineOutput second;

  ProbeNode()
  {
    SO_NODE_CONSTRUCTOR(ProbeNode);
    outputs.addOutput(this, "first", &first, SoSFInt32::getClassTypeId());
    outputs.addOutput(this, "second", &second, SoSFInt32::getClassTypeId());
    first.setNodeContainer(this);
    second.setNodeContainer(this);
  }

  static void initClass()
  {
    SO_NODE_INIT_CLASS(ProbeNode, SoNodeEngine, "NodeEngine");
  }

  const SoEngineOutputData * getOutputData() const override { return &outputs; }

protected:
  ~ProbeNode() override = default;
  void evaluate() override {}

private:
  SoEngineOutputData outputs;
};

SO_NODE_SOURCE(ProbeNode);

class ProbeEngine : public SoEngine {
  SO_ENGINE_HEADER(ProbeEngine);

public:
  SoSFInt32 input;
  SoEngineOutput first;
  SoEngineOutput second;

  ProbeEngine()
  {
    SO_ENGINE_CONSTRUCTOR(ProbeEngine);
    SO_ENGINE_ADD_INPUT(input, (0));
    SO_ENGINE_ADD_OUTPUT(first, SoSFInt32);
    SO_ENGINE_ADD_OUTPUT(second, SoSFInt32);
  }

  static void initClass()
  {
    SO_ENGINE_INIT_CLASS(ProbeEngine, SoEngine, "Engine");
  }

protected:
  ~ProbeEngine() override = default;
  void evaluate() override {}
};

SO_ENGINE_SOURCE(ProbeEngine);

static void initProbes()
{
  SoDB::init();
  ProbeNode::initClass();
  ProbeEngine::initClass();
}

#endif
