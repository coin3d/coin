/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif // !_WIN32
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>

/* These two externs are for interfacing against getopt(). */

#ifndef _WIN32
extern int optind;
extern char * optarg;
#endif // _WIN32

void
usage(const char * argv_0)
{
  fprintf(stderr, "\nUsage: %s [-bfth] [-o filename] file ...\n", argv_0);

  fprintf(stderr, "\t-b:\twrite in binary format\n");
  fprintf(stderr, "\t-f:\tflatten SoFile nodes\n");
  fprintf(stderr, "\t-t:\tinline all textures\n");
  fprintf(stderr, "\t-h:\tshow usage\n");
  fprintf(stderr, "\t-o:\twrite to ``filename'' instead of stdout\n");

  fprintf(stderr,
          "\n  ``%s'' will read all files given on the command line,\n",
          argv_0);
  fprintf(stderr,
          "  compile them into a single scene and write them back out\n");
  fprintf(stderr,
          "  again on stdout (unless the ``-o'' option is used).\n\n");
  fprintf(stderr,
          "  Use ``-'' or no input files to read from stdin.\n\n");
}

int
main(int argc, char * argv[])
{
  int writeasbinary = 0;
  int flattenfiles = 0;
  int flattentextures = 0;
  const char * outname = NULL;

  /* Parse command line. */
#ifndef _WIN32
  int getoptchar;
  while ((getoptchar = getopt(argc, argv, "bftho:")) != EOF) {
    char c = (char)getoptchar;
    switch (c) {
    case 'b':
      writeasbinary = 1;
      break;
    case 'f':
      flattenfiles = 1;
      break;
    case 't':
      flattentextures = 1;
      break;
    case 'h':
      usage(argv[0]);
      exit(0);
      break;
    case 'o':
      outname = optarg;
      if (!outname) {
        usage(argv[0]);
        exit(1);
      }
      break;
    case '?':
      usage(argv[0]);
      exit(1);
      break;
    }
  }
#endif // _WIN32
  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

#ifndef _WIN32
  int i = optind;
#else
  int i = 1;
#endif
  SoInput stdinp;
  SoSeparator * root = new SoSeparator;

  if (i >= argc) {
    /* No files specified, read from stdin only. */
    SoSeparator * tmp = SoDB::readAll(&stdinp);
    if (tmp) root->addChild(tmp);
  }
  else {
    SoInput fileinp;

    for (; i < argc; i++) {
      SoInput * inp = NULL;

      if (strcmp("-", argv[i]) == 0) inp = &stdinp;
      else if (fileinp.openFile(argv[i])) inp = &fileinp;
      else fprintf(stderr, "Couldn't open file '%s' -- skipping.\n", argv[i]);

      if (inp) {
        SoSeparator * tmproot = SoDB::readAll(inp);
        if (tmproot) {
          root->addChild(tmproot->getNumChildren() == 1 ?
                         tmproot->getChild(0) : tmproot);
        }
        else {
          fprintf(stderr, "Couldn't read file '%s' -- skipping.\n", argv[i]);
        }

        if (inp == &fileinp) inp->closeFile();
      }
    }
  }

  if (flattenfiles) {
    // FIXME: flatten SoFile nodes. 19991009 mortene.
    fprintf(stderr, "Warning: SoFile flattening not supported yet!\n");
  }

  if (flattentextures) {
    // FIXME: inline SoTexture2 nodes. 19991009 mortene.
    fprintf(stderr, "Warning: texture inlining not supported yet!\n");
  }

  if (root->getNumChildren() > 0) {
    SoOutput out;
    if (outname) {
      if (!out.openFile(outname)) {
        fprintf(stderr, "Couldn't write output to '%s'.\n", outname);
        exit(1);
      }
    }
    out.setBinary(writeasbinary);

    SoWriteAction wa(&out);
    for (int i=0; i < root->getNumChildren(); i++) {
      wa.apply(root->getChild(i));
    }
  }

  return 0;
}
