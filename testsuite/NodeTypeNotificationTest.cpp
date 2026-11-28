#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoVertexAttribute.h>
#include <Inventor/engines/SoNodeEngine.h>
#ifdef TEST_NODEKITS
#include <Inventor/nodekits/SoNodeKit.h>
#endif
#include <cstring>
#include <cstdio>
class LocalInvalidationCube : public SoCube {
public:
  LocalInvalidationCube() : notifications(0), cacheValid(true) {}
  void notify(SoNotList * list) override {
    ++notifications;
    cacheValid=false;
    SoCube::notify(list);
  }
  int notifications;
  bool cacheValid;
protected:
  ~LocalInvalidationCube() override {}
};
class ConstructorNotificationCube : public SoCube {
  SO_NODE_HEADER(ConstructorNotificationCube);
public:
  static void initClass() {
    SO_NODE_INIT_CLASS(ConstructorNotificationCube,SoCube,"Cube");
  }
  ConstructorNotificationCube() : ready(false), premature(0), notifications(0) {
    SO_NODE_CONSTRUCTOR(ConstructorNotificationCube);
    this->setNodeType(SoNode::VRML2);
    this->ready=true;
  }
  void notify(SoNotList * list) override {
    ++this->notifications;
    if (!this->ready) ++this->premature;
    SoCube::notify(list);
  }
  bool ready;
  int premature;
  int notifications;
protected:
  ~ConstructorNotificationCube() override {}
};
SO_NODE_SOURCE(ConstructorNotificationCube);
static int failures;
static void check(bool condition, const char * message) { if (!condition) { std::fprintf(stderr,"%s\n",message); ++failures; } }
static void count(void * data, SoSensor *) { ++*static_cast<int *>(data); }
int main() {
  SoDB::init();
  ConstructorNotificationCube::initClass();
#ifdef TEST_NODEKITS
  SoNodeKit::init();
#endif
  {
    ConstructorNotificationCube * probe=new ConstructorNotificationCube;
    probe->ref();
    check(probe->premature==0 && probe->notifications==0,
          "constructor node type does not dispatch virtual notification");
    probe->touch();
    check(probe->premature==0 && probe->notifications==1,
          "ordinary touch still dispatches virtual notification");
    probe->unref();
  }
  // A type change in a constructor must never dispatch notify into a
  // subclass before its private data or fields have been initialized.
  SoTypeList types; SoType::getAllDerivedFrom(SoNode::getClassTypeId(),types);
  for (int i=0; i<types.getLength(); ++i) {
    SoType t=types[i]; if (!t.canCreateInstance()) continue;
    std::fprintf(stderr,"constructing %s\n",t.getName().getString());
    SoNode * one=static_cast<SoNode *>(t.createInstance()); one->ref();
    SoNode * two=static_cast<SoNode *>(t.createInstance()); two->ref();
    check(one->getTypeId()==t && two->getTypeId()==t,"builtin factory type");
    check(one->getNodeType()==two->getNodeType(),"construction preserves deterministic node type");
    SoFieldList fields1, fields2; one->getFields(fields1); two->getFields(fields2);
    check(fields1.getLength()==fields2.getLength(),"constructor initializes field list");
    for (int j=0; j<fields1.getLength() && j<fields2.getLength(); ++j)
      check(fields1[j]->isDefault()==fields2[j]->isDefault(),"constructor preserves field defaults");
    if (std::strncmp(t.getName().getString(),"VRML",4)==0) {
      // VRML node engines use SoNodeEngine's native INVENTOR default;
      // the other VRML builtins explicitly select VRML2 in construction.
      const bool nodeEngine=t.isDerivedFrom(SoNodeEngine::getClassTypeId());
      const SoNode::NodeType nativeType=nodeEngine ? SoNode::INVENTOR : SoNode::VRML2;
      check(one->getNodeType()==nativeType,"VRML builtin construction retains native node type");
    }
    if (t==SoVertexAttribute::getClassTypeId()) {
      check(one->getNodeType()==SoNode::COIN_3_0,"VertexAttribute construction retains compatibility type");
      SoVertexAttribute * attribute=static_cast<SoVertexAttribute *>(one);
      check(attribute->name.getValue()==SbName::empty(),"VertexAttribute default name");
      attribute->touch(); // Its notify implementation may now safely read PImpl.
    }
    two->unref(); one->unref();
  }
  {
    SoGroup * parent=new SoGroup; parent->ref();
    LocalInvalidationCube * invalidator=new LocalInvalidationCube;
    parent->addChild(invalidator);
    int localSensors=0, parentSensors=0;
    SoNodeSensor localSensor(count,&localSensors), parentSensor(count,&parentSensors);
    localSensor.setPriority(0); parentSensor.setPriority(0);
    localSensor.attach(invalidator); parentSensor.attach(parent);
    invalidator->enableNotify(FALSE);
    const auto localId=invalidator->getNodeId(), parentId=parent->getNodeId();
    invalidator->setNodeType(SoNode::VRML1);
    check(invalidator->notifications==0 && invalidator->cacheValid,
          "node type propagation bypasses subclass virtual notification");
    check(invalidator->getNodeId()!=localId,
          "disabled propagation still updates subclass local node ID");
    check(parent->getNodeId()==parentId && localSensors==0 && parentSensors==0,
          "subclass local invalidation does not propagate while disabled");
    const auto changedId=invalidator->getNodeId();
    invalidator->cacheValid=true;
    invalidator->setNodeType(SoNode::VRML1);
    check(invalidator->notifications==0 && invalidator->cacheValid &&
          invalidator->getNodeId()==changedId,
          "same type remains a complete no-op while propagation is disabled");
    invalidator->enableNotify(TRUE); invalidator->touch();
    check(invalidator->notifications==1 && !invalidator->cacheValid &&
          localSensors==1 && parentSensors==1,
          "reenabled touch invokes subclass and propagates normally");
    localSensor.detach(); parentSensor.detach(); parent->unref();
  }
  SoGroup * root = new SoGroup; root->ref();
  SoGroup * other = new SoGroup; other->ref();
  SoGroup * middle = new SoGroup; root->addChild(middle);
  SoCube * node = new SoCube; middle->addChild(node); other->addChild(node);
  node->setOverride(TRUE);
  int local=0, ancestor=0, second=0;
  SoNodeSensor ns(count,&local), rs(count,&ancestor), os(count,&second);
  ns.setPriority(0); rs.setPriority(0); os.setPriority(0);
  ns.attach(node); rs.attach(root); os.attach(other);
  auto nid=node->getNodeId(), rid=root->getNodeId(), oid=other->getNodeId();
  node->setNodeType(SoNode::VRML1);
  check(node->getNodeType()==SoNode::VRML1,"type applied");
  check(node->isOverride(),"override preserved");
  check(node->getNodeId()!=nid,"changed type updates local ID");
  check(root->getNodeId()!=rid && other->getNodeId()!=oid,"changed type updates both DAG ancestors");
  check(local==1 && ancestor==1 && second==1,"changed type notifies sensors exactly once");
  nid=node->getNodeId(); rid=root->getNodeId(); oid=other->getNodeId();
  node->setNodeType(SoNode::VRML1);
  check(node->getNodeId()==nid && root->getNodeId()==rid && other->getNodeId()==oid,"same type is ID no-op");
  check(local==1 && ancestor==1 && second==1,"same type is sensor no-op");
  node->enableNotify(FALSE);
  node->setNodeType(SoNode::VRML2);
  check(node->getNodeType()==SoNode::VRML2 && node->isOverride(),"disabled notification still applies flags");
  check(root->getNodeId()==rid && other->getNodeId()==oid,"disabled notification blocks ancestors");
  check(local==1 && ancestor==1 && second==1,"disabled notification blocks sensors");
  node->enableNotify(TRUE); node->touch();
  check(local==2 && ancestor==2 && second==2,"touch after reenable propagates normally");
  node->setNodeType(SoNode::INVENTOR);
  check(node->getNodeType()==SoNode::INVENTOR && node->isOverride(),"roundtrip preserves override");
  SoNode * copy=node->copy(); copy->ref();
  check(copy->getNodeType()==node->getNodeType() && copy->isOverride(),"copy preserves type and override");
  copy->unref(); ns.detach(); rs.detach(); os.detach(); other->unref(); root->unref();
  return failures ? 1 : 0;
}
