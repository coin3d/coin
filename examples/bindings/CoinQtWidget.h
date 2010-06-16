#ifndef COIN_QTWIDGET_H
#define COIN_QTWIDGET_H

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

#include <qgl.h>

#include <Inventor/SbBasic.h>

class SoNode;
class QDialog;

// *************************************************************************

//FIXME: Split this class in one CoinQt3Widget and one CoinQt4Widget
//file, and port the last one to Qt4. BFG 20090316

class CoinQtWidget : public QGLWidget {
  typedef QGLWidget inherited;
  Q_OBJECT

public:
  CoinQtWidget(QWidget * parent = NULL, const char * name = NULL,
               const QGLWidget * shareWidget = NULL, WFlags f = 0);
  CoinQtWidget(const QGLFormat & format, QWidget * parent = NULL,
               const char * name = NULL, const QGLWidget * shareWidget = NULL,
               WFlags f = 0);
  virtual ~CoinQtWidget(void);

  virtual SbBool setSceneGraph(SoNode * root, SbBool managed = FALSE);
  virtual SoNode * getSceneGraph(void) const;

public slots:
  virtual void tick(void);
  virtual void about(void);

protected:
  virtual void resizeGL(int width, int height);
  virtual void initializeGL(void);
  virtual void paintGL(void);
  virtual void glInit(void);
  virtual void glDraw(void);

  virtual void mousePressEvent(QMouseEvent * event);
  virtual void mouseReleaseEvent(QMouseEvent * event);
  virtual void mouseMoveEvent(QMouseEvent * event);
  virtual void keyPressEvent(QKeyEvent * event);
  virtual void keyReleaseEvent(QKeyEvent * event);

  virtual void updateGL(void);

private:
  class CoinQtWidgetP * internals;

}; // CoinQtWidget

// *************************************************************************

#endif /* !COIN_QTWIDGET_H */
