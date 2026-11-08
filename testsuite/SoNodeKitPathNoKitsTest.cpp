#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoSeparator.h>

int
main()
{
  SoDB::init();
  SoPath * path = new SoPath;
  path->ref();

  const SoNodeKitPathView view = path->nodeKitPath();
  bool correct = view.getLength() == 0 && view.getTail() == NULL &&
    view.getNode(0) == NULL && view.getNodeFromTail(0) == NULL;

  SoSeparator * head = new SoSeparator;
  head->ref();
  path->setHead(head);
  correct = correct && view.getLength() == 0 && view.getTail() == NULL &&
    view.getNode(0) == NULL && view.getNodeFromTail(0) == NULL;

  path->unref();
  head->unref();
  SoDB::finish();
  return correct ? 0 : 1;
}
