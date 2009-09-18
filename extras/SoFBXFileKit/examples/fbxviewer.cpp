#include <Inventor/Qt/SoQt.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/VRMLnodes/SoVRML.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <QtOpenGL/QGLFormat>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

void made_selection(void * userdata, SoPath * path)
{
  SoFullPath *fullpath = (SoFullPath *)path;
  fprintf(stdout, "%s\n", fullpath->getNodeFromTail(2)->getName().getString());
  
  fullpath->getHead()->touch(); // to redraw
}

int
main(int argc, char ** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <infile.fbx>\n", argv[0]);
    exit(1);
  }

  QWidget * window = SoQt::init(argv[0]);

  SoInput in;
  SoSeparator *fileroot = NULL;
  const char *filename = argv[1];
  SoInput::addDirectoryLast(QFileInfo(filename).dir().path().toUtf8());
  SoInput::addDirectoryLast((QFileInfo(filename).dir().path() + "/../assets").toUtf8());
  SoInput::addDirectoryLast((QFileInfo(filename).dir().path() + "/../textures").toUtf8());
  if (in.openFile(filename)) {
    fileroot = SoDB::readAll(&in);
    if (!fileroot) {
      fprintf(stderr, "Unable to read file %s\n",argv[1]);
      exit(1);
    }
  }
  else {
    fprintf(stderr, "Unable to open file %s\n",argv[1]);
    exit(1);
  }

  QGLFormat f;
  f.setSampleBuffers(true);
  f.setSamples(4);
  QGLFormat::setDefaultFormat(f);

  SoQtExaminerViewer * viewer = new SoQtExaminerViewer(window);

  SoSeparator *root = new SoSeparator;
  root->ref();
  // Increase texture quality
  SoComplexity *complexity = new SoComplexity;
  complexity->textureQuality = 1;
  root->addChild(complexity);
  // Turn on two-sided lighting and handle non-convex polygons
  SoShapeHints *shapehints = new SoShapeHints;
  shapehints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  shapehints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
  // Don't add shapehints since it makes some models fail
  //  root->addChild(shapehints);
  SoSelection *selection = new SoSelection;
  root->addChild(selection);
  selection->addChild(fileroot);
  selection->addSelectionCallback(made_selection, (void *)1L);

  SoBoxHighlightRenderAction *ra = new SoBoxHighlightRenderAction;
  ra->setLineWidth(1);
  viewer->setGLRenderAction(ra);
  viewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
  viewer->setBackgroundColor(SbColor(0.3,0.3,0.3));
  viewer->setSceneGraph(root);
  SoCamera *cam = viewer->getCamera();
  cam->orientation.setValue(SbVec3f(-0.80, 0.57, 0.20), 0.80);
  viewer->viewAll();
  viewer->show();
  SoQt::show(window);

  // FIXME: Temporary setting until this is supported in Coin. kintel 20090218.
#ifdef _WIN32
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81f8
#define GL_SEPARATE_SPECULAR_COLOR   0x81fa
#endif
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  SoQt::mainLoop();

  delete viewer;
  return 0;
} // main()

// *************************************************************************
 
