#include <Inventor/SoDB.h>
#include <Inventor/VRMLnodes/SoVRMLScript.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <cstdio>
#include <string>
static void count(void * data, SoSensor *) { ++*static_cast<int *>(data); }
static int failures;
static void check(bool condition, const char * message) {
  if (!condition) { std::fprintf(stderr,"%s\n",message); ++failures; }
}
int main(int argc, char ** argv) {
  SoDB::init();
  const std::string mode=argc>1 ? argv[1] : "touch";
  SoGroup * root=new SoGroup; root->ref();
  SoVRMLScript * script=new SoVRMLScript; root->addChild(script);
  int local=0, parent=0;
  SoNodeSensor localSensor(count,&local), parentSensor(count,&parent);
  localSensor.setPriority(0); parentSensor.setPriority(0);
  localSensor.attach(script); parentSensor.attach(root);
  auto sid=script->getNodeId(), rid=root->getNodeId();
  if (mode=="type") {
    check(script->getNodeType()==SoNode::VRML2,"script starts with VRML2 semantics");
    script->setNodeType(SoNode::INVENTOR);
    check(script->getNodeType()==SoNode::INVENTOR,"script applies changed NodeType");
  } else if (mode=="fields") {
    script->directOutput=TRUE;
    check(script->directOutput.getValue(),"ordinary Script field remains writable");
  } else {
    // Public touch carries no lastField. This case must demonstrate the
    // Script defect independently of the setNodeType notification fix.
    script->touch();
  }
  check(script->getNodeId()!=sid,"Script notification updates local ID");
  check(root->getNodeId()!=rid,"Script notification reaches ancestor");
  check(local==1 && parent==1,"Script notification reaches sensors once");
  if (mode=="fields") {
    sid=script->getNodeId(); rid=root->getNodeId();
    script->mustEvaluate=TRUE;
    check(script->mustEvaluate.getValue(),"mustEvaluate remains writable");
    check(script->getNodeId()!=sid && root->getNodeId()!=rid,"mustEvaluate notification reaches IDs");
    check(local==2 && parent==2,"mustEvaluate notification reaches sensors once");
  }
  localSensor.detach(); parentSensor.detach(); root->unref();
  return failures ? 1 : 0;
}
