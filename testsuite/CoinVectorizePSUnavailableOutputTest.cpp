#include <Inventor/SbColor.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/annex/HardCopy/SoVectorOutput.h>
#include <Inventor/annex/HardCopy/SoVectorizePSAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>

#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

int
main(void)
{
  for (int fd = 0; fd <= 2; ++fd) {
    if (fcntl(fd, F_GETFD) == -1) return 77;
  }

  SoDB::init();

  struct rlimit original;
  if (getrlimit(RLIMIT_NOFILE, &original) != 0 || original.rlim_max < 3) {
    SoDB::finish();
    return 77;
  }
  struct rlimit exhausted = original;
  exhausted.rlim_cur = 3;
  if (setrlimit(RLIMIT_NOFILE, &exhausted) != 0) {
    SoDB::finish();
    return 77;
  }

  int result = 0;
  {
    SoVectorizePSAction action;
    if (action.getOutput()->getFilePointer() != NULL) result = 1;

    SoSeparator * root = new SoSeparator;
    root->ref();
    root->addChild(new SoCube);

    SoDrawStyle * style = new SoDrawStyle;
    style->linePattern = 0x0f0f;
    style->lineWidth = 3.0f;
    style->pointSize = 4.0f;
    root->addChild(style);
    SoCoordinate3 * coords = new SoCoordinate3;
    coords->point.set1Value(0, -0.5f, -0.5f, 0.0f);
    coords->point.set1Value(1, 0.5f, 0.5f, 0.0f);
    root->addChild(coords);
    SoLineSet * lines = new SoLineSet;
    lines->numVertices = 2;
    root->addChild(lines);
    SoPointSet * points = new SoPointSet;
    points->numPoints = 2;
    root->addChild(points);

    SoText2 * text = new SoText2;
    text->string = "test";
    root->addChild(text);

    static const unsigned char pixels[4 * 3] = { 0 };
    SoImage * image = new SoImage;
    image->image.setValue(SbVec2s(2, 2), 3, pixels);
    root->addChild(image);

    action.setBackgroundColor(TRUE, SbColor(1.0f, 1.0f, 1.0f));
    const SoVectorizeAction::PointStyle pointstyles[] = {
      SoVectorizeAction::CIRCLE, SoVectorizeAction::SQUARE
    };
    for (int i = 0; i < 2; ++i) {
      action.setPointStyle(pointstyles[i]);
      action.beginPage(SbVec2f(0.0f, 0.0f), SbVec2f(100.0f, 100.0f));
      action.calibrate(SbViewportRegion(100, 100));
      action.beginViewport();
      action.apply(root);
      action.endViewport();
      action.endPage();
    }

    // If the output becomes available in the middle of a page, nothing
    // must be written before the next page starts.
    action.beginPage(SbVec2f(0.0f, 0.0f), SbVec2f(100.0f, 100.0f));
    if (setrlimit(RLIMIT_NOFILE, &original) != 0) result = 1;
    char filename[] = "/tmp/coin-vectorize-ps-XXXXXX";
    const int descriptor = mkstemp(filename);
    const int savedstdout = dup(STDOUT_FILENO);
    if (descriptor == -1 || savedstdout == -1) {
      result = 1;
    }
    else {
      dup2(descriptor, STDOUT_FILENO);
      FILE * stream = action.getOutput()->getFilePointer();
      if (stream == NULL) {
        result = 1;
      }
      else {
        action.apply(root);
        action.endPage();
        fflush(stream);
        struct stat info;
        if (fstat(descriptor, &info) != 0 || info.st_size != 0) result = 1;

        action.beginPage(SbVec2f(0.0f, 0.0f), SbVec2f(100.0f, 100.0f));
        action.apply(root);
        action.endPage();
        fflush(stream);
        char start[5] = { 0 };
        if (pread(descriptor, start, 4, 0) != 4 ||
            strcmp(start, "%!PS") != 0) {
          result = 1;
        }
      }
      dup2(savedstdout, STDOUT_FILENO);
    }
    if (savedstdout != -1) close(savedstdout);
    if (descriptor != -1) {
      close(descriptor);
      unlink(filename);
    }
    root->unref();
  }

  SoDB::finish();
  return fcntl(1, F_GETFD) == -1 ? 1 : result;
}
