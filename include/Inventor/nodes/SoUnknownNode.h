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

#ifndef __SOUNKNOWNNODE_H__
#define __SOUNKNOWNNODE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>


class SoUnknownNode : public SoNode {
  typedef SoNode inherited;

  // FIXME: needs dynamic SoFieldData instances to store the fields?
  // 20000102 mortene.
  SO_NODE_HEADER(SoUnknownNode);

public:
  static void initClass(void);
  SoUnknownNode(void);

  void setNodeClassName(const SbName & name);

protected:
  virtual ~SoUnknownNode();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual const char * getFileFormatName(void) const;

private:
  SbName classname;
};

#endif // !__SOUNKNOWNNODE_H__
