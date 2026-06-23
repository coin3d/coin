#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>

int main(int argc, char** argv)
{
	QWidget* mWindow = SoQt::init(argc, argv, argv[0]);
	SoSeparator* mRoot = nullptr;
	if (argc < 2) {
		// Create simple scene graph
		mRoot = new SoSeparator;
		mRoot->ref();
		mRoot->addChild(new SoCone);
	} else {
		// Read scene graph from file
		SoInput in;
		if (!in.openFile(argv[1])) {
			qWarning("could not open file %s\n", argv[1]);
			exit(1);
		}
		mRoot = SoDB::readAll(&in);
		if (!mRoot) {
			qWarning("could not read file %s\n", argv[1]);
			exit(1);
		}
		mRoot->ref();
	}
	SoQtExaminerViewer eviewer(mWindow);
	eviewer.setSceneGraph(mRoot);
	eviewer.show();
	SoQt::show(mWindow);
	SoQt::mainLoop();
	mRoot->unref();
	return 0;
}