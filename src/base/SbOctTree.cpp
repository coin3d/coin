/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbOctTree.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbPlane.h>
#include <float.h>
#include <stddef.h>

/*!
  \class SbOctTree SbOctTree.h Inventor/SbOctTree.h
  \brief The SbOctTree class defines a generic oct tree for fast geometry searches.
  \ingroup base

  Note: SbOctTree is an extension versus the Open Inventor API.
*/

/*!
  \struct SbOctTreeFuncs SbOctTree.h Inventor/SbOctTree.h
  The SbOctTreeFuncs struct is used to specify callback functions for
  items SbOcTree.

  The only function that must be set is the \e bboxfunc. The other functions
  must be set if you intend to use the corresponding find methods in
  SbOctTree.
*/

/*!
  \var SbOctTreeFuncs::bboxfunc
  Should return the bounding box of the item.
 */
/*!
  \var SbOctTreeFuncs::ptinsidefunc
  Should return whether a point is inside item..
*/
/*!
  \var SbOctTreeFuncs::insideboxfunc
  Should return whether item is inside a box.
 */
/*!
  \var SbOctTreeFuncs::insidespherefunc
  Should return whether item is inside sphere.
 */
/*!
  \var SbOctTreeFuncs::insideplanesfunc
  Should return whether item is inside planes.
*/

//
// got to have unique intersection funcs, therefore the standard
// Inventor intersection functions won't do. E.g. SbBox3f::pointInside()
// will return TRUE for all eight child-boxes if the center point of the
// parent box is tested, which is correct, but not really usable for an
// oct tree.
//

static SbBool
intersect_box_sphere(const SbBox3f & box,
                     const SbSphere & sphere)
{
  const SbVec3f &C = sphere.getCenter();
  const SbVec3f &Bmin = box.getMin();
  const SbVec3f &Bmax = box.getMax();
  float dmin = 0;
  for (int i = 0; i < 3; i++) {
    if (C[i] < Bmin[i]) dmin += SbSqr(C[i] - Bmin[i]);
    else if (C[i] > Bmax[i]) dmin += SbSqr(C[i] - Bmax[i]);
  }
  return (dmin <= SbSqr(sphere.getRadius()));
}

static SbBool
intersect_box_box(const SbBox3f & box1, const SbBox3f & box2)
{
  return ! (box1.getMin()[0] >= box2.getMax()[0] ||
            box1.getMax()[0] < box2.getMin()[0] ||
            box1.getMin()[1] >= box2.getMax()[1] ||
            box1.getMax()[1] < box2.getMin()[1] ||
            box1.getMin()[2] >= box2.getMax()[2] ||
            box1.getMax()[2] < box2.getMin()[2]);
}

static SbBool
point_inside_box(const SbVec3f & pt, const SbBox3f & box)
{
  return ! (pt[0] < box.getMin()[0] ||
            pt[0] >= box.getMax()[0] ||
            pt[1] < box.getMin()[1] ||
            pt[1] >= box.getMax()[1] ||
            pt[2] < box.getMin()[2] ||
            pt[2] >= box.getMax()[2]);
}

static SbBool
box_inside_planes(const SbBox3f & box, const SbPlane * const planes,
                  const int numplanes)
{
  // Uses box "radius" for speed.
  // FIXME: consider just checking all 8 points of the box. pederb, 20000811
  SbVec3f size = (box.getMax() - box.getMin()) * 0.5f;
  float radius = (float)sqrt(size[0]*size[0] + size[1]*size[1] + size[2]*size[2]);

  SbVec3f center = (box.getMin() + box.getMax()) * 0.5f;

  for (int i = 0; i < numplanes; i++) {
    if (planes[i].getDistance(center) < -radius) return FALSE;
  }
  return TRUE;
}

class SbOctTreeNode
{
public:

  SbOctTreeNode(void);
  ~SbOctTreeNode();

  void addItem(void * const item,
               const SbOctTreeFuncs & itemfuncs,
               const int maxitems,
               const SbBox3f &nodesize);
  void removeItem(void * const item,
                  const SbOctTreeFuncs & itemfuncs,
                  const SbBox3f &nodesize);
  void findItems(const SbVec3f &pos,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize,
                 const SbBool removeduplicates) const;
  void findItems(const SbBox3f &box,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize,
                 const SbBool removeduplicates) const;
  void findItems(const SbSphere &sphere,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize,
                 const SbBool removeduplicates) const;
  void findItems(const SbPlane * const planes,
                 const int numPlanes,
                 SbList <void*> &destarray,
                 const SbOctTreeFuncs &itemfuncs,
                 const SbBox3f &nodesize,
                 const SbBool removeduplicates) const;

  void debugTree(FILE *fp, const int indent);

private:

  void splitBox(const SbBox3f & box, SbBox3f * destarray) const;
  SbBool splitNode(const SbBox3f & nodesize, const SbOctTreeFuncs & funcs);

  SbOctTreeNode * children[8];
  SbList <void*> items;
};

void 
SbOctTreeNode::debugTree(FILE *fp, const int indent)
{
  int i;
  for (i = 0; i < indent; i++) fprintf(fp," ");
  fprintf(fp, "Node: %d\n", this->items.getLength());
  if (this->children[0]) {
    for (i =0 ; i < 8; i++) {
      this->children[i]->debugTree(fp, indent+1);
    }
  }
}

static void
add_to_array(SbList <void*> & array, void * ptr)
{
  int i, n= array.getLength();
  for (i = 0; i < n; i++) {
    if (array[i] == ptr) return;
  }
  array.append(ptr);
}

SbOctTreeNode::SbOctTreeNode(void)
{
  for (int i = 0; i < 8; i++)
    this->children[i] = NULL;
}

SbOctTreeNode::~SbOctTreeNode()
{
  if (this->children[0]) {
    for (int i = 0; i < 8; i++) delete children[i];
  }
}

void
SbOctTreeNode::addItem(void * const item,
                       const SbOctTreeFuncs & itemfuncs,
                       const int maxitems,
                       const SbBox3f & nodesize)
{
  if (this->children[0]) { // node has been split
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (itemfuncs.insideboxfunc(item, childbox[i])) {
        this->children[i]->addItem(item, itemfuncs,
                                   maxitems, childbox[i]);
      }
    }
  }
  else if (this->items.getLength() >= maxitems) {
    // avoid trying a split too often by using a modulo
    if ((this->items.getLength() % (maxitems+1) == maxitems) &&
        this->splitNode(nodesize, itemfuncs)) {
      this->addItem(item, itemfuncs, maxitems, nodesize);
    }
    else {
      this->items.append(item);
    }
  }
  else {
    this->items.append(item);
  }
}

void
SbOctTreeNode::removeItem(void *const item,
                          const SbOctTreeFuncs & itemfuncs,
                          const SbBox3f & nodesize)
{
  if (children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    int i;
    for (i = 0; i < 8; i++) {
      if (itemfuncs.insideboxfunc(item, childbox[i])) {
        this->children[i]->removeItem(item, itemfuncs,
                                      childbox[i]);
      }
    }
  }
  else {
    int n = this->items.getLength();
    int i;

    for (i = 0; i < n; i++) {
      if (this->items[i] == item) {
        this->items.removeFast(i);
        n--;
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbVec3f & pos,
                         SbList <void*> & destarray,
                         const SbOctTreeFuncs & itemfuncs,
                         const SbBox3f & nodesize,
                         const SbBool removeduplicates) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (point_inside_box(pos, childbox[i])) {
        children[i]->findItems(pos, destarray,
                               itemfuncs, childbox[i],
                               removeduplicates);
      }
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.ptinsidefunc(item, pos)) {
        if (removeduplicates)
          add_to_array(destarray, item);
        else
          destarray.append(item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbBox3f & box,
                         SbList <void*> & destarray,
                         const SbOctTreeFuncs & itemfuncs,
                         const SbBox3f & nodesize,
                         const SbBool removeduplicates) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (intersect_box_box(box, childbox[i]))
        this->children[i]->findItems(box, destarray,
                                     itemfuncs, childbox[i],
                                     removeduplicates);
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.insideboxfunc(item, box)) {
        if (removeduplicates)
          add_to_array(destarray, item);
        else
          destarray.append(item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbSphere & sphere,
                         SbList <void*> & destarray,
                         const SbOctTreeFuncs & itemfuncs,
                         const SbBox3f & nodesize,
                         const SbBool removeduplicates) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (intersect_box_sphere(childbox[i],sphere))
        this->children[i]->findItems(sphere, destarray,
                                     itemfuncs, childbox[i],
                                     removeduplicates);
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void * item = this->items[i];
      if (itemfuncs.insidespherefunc(item, sphere)) {
        if (removeduplicates)
          add_to_array(destarray, item);
        else
          destarray.append(item);
      }
    }
  }
}

void
SbOctTreeNode::findItems(const SbPlane * const planes,
                         const int numplanes,
                         SbList <void*> & destarray,
                         const SbOctTreeFuncs & itemfuncs,
                         const SbBox3f & nodesize,
                         const SbBool removeduplicates) const
{
  if (this->children[0]) {
    SbBox3f childbox[8];
    splitBox(nodesize, childbox);
    for (int i = 0; i < 8; i++) {
      if (box_inside_planes(childbox[i], planes, numplanes)) {
        this->children[i]->findItems(planes,
                                     numplanes,
                                     destarray,
                                     itemfuncs,
                                     childbox[i],
                                     removeduplicates);
      }
    }
  }
  else {
    int n = this->items.getLength();
    for (int i = 0; i < n; i++) {
      void *item = this->items[i];
      if (itemfuncs.insideplanesfunc(item, planes, numplanes)) {
        if (removeduplicates)
          add_to_array(destarray, item);
        else
          destarray.append(item);
      }
    }
  }
}

void
SbOctTreeNode::splitBox(const SbBox3f & box, SbBox3f * dest) const
{
  SbVec3f mid = (box.getMin() + box.getMax()) * 0.5f;

  for (int i = 0; i < 8; i++) {
    dest[i].setBounds((i & 4) ? box.getMin()[0] : mid[0],
                      (i & 2) ? box.getMin()[1] : mid[1],
                      (i & 1) ? box.getMin()[2] : mid[2],
                      (i & 4) ? mid[0] : box.getMax()[0],
                      (i & 2) ? mid[1] : box.getMax()[1],
                      (i & 1) ? mid[2] : box.getMax()[2]);
  }
}

SbBool
SbOctTreeNode::splitNode(const SbBox3f & nodesize,
                         const SbOctTreeFuncs & itemfuncs)
{
  int i;
  for (i = 0; i < 8; i++)
    this->children[i] = new SbOctTreeNode();

  SbBox3f childbox[8];
  splitBox(nodesize, childbox);

  int n = this->items.getLength();
  for (i = 0; i < n; i++) {
    void *item = this->items[i];
    for (int j = 0; j < 8; j++) {
      if (itemfuncs.insideboxfunc(item, childbox[j])) {
        children[j]->items.append(item);
      }
    }
  }
  int cntnodes = 0;
  for (i = 0; i < 8; i++) {
    int len = children[i]->items.getLength();
    if (len == n) cntnodes++;
  }
  if (cntnodes > 0) {
    for (i = 0; i < 8; i++) {
      delete children[i];
      children[i] = NULL;
    }
    return FALSE;
  }
  this->items.truncate(0, TRUE);
  return TRUE;
}


/*!
  Constructor.
*/
SbOctTree::SbOctTree(const SbBox3f & bbox,
                     const SbOctTreeFuncs & itemfuncs,
                     const int maxitems)
  : boundingbox(bbox),
    topnode(new SbOctTreeNode()),
    itemfuncs(itemfuncs),
    maxitemspernode(maxitems)
{
}

/*!
  Destructor.
*/
SbOctTree::~SbOctTree()
{
  delete this->topnode;
}

/*!
  Restores this oct tree to an empty oct tree. The bounding
  box will still be the same though.
*/
void
SbOctTree::clear(void)
{
  delete this->topnode;
  this->topnode = new SbOctTreeNode();
}

/*!
  Adds an item to this oct tree.
*/
void
SbOctTree::addItem(void * const item)
{
  this->topnode->addItem(item, this->itemfuncs,
                         this->maxitemspernode,
                         this->boundingbox);
}

/*!
  Removes the item from the octtree. The octtree will not be
  modified/simplified even when all items are removed.
*/
void
SbOctTree::removeItem(void * const item)
{
  this->topnode->removeItem(item, this->itemfuncs,
                            this->boundingbox);
}

/*!
  Finds all items which contains the point \a pos. Items are
  returned in \a destarray.

  If \a removeduplicates is TRUE (the default), \a destarray will not
  contain duplicate items. This is not an optimized process, so if
  you're looking for speed you should set this to FALSE and do
  your own postprocessing of the array of returned items.
*/
void
SbOctTree::findItems(const SbVec3f & pos,
                     SbList <void*> & destarray,
                     const SbBool removeduplicates) const
{
  assert(this->itemfuncs.ptinsidefunc);
  topnode->findItems(pos, destarray,
                     this->itemfuncs,
                     this->boundingbox,
                     removeduplicates);
}

/*!
  Finds all items inside \a box. Items are returned in \a destarray.

  If \a removeduplicates is TRUE (the default), \a destarray will not
  contain duplicate items. This is not an optimized process, so if
  you're looking for speed you should set this to FALSE and do
  your own postprocessing of the array of returned items.
*/
void
SbOctTree::findItems(const SbBox3f & box, SbList <void*> & destarray,
                     const SbBool removeduplicates) const
{
  assert(this->itemfuncs.insideboxfunc);
  this->topnode->findItems(box, destarray,
                           this->itemfuncs,
                           this->boundingbox,
                           removeduplicates);
}

/*!
  Finds all items inside \a sphere. Items are returned in \a destarray.

  If \a removeduplicates is TRUE (the default), \a destarray will not
  contain duplicate items. This is not an optimized process, so if
  you're looking for speed you should set this to FALSE and do
  your own postprocessing of the array of returned items.
*/
void
SbOctTree::findItems(const SbSphere & sphere,
                     SbList <void*> & destarray,
                     const SbBool removeduplicates) const
{
  assert(this->itemfuncs.insidespherefunc);
  this->topnode->findItems(sphere, destarray,
                           this->itemfuncs,
                           this->boundingbox,
                           removeduplicates);
}

/*!
  Finds all items inside \a planes. The method
  SbPlane::isInHalfSpace() should be used, and only items which are
  (partly) inside \e all planes are returned. Items are returned in \a
  destarray.

  If \a removeduplicates is TRUE (the default), \a destarray will not
  contain duplicate items. This is not an optimized process, so if
  you're looking for speed you should set this to FALSE and do
  your own postprocessing of the array of returned items.
*/
void
SbOctTree::findItems(const SbPlane * const planes,
                     const int numplanes,
                     SbList <void*> & destarray,
                     const SbBool removeduplicates) const
{
  assert(this->itemfuncs.insideplanesfunc);
  this->topnode->findItems(planes, numplanes, destarray,
                           this->itemfuncs, this->boundingbox,
                           removeduplicates);
}

/*!
  Returns a bounding box enclosing all the elements in the tree.
  This is just the same bounding box which was supplied to the
  constructor.
*/
const SbBox3f &
SbOctTree::getBoundingBox(void) const
{
  return this->boundingbox;
}

void 
SbOctTree::debugTree(FILE * fp)
{
  fprintf(fp, "Oct Tree:\n");
  if (this->topnode) this->topnode->debugTree(fp, 1);
}
