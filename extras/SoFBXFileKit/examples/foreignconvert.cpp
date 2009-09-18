#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/VRMLnodes/SoVRML.h>
#include <Inventor/annex/ForeignFiles/SoForeignFileKit.h>

int main(int argc, char *argv[])
{
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s <infile.fbx> [<outfile.iv>]\n", argv[0]);
    exit(1);
  }

  SoDB::init();
  SoInteraction::init();

  SoInput in;
  if (!in.openFile(argv[1])) {
    exit(1);
  }
  fprintf(stderr, "Reading...\n");

  SbBool vrml2 = in.isFileVRML2();
  
  SoGroup  *root = 
    !vrml2 ? (SoGroup *)SoDB::readAll(&in) : (SoGroup *)SoDB::readAllVRML(&in);
  if (root) {
    root->ref();

    SoSearchAction sa;
    sa.setType(SoForeignFileKit::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(FALSE);
    sa.apply(root);
    SoPathList results = sa.getPaths();
    for (int i=0;i<results.getLength();i++) {
      SoPath *path = results[i];
      assert(path->getTail()->isOfType(SoForeignFileKit::getClassTypeId()));
      SoForeignFileKit *ffkit = (SoForeignFileKit *)path->getTail();
      SoGroup *parent = (SoGroup *)path->getNodeFromTail(1);
      parent->replaceChild(path->getIndexFromTail(1), ffkit->convert());
    }

    SbString hdr = in.getHeader();
    if (hdr.getLength() == 0) hdr = "#Inventor V2.1 ascii";
    in.closeFile();

    fprintf(stderr, "Writing...\n");

    SoOutput out;
    SbString outfile;
    if (argc == 3) outfile += argv[2];
    else {
      outfile += argv[1];
      outfile += ".iv";
    }
    out.openFile(outfile.getString());
    out.setHeaderString(hdr);
    SoWriteAction wra(&out);
    wra.apply(root);

    root->unref();
  }
  else {
    fprintf(stderr, "Error reading file\n");
  }
}
