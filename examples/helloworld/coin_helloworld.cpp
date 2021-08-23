#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>

int main(int argc, char** argv)
{
	
	QWidget* mWindow = SoQt::init(argc, argv, argv[0]);
	SoSeparator* mRoot = new SoSeparator;
	mRoot->ref();
	SoCone* mCone = new SoCone;
	mRoot->addChild(mCone);
	SoQtExaminerViewer eviewer(mWindow);
	eviewer.setSceneGraph(mRoot);
	eviewer.show();
	SoQt::show(mWindow);
	SoQt::mainLoop();
	mRoot->unref();
	return 0;
}