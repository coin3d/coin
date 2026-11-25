#ifndef COIN_TEST_DEPTH_COUNTING_ENGINE_H
#define COIN_TEST_DEPTH_COUNTING_ENGINE_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/errors/SoDebugError.h>
#include <cstdio>

// Use a separate instance per field: reading one output must not clean another
// field's engine and conceal an unnecessary evaluation.
class DepthCountingEngine : public SoEngine {
  // The default C error handler takes the same global lock as addOutput.
  // Forward the enum-output advisory directly while registering metadata.
  struct OutputDiagnostic {
    SoErrorCB * previous;
    void * previousData;
    static void report(const SoError * error, void *) {
      std::fprintf(stderr, "%s\n", error->getDebugString().getString());
    }
    OutputDiagnostic() : previous(SoDebugError::getHandlerCallback()),
                         previousData(SoDebugError::getHandlerData()) {
      SoDebugError::setHandlerCallback(report, NULL);
    }
    ~OutputDiagnostic() { SoDebugError::setHandlerCallback(previous, previousData); }
  };
  static SoType classTypeId;
  SoFieldData inputData;
  SoEngineOutputData outputData;
public:
  SoSFInt32 revision;
  SoEngineOutput boolean, function, range;
  int evaluations;
  SbBool booleanValue;
  int functionValue;
  SbVec2f rangeValue;
  static void initClass() {
    classTypeId = SoType::createType(SoEngine::getClassTypeId(),
                                    "DepthCountingEngine");
  }
  DepthCountingEngine() : evaluations(0), booleanValue(FALSE),
    functionValue(0), rangeValue(0, 1) {
    // No static field/output metadata is shared between these engines.
    this->isBuiltIn = FALSE;
    revision.setValue(0);
    revision.setContainer(this);
    inputData.addField(this, "revision", &revision);
    outputData.addOutput(this, "boolean", &boolean, SoSFBool::getClassTypeId());
    {
      OutputDiagnostic diagnostic;
      outputData.addOutput(this, "function", &function, SoSFEnum::getClassTypeId());
    }
    outputData.addOutput(this, "range", &range, SoSFVec2f::getClassTypeId());
    boolean.setContainer(this);
    function.setContainer(this);
    range.setContainer(this);
  }
  SoType getTypeId() const override { return classTypeId; }
  const SoFieldData * getFieldData() const override { return &inputData; }
  const SoEngineOutputData * getOutputData() const override { return &outputData; }
  void dirty() { revision = revision.getValue() + 1; }
protected:
  ~DepthCountingEngine() override { }
  void evaluate() override {
    ++evaluations;
    SO_ENGINE_OUTPUT(boolean, SoSFBool, setValue(booleanValue));
    SO_ENGINE_OUTPUT(function, SoSFEnum, setValue(functionValue));
    SO_ENGINE_OUTPUT(range, SoSFVec2f, setValue(rangeValue));
  }
};
SoType DepthCountingEngine::classTypeId;

#endif
