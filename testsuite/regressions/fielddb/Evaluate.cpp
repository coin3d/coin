#include "Support.h"
#include "../nodeengine/Fixture.h"
#include <Inventor/fields/SoSFFloat.h>
#include <cstring>

namespace {
class EvaluateField : public SoSFInt32 {
public:
  mutable int calls;
  bool armed, writeBeforeThrow, recurse;
  EvaluateField() : calls(0), armed(false), writeBeforeThrow(false), recurse(false) {}
protected:
  void evaluateConnection() const override {
    ++calls;
    if (recurse) (void) this->getValue();
    if (armed) {
      if (writeBeforeThrow) const_cast<EvaluateField *>(this)->setValue(91);
      throw &fielddb::failure;
    }
    SoSFInt32::evaluateConnection();
  }
};
class CopyField : public SoSFInt32 {
public:
  bool armed; int copies;
  CopyField() : armed(false), copies(0) {}
  void copyFrom(const SoField & other) override {
    ++copies;
    // Deliberately clear dirty through the ordinary setter before throwing.
    SoSFInt32::copyFrom(other);
    if (armed) throw &fielddb::failure;
  }
};
void otherFieldCanEvaluate()
{
  // Both fields are unrelated to the failed field, so an evaluation barrier on
  // that field cannot mask a leaked recursive global field mutex.
  SoSFInt32 source, destination;
  source.setValue(37);
  REQUIRE(destination.connectFrom(&source, TRUE));
  destination.setDirty(TRUE);
  fielddb::onOtherThread([&]() { REQUIRE(destination.getValue() == 37); });
  destination.disconnect();
}
void overrideFailure(const char * mode)
{
  SoSFInt32 source;
  source.setValue(42);
  EvaluateField field;
  REQUIRE(field.connectFrom(&source, TRUE));
  field.armed = true;
  field.writeBeforeThrow = std::strcmp(mode, "override-write") == 0;
  field.recurse = std::strcmp(mode, "recursive-throw") == 0;
  field.setDirty(TRUE);
  fielddb::expectFailure([&]() { (void) field.getValue(); });
  REQUIRE(field.getDirty());
  REQUIRE(!field.isReadOnly());
  otherFieldCanEvaluate();
  field.armed = false;
  REQUIRE(field.getValue() == 42);
  REQUIRE(field.calls == 2);
  REQUIRE(!field.getDirty());
  REQUIRE(field.getValue() == 42 && field.calls == 2);
  otherFieldCanEvaluate();
  field.disconnect();
}
void copyFailure(bool notify)
{
  SoSFInt32 source;
  source.setValue(63);
  CopyField field;
  REQUIRE(field.connectFrom(&source, TRUE));
  field.enableNotify(notify ? TRUE : FALSE);
  field.armed = true;
  field.setDirty(TRUE);
  fielddb::expectFailure([&]() { (void) field.getValue(); });
  REQUIRE(field.isNotifyEnabled() == (notify ? TRUE : FALSE));
  REQUIRE(field.getDirty());
  otherFieldCanEvaluate();
  field.armed = false;
  REQUIRE(field.getValue() == 63);
  REQUIRE(field.copies == 2);
  REQUIRE(!field.getDirty());
  REQUIRE(field.isNotifyEnabled() == (notify ? TRUE : FALSE));
  field.disconnect();
}
template<class Engine> void engineFailure()
{
  Engine * engine = new Engine;
  engine->ref();
  SoSFInt32 field;
  REQUIRE(field.connectFrom(&engine->first, TRUE));
  directNotify(engine);
  field.setDirty(TRUE);
  engine->state.throwEvaluate = true;
  bool caught = false;
  try { (void) field.getValue(); }
  catch (const std::runtime_error &) { caught = true; }
  REQUIRE(caught && field.getDirty());
  REQUIRE(field.isNotifyEnabled() && !field.isReadOnly());
  otherFieldCanEvaluate();
  engine->state.throwEvaluate = false;
  REQUIRE(field.getValue() == 17);
  REQUIRE(engine->state.evaluations == 2);
  REQUIRE(!field.getDirty());
  field.disconnect();
  engine->unref();
}
void converterFailure()
{
  SoSFInt32 master;
  master.setValue(29);
  EvaluateField source;
  REQUIRE(source.connectFrom(&master, TRUE));
  SoSFFloat converted;
  REQUIRE(converted.connectFrom(&source, TRUE));
  source.armed = true;
  source.setDirty(TRUE);
  converted.setDirty(TRUE);
  fielddb::expectFailure([&]() { (void) converted.getValue(); });
  REQUIRE(source.getDirty() && converted.getDirty());
  otherFieldCanEvaluate();
  source.armed = false;
  REQUIRE(converted.getValue() == 29.0f);
  REQUIRE(!source.getDirty() && !converted.getDirty());
  converted.disconnect(); source.disconnect();
}
void controls(const char * mode)
{
  SoSFInt32 source;
  source.setValue(45);
  EvaluateField field;
  REQUIRE(field.connectFrom(&source, TRUE));
  if (!std::strcmp(mode, "disabled")) {
    field.enableConnection(FALSE);
    field.armed = true;
    field.setDirty(TRUE);
    (void) field.getValue();
    REQUIRE(field.calls == 0);
    otherFieldCanEvaluate();
    field.armed = false;
    field.enableConnection(TRUE);
  }
  if (!std::strcmp(mode, "recursive-success")) field.recurse = true;
  field.setDirty(TRUE);
  REQUIRE(field.getValue() == 45);
  REQUIRE(!field.getDirty());
  const int calls = field.calls;
  REQUIRE(field.getValue() == 45 && field.calls == calls);
  otherFieldCanEvaluate();
  field.disconnect();
  if (!std::strcmp(mode, "cycle")) {
    SoSFInt32 a, b;
    a.setValue(7); b.setValue(7);
    REQUIRE(a.connectFrom(&b, TRUE));
    REQUIRE(b.connectFrom(&a, TRUE));
    a.setDirty(TRUE); b.setDirty(TRUE);
    REQUIRE(a.getValue() == 7 && b.getValue() == 7);
    otherFieldCanEvaluate();
    a.disconnect(); b.disconnect();
  }
}
}
int main(int argc, char ** argv)
{
  REQUIRE(argc == 2);
  fielddb::init(); ProbeNode::initClass(); ProbeEngine::initClass();
  const char * mode = argv[1];
  if (!std::strcmp(mode, "copy-notify")) copyFailure(true);
  else if (!std::strcmp(mode, "copy-silent")) copyFailure(false);
  else if (!std::strcmp(mode, "engine")) engineFailure<ProbeEngine>();
  else if (!std::strcmp(mode, "nodeengine")) engineFailure<ProbeNode>();
  else if (!std::strcmp(mode, "converter")) converterFailure();
  else if (!std::strcmp(mode, "override") || !std::strcmp(mode, "override-write") ||
           !std::strcmp(mode, "recursive-throw")) overrideFailure(mode);
  else controls(mode);
  return 0;
}
