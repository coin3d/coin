#include "../nodeengine/Fixture.h"
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/misc/SoProtoInstance.h>
#include <cstring>

template<class T> int duplicate(bool absent) {
  T * e = new T; e->ref();
  ProbeEngine * owner = new ProbeEngine; owner->ref();
  SoSFInt32 first, second, missing;
  first.setContainer(owner); second.setContainer(owner); missing.setContainer(owner);
  e->first.addConnection(&first); e->first.addConnection(&second);
  CHECK(e->getRefCount() == 3);
  e->evaluateWrapper(); const int evaluations = e->state.evaluations;
  try {
    if (absent) e->first.removeConnection(&missing);
    else e->first.addConnection(&first);
  } catch (...) { CHECK(false); }
  CHECK(e->first.getNumConnections() == 2);
  CHECK(e->first[0] == &first); CHECK(e->first[1] == &second);
  CHECK(e->getRefCount() == 3);
  // Node engines always evaluate; ordinary engines must remain clean.
  if (!e->first.isNodeEngineOutput()) {
    e->evaluateWrapper(); CHECK(e->state.evaluations == evaluations);
  }
  e->first.removeConnection(&first); CHECK(e->getRefCount() == 2);
  CHECK(e->first.getNumConnections() == 1); CHECK(e->first[0] == &second);
  e->first.removeConnection(&second); CHECK(e->getRefCount() == 1);
  // Also exercise missing removal from an empty list.
  e->first.removeConnection(&missing); CHECK(e->getRefCount() == 1);
  owner->unref(); e->unref(); return 0;
}
template<class T> int valid() {
  T * e = new T; e->ref();
  ProbeEngine * owner = new ProbeEngine; owner->ref();
  SoFieldData empty;
  SoProtoInstance * proto = new SoProtoInstance(NULL, &empty); proto->ref();
  SoSFInt32 bare, external, protofield;
  external.setContainer(owner); protofield.setContainer(proto);
  e->first.addConnection(&bare); CHECK(e->getRefCount() == 1);
  e->first.addConnection(&e->input); CHECK(e->getRefCount() == 1);
  e->first.addConnection(&protofield); CHECK(e->getRefCount() == 1);
  e->first.addConnection(&external); CHECK(e->getRefCount() == 2);
  CHECK(e->first.getNumConnections() == 4);
  CHECK(e->first[0] == &bare); CHECK(e->first[1] == &e->input);
  CHECK(e->first[2] == &protofield); CHECK(e->first[3] == &external);
  e->first.removeConnection(&e->input); CHECK(e->getRefCount() == 2);
  CHECK(e->first[0] == &bare); CHECK(e->first[1] == &protofield); CHECK(e->first[2] == &external);
  e->first.removeConnection(&protofield); CHECK(e->getRefCount() == 2);
  e->first.removeConnection(&bare); CHECK(e->getRefCount() == 2);
  e->first.removeConnection(&external); CHECK(e->getRefCount() == 1);
  CHECK(e->first.getNumConnections() == 0);
  // A valid insertion must dirty an ordinary engine after evaluation.
  e->evaluateWrapper(); const int before = e->state.evaluations;
  e->first.addConnection(&bare); e->evaluateWrapper();
  CHECK(e->state.evaluations == before + 1); CHECK(bare.getValue() == 17);
  e->first.removeConnection(&bare);
  proto->unref(); owner->unref(); e->unref(); return 0;
}
template<class T> int workflow() {
  T * e = new T; e->ref();
  ProbeEngine * owner = new ProbeEngine; owner->ref();
  {
    SoSFInt32 slave; slave.setContainer(owner);
    CHECK(slave.connectFrom(&e->first)); CHECK(e->getRefCount() == 2);
    CHECK(slave.getValue() == 17);
    CHECK(slave.appendConnection(&e->second));
    CHECK(e->getRefCount() == 3);
    CHECK(e->first.getNumConnections() == 1); CHECK(e->second.getNumConnections() == 1);
    CHECK(e->first[0] == &slave); CHECK(e->second[0] == &slave);
    slave.disconnect(&e->first); CHECK(e->first.getNumConnections() == 0);
    CHECK(e->second.getNumConnections() == 1); CHECK(e->getRefCount() == 2);
    SoEngineOutput * remaining = NULL;
    CHECK(slave.getConnectedEngine(remaining)); CHECK(remaining == &e->second);
    // disconnect preserves the cached value; a master notification requests evaluation.
    e->input.touch();
    CHECK(slave.getValue() == 23);
    slave.disconnect(); CHECK(e->getRefCount() == 1);
    CHECK(slave.connectFrom(&e->first)); CHECK(e->getRefCount() == 2);
  }
  CHECK(e->first.getNumConnections() == 0); CHECK(e->getRefCount() == 1);
  {
    SoSFFloat converted;
    CHECK(converted.connectFrom(&e->first)); CHECK(converted.getValue() == 17.0f);
    CHECK(e->first.getNumConnections() == 1);
    converted.disconnect(); CHECK(e->first.getNumConnections() == 0);
    CHECK(converted.connectFrom(&e->first)); CHECK(converted.getValue() == 17.0f);
  }
  CHECK(e->first.getNumConnections() == 0); CHECK(e->getRefCount() == 1);
  owner->unref(); e->unref(); return 0;
}
template<class T> int dispatch(const char * mode) {
  if (!std::strcmp(mode, "duplicate")) return duplicate<T>(false);
  if (!std::strcmp(mode, "absent")) return duplicate<T>(true);
  if (!std::strcmp(mode, "valid")) return valid<T>();
  CHECK(!std::strcmp(mode, "workflow")); return workflow<T>();
}
int main(int argc, char ** argv) {
  CHECK(argc == 3); initProbes();
  return !std::strcmp(argv[1], "node") ? dispatch<ProbeNode>(argv[2]) : dispatch<ProbeEngine>(argv[2]);
}
