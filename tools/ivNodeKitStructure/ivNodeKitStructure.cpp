/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <stdio.h>

// FIXME: do a proper check for this through configure.
// 19991206 mortene.
#ifndef _WIN32
#define HAVE_GETOPT 1
#endif // !_WIN32

#if HAVE_GETOPT
#include <unistd.h>
/* These two externs are for interfacing against getopt(). */
extern int optind;
extern char * optarg;
#endif // HAVE_GETOPT


void
usage(const char * invname)
{
#if HAVE_GETOPT
  fprintf(stderr, "\nUsage: %s [-h] <kitclassname>\n\n", invname);
  fprintf(stderr, "\t-h:\tshow usage\n\n");
#else // !HAVE_GETOPT
  fprintf(stderr, "\nUsage: %s <kitclassname>\n\n", invname);
#endif // !HAVE_GETOPT
}

int
main(int argc, char ** argv)
{
#if HAVE_GETOPT
  /* Parse command line. */
  int getoptchar;
  while ((getoptchar = getopt(argc, argv, "h")) != EOF) {
    char c = (char)getoptchar;
    switch (c) {
    case '?':
    case 'h':
      usage(argv[0]);
      exit(0);
      break;
    }
  }

  int i = optind;
#else // !HAVE_GETOPT
  int i = 1;
#endif // !HAVE_GETOPT

  if ((argc - i) != 1) {
    usage(argv[0]);
    exit(1);
  }

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  SoType kittype = SoType::fromName(argv[i]);
  if (kittype == SoType::badType()) {
    fprintf(stderr, "ERROR: unknown class type ``%s''.\n", argv[i]);
    exit(1);
  }
  else if (!kittype.isDerivedFrom(SoBaseKit::getClassTypeId())) {
    fprintf(stderr, "ERROR: type ``%s'' is not a nodekit.\n", argv[i]);
    exit(1);
  }

  SoBaseKit * bk = (SoBaseKit *)kittype.createInstance();

  bk->printDiagram();
  fprintf(stdout, "\n\n");
  bk->printTable();

  return 0;
}
