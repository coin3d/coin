
#include <assert.h>
#include <stdio.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/actions/SoWriteAction.h>

int
main(int argc, char ** argv)
{
  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  if ( argc != 3 ) {
    fprintf(stdout, "Usage: %s infile outfile\n", argv[0]);
    return 0;
  }

  SoInput * in = new SoInput;
  if ( !in->openFile(argv[1]) ) {
    fprintf(stderr, "error: could not open file '%s'\n", argv[1]);
    delete in;
    SoDB::cleanup();
    return -1;
  }

  SoNode * scene = NULL;
  if ( !SoDB::read(in, scene) ) {
    fprintf(stderr, "error: could not read file '%s'\n", argv[1]);
    delete in;
    SoDB::cleanup();
    return -1;
  }
  delete in;
  scene->ref();

  SoOutput * out = new SoOutput;
  if ( !out->openFile(argv[2]) ) {
    fprintf(stderr, "error: could not open file '%s' for writing\n");
    scene->unref();
    delete out;
    SoDB::cleanup();
    return -1;
  }
  
  SoWriteAction wa(out);
  wa.apply(scene);

  out->closeFile();
  delete out;

  scene->unref();

  SoDB::cleanup();
  return 0;
}

