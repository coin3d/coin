/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_MANIPULATORS

/*!
  \class SoTrackballManip SoTrackballManip.h Inventor/manips/SoTrackballManip.h
  \brief The SoTrackballManip wraps an SoTrackballDragger for convenience.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/trackball.png">
  </center>

  The manipulator class takes care of wrapping up the
  SoTrackballDragger in a simple and convenient API for the
  application programmer, making it automatically surround the
  geometry it influences and taking care of the book-keeping routines
  for it's interaction with the relevant fields of an SoTransformation
  node.

  Here's a dead simple, stand-alone example on how to set up an
  SoTrackballManip in a scenegraph:

  \code
  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/manips/SoTrackballManip.h>
      
  int
  main(int argc, char* argv[])
  {
    QWidget * mainwin = SoQt::init(argv[0]);
  
    SoSeparator * root = new SoSeparator;
    root->ref();
  
    SoTrackballManip * manip = new SoTrackballManip;
    root->addChild(manip);
  
    root->addChild(new SoCone);
      
    SoQtExaminerViewer * viewer = new SoQtExaminerViewer(mainwin);
    viewer->setSceneGraph(root);
      
    SoQt::show(mainwin);
    SoQt::mainLoop();
  
    root->unref();
      
    return 0;
  }
  \endcode

  Compile and hit 'ESC' to go into scenegraph interaction mode in the
  examinerviewer, then change the rotation of the cone by interacting
  with the dragger bands.

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/trackball-cone.png">
  </center>
*/

#include <Inventor/manips/SoTrackballManip.h>

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTrackballDragger.h>

#include "nodes/SoSubNodeP.h"

class SoTrackballManipP {
public:
};

SO_NODE_SOURCE(SoTrackballManip);


// doc in super
void
SoTrackballManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTrackballManip, SO_FROM_INVENTOR_1);
}

/*!
  Default constructor. Allocates an SoTrackballDragger and an
  SoSurroundScale node to surround the geometry within our part of the
  scenegraph.
*/
SoTrackballManip::SoTrackballManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTrackballManip);

  SoTrackballDragger *dragger = new SoTrackballDragger;
  this->setDragger(dragger);

  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}

/*!
  Destructor.
*/
SoTrackballManip::~SoTrackballManip()
{
}

#endif // HAVE_MANIPULATORS
